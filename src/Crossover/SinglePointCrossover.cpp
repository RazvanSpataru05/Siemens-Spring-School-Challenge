#include <Crossover/SinglePointCrossover.h>
#include <GeneticAlgorithm/Individual.h>
#include <Services/RandomNumbersGenerator.h>

void SinglePointCrossover::Crossover(IIndividual& first, IIndividual& second)
{
	Individual& firstIndividual = dynamic_cast<Individual&>(first);
	Individual& secondIndividual = dynamic_cast<Individual&>(second);

	size_t numberOfGenes = firstIndividual.GetBuilding()->GetCubesExistence().size();

	int randomNumber = RandomNumbersGenerator::GenerateIntegerNumberInRange(1, numberOfGenes - 1);

	std::vector<bool> newCubesExistence = firstIndividual.GetBuilding()->GetCubesExistence();
	std::vector<bool> newOtherCubesExistence = secondIndividual.GetBuilding()->GetCubesExistence();

	for (size_t index = randomNumber; index < numberOfGenes; ++index)
	{
		newCubesExistence[index] = secondIndividual.GetBuilding()->GetCubesExistence()[index];
		newOtherCubesExistence[index] = firstIndividual.GetBuilding()->GetCubesExistence()[index];
	}

	firstIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newCubesExistence);
	firstIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newCubesExistence);

	secondIndividual.GetBuilding()->EliminateCubesBasedOnCubesExistence(newOtherCubesExistence);
	secondIndividual.GetBuilding()->AddCubesBasedOnCubesExistence(newOtherCubesExistence);
}
