#include <GeneticAlgorithm/Individual.h>

Individual::Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize,
	double maximStress, std::unique_ptr<IFitnessFunction> fitnessFunction)
	: m_sizeOx{ sizeOx }, m_sizeOy{ sizeOy }, m_sizeOz{ sizeOz },
	m_elementSize{ elementSize }, m_maximStress{ maximStress },
	m_fitnessFunction{ std::move(fitnessFunction) }
{
	m_building = std::make_shared<Building>(m_sizeOx, m_sizeOy, m_sizeOz, m_elementSize);
	m_building->Build();
	m_building->AddConstraints();

	m_initialGenes = std::vector<bool>(m_building->GetCubesExistence().size(), true);
}

Individual::Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize,
	const std::vector<bool>& cubesExistence,
	double maximStress, std::unique_ptr<IFitnessFunction> fitnessFunction)
	: m_sizeOx{ sizeOx }, m_sizeOy{ sizeOy }, m_sizeOz{ sizeOz },
	m_elementSize{ elementSize }, m_maximStress{ maximStress },
	m_fitnessFunction{ std::move(fitnessFunction) }
{
	m_building = std::make_shared<Building>(m_sizeOx, m_sizeOy, m_sizeOz, m_elementSize);
	m_building->Build();
	m_building->AddConstraints();
	m_building->EliminateCubesBasedOnCubesExistence(cubesExistence);

	for (int index = 0; index < cubesExistence.size(); ++index)
		m_initialGenes.emplace_back(cubesExistence[index]);
}

Individual::Individual(const Individual& another)
{
	*this = another;
}

Individual::Individual(Individual&& another) noexcept
{
	*this = std::move(another);
}

Individual& Individual::operator=(const Individual& another)
{
	if (this != &another)
	{
		m_sizeOx = another.m_sizeOx;
		m_sizeOy = another.m_sizeOy;
		m_sizeOz = another.m_sizeOz;
		m_elementSize = another.m_elementSize;
		m_maximStress = another.m_maximStress;
		m_building = another.m_building;

		m_fitnessFunction = another.m_fitnessFunction->Clone();
	}
	return *this;
}

Individual& Individual::operator=(Individual&& another) noexcept
{
	if (this != &another)
	{
		int resetValue = 0;
		m_sizeOx = std::exchange(another.m_sizeOx, resetValue);
		m_sizeOy = std::exchange(another.m_sizeOy, resetValue);
		m_sizeOz = std::exchange(another.m_sizeOz, resetValue);
		m_elementSize = std::exchange(another.m_elementSize, resetValue);
		m_maximStress = std::exchange(another.m_maximStress, resetValue);
		m_building = std::exchange(another.m_building, nullptr);

		m_fitnessFunction = std::move(another.m_fitnessFunction);
	}
	return *this;
}

const std::shared_ptr<Building> Individual::GetBuilding() const
{
	return m_building;
}

double Individual::GetCurrentMaxStress() const
{
	return m_currentMaxStress;
}

double Individual::Evaluate()
{
	m_currentMaxStress = SimulateAndGetMaximStress();

	if (m_currentMaxStress > m_maximStress || m_currentMaxStress < EPSILON_STRESS)
		return MINIM_INDIVIDUAL_VALUE;

	int removedElements = GetNumberOfRemovedElements();
	int maxRemovedElements = std::count(m_initialGenes.begin(), m_initialGenes.end(), true);

	return m_fitnessFunction->Evaluate(removedElements, maxRemovedElements, m_currentMaxStress, m_maximStress);
}

int Individual::GetSizeOx() const
{
	return m_sizeOx;
}

int Individual::GetSizeOy() const
{
	return m_sizeOy;
}

int Individual::GetSizeOz() const
{
	return m_sizeOz;
}

void Individual::Mutation(double mutationProbability)
{
	size_t numberOfGenes = m_building->GetCubesExistence().size();

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(
		LOWER_BOUND, UPPER_BOUND, numberOfGenes);

	std::vector<bool> newCubesExistence = m_building->GetCubesExistence();

	for (size_t index = 0; index < numberOfGenes; ++index)
	{
		if (randomNumbers[index] < mutationProbability)
		{
			if (!IsOnTopLayer(index) && m_initialGenes[index])
			{
				if (m_building->GetCubesExistence()[index])
					newCubesExistence[index] = false;
				else
					newCubesExistence[index] = true;
			}
		}
	}

	m_building->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	m_building->AddCubesBasedOnCubesExistence(newCubesExistence);
}

bool Individual::operator==(const Individual& other) const
{
	for (int index = 0; index < m_building->GetCubesExistence().size(); ++index)
		if (m_building->GetCubesExistence()[index] != other.m_building->GetCubesExistence()[index])
			return false;

	return
		m_sizeOx == other.m_sizeOx &&
		m_sizeOy == other.m_sizeOy &&
		m_sizeOz == other.m_sizeOz &&
		m_elementSize == other.m_elementSize &&
		m_maximStress == other.m_maximStress;
}

std::shared_ptr<Building> Individual::CreateBuildingFromDetails(int sizeOx, int sizeOy, int sizeOz,
	double elementSize, const std::vector<bool>& cubesExistence)
{
	auto building = std::make_shared<Building>(sizeOx, sizeOy, sizeOz, elementSize);
	building->Build();
	building->AddConstraints();

	building->EliminateCubesBasedOnCubesExistence(cubesExistence);

	return building;
}

int Individual::GetNumberOfRemovedElements() const
{
	int numberOfRemovedElements = 0;
	std::vector<bool> cubesExistence = m_building->GetCubesExistence();

	for (const auto&cubeExistence : cubesExistence)
	{
		if (!cubeExistence)
			numberOfRemovedElements++;
	}

	return numberOfRemovedElements;
}

int Individual::GetNumberOfRemovedElementsWithoutInitialGene() const
{
	int count = GetNumberOfRemovedElements();
	for (size_t index = 0; index < m_initialGenes.size(); ++index)
	{
		if (!m_initialGenes[index]) --count;
	}
	return count;
}

double Individual::SimulateAndGetMaximStress()
{
	auto clone = CreateBuildingFromDetails(m_sizeOx, m_sizeOy, m_sizeOz, m_elementSize, m_building->GetCubesExistence());
	
	ConfigureSystem configureSystem(clone->GetSystem());
	configureSystem.SetSystemTimestepper();
	configureSystem.SetSystemSover();
	configureSystem.Simulate(0.1);

	double maximStress = 0.0;
	auto elements = clone->GetMesh()->GetElements();

	for (const auto& element : elements)
	{
		auto castedElement = std::dynamic_pointer_cast<chrono::fea::ChElementHexaCorot_8>(element);
		auto stress = castedElement->GetStress(0.5, 0.5, 0.5);

		double stressOnOx, stressOnOy, stressOnOz;

		stress.ComputePrincipalStresses(stressOnOx, stressOnOy, stressOnOz);

		if (fabs(stressOnOx) > maximStress)
			maximStress = fabs(stressOnOx);

		if (fabs(stressOnOy) > maximStress)
			maximStress = fabs(stressOnOy);

		if (fabs(stressOnOz) > maximStress)
			maximStress = fabs(stressOnOz);
	}

	return maximStress;
}

bool Individual::IsOnTopLayer(size_t possition) const
{
	uint16_t currentOyLayer = possition / (m_building->GetCubesExistence().size() / m_sizeOy);
	double currentOyCoord = currentOyLayer * m_elementSize - m_elementSize;
	double maximOyCoord = m_sizeOy * m_elementSize - 2 * m_elementSize;

	if (fabs(currentOyCoord - maximOyCoord) > EPSILON)
		return false;

	return true;
}

std::ostream& operator<<(std::ostream& out, const Individual& individual)
{
	out << std::endl;
	out << individual.m_sizeOx << std::endl;
	out << individual.m_sizeOy << std::endl;
	out << individual.m_sizeOz << std::endl;
	out << individual.m_elementSize << std::endl;

	size_t size = individual.m_building->GetCubesExistence().size();
	for (int index = 0; index < size; ++index)
		if (index != size - 1)
			out << individual.m_building->GetCubesExistence()[index] << " ";
	out << individual.m_building->GetCubesExistence()[size - 1];

	return out;
}
