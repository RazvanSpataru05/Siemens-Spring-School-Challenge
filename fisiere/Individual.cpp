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
		m_initialGenes = another.m_initialGenes;
		m_building = CreateBuildingFromDetails(
			another.m_sizeOx, another.m_sizeOy, another.m_sizeOz,
			another.m_elementSize,
			another.m_building->GetCubesExistence());
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
		m_initialGenes = std::move(another.m_initialGenes);
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

const std::vector<bool> Individual::GetInitialGenes() const
{
	return m_initialGenes;
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
	// switch strategies here by uncommenting the one you want to use

	 //Crossover3DBlock(other);
	 Crossover2PointPlanar(other);
	//CrossoverSymmetryForced(other);
}

void Individual::Crossover3DBlock(IIndividual& other)
{
	size_t numberOfGenes = m_building->GetCubesExistence().size();
	Individual& otherIndividual = dynamic_cast<Individual&>(other);

	std::vector<bool> newCubesExistence = m_building->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = otherIndividual.m_building->GetCubesExistence();

	// random 3D bounding box
	int minX = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOx > 0 ? m_sizeOx - 1 : 0);
	int maxX = RandomNumbersGenerator::GenerateIntegerNumberInRange(minX, m_sizeOx > 0 ? m_sizeOx - 1 : 0);
	
	int minY = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOy > 0 ? m_sizeOy - 1 : 0);
	int maxY = RandomNumbersGenerator::GenerateIntegerNumberInRange(minY, m_sizeOy > 0 ? m_sizeOy - 1 : 0);
	
	int minZ = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOz > 0 ? m_sizeOz - 1 : 0);
	int maxZ = RandomNumbersGenerator::GenerateIntegerNumberInRange(minZ, m_sizeOz > 0 ? m_sizeOz - 1 : 0);

	for (size_t index = 0; index < numberOfGenes; ++index)
	{
		if (IsOnTopLayer(index))
		{
			newCubesExistence[index] = true;
			newOtherCubesExistence[index] = true;
			continue;
		}

		int y = index / (m_sizeOx * m_sizeOz);
		int remainder = index % (m_sizeOx * m_sizeOz);
		int z = remainder / m_sizeOx;
		int x = remainder % m_sizeOx;

		bool inBox = (x >= minX && x <= maxX) && (y >= minY && y <= maxY) && (z >= minZ && z <= maxZ);

		if (inBox)
		{
			newCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
			newOtherCubesExistence[index] = m_building->GetCubesExistence()[index];
		}
		else
		{
			newCubesExistence[index] = m_building->GetCubesExistence()[index];
			newOtherCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
		}
	}

	m_building->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	m_building->AddCubesBasedOnCubesExistence(newCubesExistence);

	otherIndividual.m_building->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	otherIndividual.m_building->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
}

void Individual::Crossover2PointPlanar(IIndividual& other)
{
	size_t numberOfGenes = m_building->GetCubesExistence().size();
	Individual& otherIndividual = dynamic_cast<Individual&>(other);

	std::vector<bool> newCubesExistence = m_building->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = otherIndividual.m_building->GetCubesExistence();

	int axis = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, 2);
	int cut1 = 0, cut2 = 0;

	if (axis == 0 && m_sizeOx > 2) {
		cut1 = RandomNumbersGenerator::GenerateIntegerNumberInRange(1, m_sizeOx - 2);
		cut2 = RandomNumbersGenerator::GenerateIntegerNumberInRange(cut1 + 1, m_sizeOx - 1);
	} else if (axis == 1 && m_sizeOy > 2) {
		cut1 = RandomNumbersGenerator::GenerateIntegerNumberInRange(1, m_sizeOy - 2);
		cut2 = RandomNumbersGenerator::GenerateIntegerNumberInRange(cut1 + 1, m_sizeOy - 1);
	} else if (m_sizeOz > 2) {
		cut1 = RandomNumbersGenerator::GenerateIntegerNumberInRange(1, m_sizeOz - 2);
		cut2 = RandomNumbersGenerator::GenerateIntegerNumberInRange(cut1 + 1, m_sizeOz - 1);
	}

	for (size_t index = 0; index < numberOfGenes; ++index)
	{
		if (IsOnTopLayer(index))
		{
			newCubesExistence[index] = true;
			newOtherCubesExistence[index] = true;
			continue;
		}

		int y = index / (m_sizeOx * m_sizeOz);
		int remainder = index % (m_sizeOx * m_sizeOz);
		int z = remainder / m_sizeOx;
		int x = remainder % m_sizeOx;

		bool swapGenes = false;
		if (axis == 0 && x >= cut1 && x < cut2) swapGenes = true;
		else if (axis == 1 && y >= cut1 && y < cut2) swapGenes = true;
		else if (axis == 2 && z >= cut1 && z < cut2) swapGenes = true;

		if (swapGenes)
		{
			newCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
			newOtherCubesExistence[index] = m_building->GetCubesExistence()[index];
		}
		else
		{
			newCubesExistence[index] = m_building->GetCubesExistence()[index];
			newOtherCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
		}
	}

	m_building->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	m_building->AddCubesBasedOnCubesExistence(newCubesExistence);

	otherIndividual.m_building->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	otherIndividual.m_building->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
}

void Individual::CrossoverSymmetryForced(IIndividual& other)
{
	size_t numberOfGenes = m_building->GetCubesExistence().size();
	Individual& otherIndividual = dynamic_cast<Individual&>(other);

	std::vector<bool> newCubesExistence = m_building->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = otherIndividual.m_building->GetCubesExistence();

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, numberOfGenes);
	double uniformRate = 0.5;

	for (size_t index = 0; index < numberOfGenes; ++index)
	{
		if (IsOnTopLayer(index))
		{
			newCubesExistence[index] = true;
			newOtherCubesExistence[index] = true;
			continue;
		}

		int y = index / (m_sizeOx * m_sizeOz);
		int remainder = index % (m_sizeOx * m_sizeOz);
		int z = remainder / m_sizeOx;
		int x = remainder % m_sizeOx;

		// only randomly swap the left half. right half will be forced to mirror it.
		if (x < (m_sizeOx + 1) / 2) 
		{
			if (randomNumbers[index] < uniformRate)
			{
				newCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
				newOtherCubesExistence[index] = m_building->GetCubesExistence()[index];
			}
			else
			{
				newCubesExistence[index] = m_building->GetCubesExistence()[index];
				newOtherCubesExistence[index] = otherIndividual.m_building->GetCubesExistence()[index];
			}

			// symmetry to right half
			int mirrorX = m_sizeOx - 1 - x;
			int mirrorIndex = y * (m_sizeOx * m_sizeOz) + z * m_sizeOx + mirrorX;
			
			// protect the right half top layer too just in case
			if (!IsOnTopLayer(mirrorIndex)) {
				newCubesExistence[mirrorIndex] = newCubesExistence[index];
				newOtherCubesExistence[mirrorIndex] = newOtherCubesExistence[index];
			}
		}
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
