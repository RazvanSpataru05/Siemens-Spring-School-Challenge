#include <Crossover/SymmetryForcedCrossover.h>
#include <GeneticAlgorithm/Individual.h>
#include <Services/RandomNumbersGenerator.h>

void SymmetryForcedCrossover::Crossover(IIndividual& first, IIndividual& second)
{
	Individual& firstIndividual = dynamic_cast<Individual&>(first);
	Individual& secondIndividual = dynamic_cast<Individual&>(second);

	size_t numberOfGenes = firstIndividual.GetBuilding()->GetCubesExistence().size();
	
	int m_sizeOx = firstIndividual.GetSizeOx();
	int m_sizeOy = firstIndividual.GetSizeOy();
	int m_sizeOz = firstIndividual.GetSizeOz();

	std::vector<bool> newCubesExistence = firstIndividual.GetBuilding()->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = secondIndividual.GetBuilding()->GetCubesExistence();

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, numberOfGenes);
	double uniformRate = 0.5;

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

		// only randomly swap the left half. right half will be forced to mirror it.
		if (x < (m_sizeOx + 1) / 2) 
		{
			if (randomNumbers[index] < uniformRate)
			{
				newCubesExistence[index] = secondIndividual.GetBuilding()->GetCubesExistence()[index];
				newOtherCubesExistence[index] = firstIndividual.GetBuilding()->GetCubesExistence()[index];
			}
			else
			{
				newCubesExistence[index] = firstIndividual.GetBuilding()->GetCubesExistence()[index];
				newOtherCubesExistence[index] = secondIndividual.GetBuilding()->GetCubesExistence()[index];
			}

			// symmetry to right half
			int mirrorX = m_sizeOx - 1 - x;
			int mirrorIndex = y * (m_sizeOx * m_sizeOz) + z * m_sizeOx + mirrorX;
			
			// protect the right half top layer too just in case
			if (!firstIndividual.IsOnTopLayer(mirrorIndex)) {
				newCubesExistence[mirrorIndex] = newCubesExistence[index];
				newOtherCubesExistence[mirrorIndex] = newOtherCubesExistence[index];
			}
		}
	}

	firstIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	firstIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newCubesExistence);

	secondIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	secondIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
}
