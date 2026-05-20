#include <Crossover/Planar2PointCrossover.h>
#include <GeneticAlgorithm/Individual.h>
#include <Services/RandomNumbersGenerator.h>

void Planar2PointCrossover::Crossover(IIndividual& first, IIndividual& second)
{
	Individual& firstIndividual = dynamic_cast<Individual&>(first);
	Individual& secondIndividual = dynamic_cast<Individual&>(second);

	size_t numberOfGenes = firstIndividual.GetBuilding()->GetCubesExistence().size();
	
	int m_sizeOx = firstIndividual.GetSizeOx();
	int m_sizeOy = firstIndividual.GetSizeOy();
	int m_sizeOz = firstIndividual.GetSizeOz();

	std::vector<bool> newCubesExistence = firstIndividual.GetBuilding()->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = secondIndividual.GetBuilding()->GetCubesExistence();

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

		bool swapGenes = false;
		if (axis == 0 && x >= cut1 && x < cut2) swapGenes = true;
		else if (axis == 1 && y >= cut1 && y < cut2) swapGenes = true;
		else if (axis == 2 && z >= cut1 && z < cut2) swapGenes = true;

		if (swapGenes)
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
