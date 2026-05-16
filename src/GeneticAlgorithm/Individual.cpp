#include <GeneticAlgorithm/Individual.h>

Individual::Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize) :
	m_sizeOx{ sizeOx }, m_sizeOy{ sizeOy }, m_sizeOz{ sizeOz }, m_elementSize{ elementSize }
{
	m_building = std::make_shared<Building>(m_sizeOx, m_sizeOy, m_sizeOz, m_elementSize);
	m_building->Build();
	m_building->AddConstraints();

	m_initialGenes = std::vector<bool>(m_building->GetCubesExistence().size(), true);
}

Individual::Individual(int sizeOx, int sizeOy, int sizeOz, double elementSize, const std::vector<bool>& cubesExistence) :
	m_sizeOx{ sizeOx }, m_sizeOy{ sizeOy }, m_sizeOz{ sizeOz }, m_elementSize{ elementSize }
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
	}
	return *this;
}

void Individual::SetMaximStress(double maximStress)
{
	m_maximStress = maximStress;
}

const std::shared_ptr<Building> Individual::GetBuilding() const
{
	return m_building;
}

double Individual::Evaluate()
{
	double maximStress = SimulateAndGetMaximStress();
	double value = MINIM_INDIVIDUAL_VALUE;

	if (maximStress >= m_maximStress || maximStress < EPSILON_STRESS)
	{
		return value;
	}

	double stressHeadroom = m_maximStress - maximStress;

	value = (pow((GetNumberOfRemovedElements() + 1), 2)) * stressHeadroom;

	return value;
}

void Individual::Crossover(IIndividual& other)
{
	size_t numberOfGenes = m_building->GetCubesExistence().size();

	int randomNumber = RandomNumbersGenerator::GenerateIntegerNumberInRange(1, numberOfGenes - 1);

	Individual& otherIndividual = dynamic_cast<Individual&> (other);

	std::vector<bool> newCubesExistence = m_building->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = otherIndividual.m_building->GetCubesExistence();

	for (size_t index = randomNumber; index < numberOfGenes; ++index)
	{
		newCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
		newOtherCubesExistence[index] = m_building->GetCubesExistence()[index];
	}

	m_building->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	m_building->AddCubesBasedOnCubesExistence(newCubesExistence);

	otherIndividual.m_building->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	otherIndividual.m_building->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
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

int Individual::GetNumberOfRemovedElements()
{
	int numberOfRemovedElements = 0;
	std::vector<bool> cubesExistence = m_building->GetCubesExistence();

	for (const auto cubeExistence : cubesExistence)
		if (!cubeExistence)
			numberOfRemovedElements++;

	return numberOfRemovedElements;
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

bool Individual::IsOnTopLayer(size_t possition)
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
