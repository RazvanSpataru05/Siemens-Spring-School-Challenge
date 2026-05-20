#include <Crossover/Block3DCrossover.h>
#include <GeneticAlgorithm/Individual.h>
#include <Services/RandomNumbersGenerator.h>

void Block3DCrossover::Crossover(IIndividual& first, IIndividual& second)
{
	Individual& firstIndividual = dynamic_cast<Individual&>(first);
	Individual& secondIndividual = dynamic_cast<Individual&>(second);

	size_t numberOfGenes = firstIndividual.GetBuilding()->GetCubesExistence().size();
	
	int m_sizeOx = firstIndividual.GetSizeOx();
	int m_sizeOy = firstIndividual.GetSizeOy();
	int m_sizeOz = firstIndividual.GetSizeOz();

	std::vector<bool> newCubesExistence = firstIndividual.GetBuilding()->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = secondIndividual.GetBuilding()->GetCubesExistence();

	// random 3D bounding box
	int minX = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOx > 0 ? m_sizeOx - 1 : 0);
	int maxX = RandomNumbersGenerator::GenerateIntegerNumberInRange(minX, m_sizeOx > 0 ? m_sizeOx - 1 : 0);
	
	int minY = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOy > 0 ? m_sizeOy - 1 : 0);
	int maxY = RandomNumbersGenerator::GenerateIntegerNumberInRange(minY, m_sizeOy > 0 ? m_sizeOy - 1 : 0);
	
	int minZ = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, m_sizeOz > 0 ? m_sizeOz - 1 : 0);
	int maxZ = RandomNumbersGenerator::GenerateIntegerNumberInRange(minZ, m_sizeOz > 0 ? m_sizeOz - 1 : 0);

	for (size_t index = 0; index < numberOfGenes; ++index)
	{
		if (firstIndividual.IsOnTopLayer(index))
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
			newCubesExistence[index] = secondIndividual.GetBuilding()->GetCubesExistence()[index];
			newOtherCubesExistence[index] = firstIndividual.GetBuilding()->GetCubesExistence()[index];
		}
		else
		{
			newCubesExistence[index] = firstIndividual.GetBuilding()->GetCubesExistence()[index];
			newOtherCubesExistence[index] = secondIndividual.GetBuilding()->GetCubesExistence()[index];
		}
	}

	firstIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	firstIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newCubesExistence);

	secondIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	secondIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
}
