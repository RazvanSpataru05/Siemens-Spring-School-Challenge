#include <Services/GeneticAlgorithmService.h>

void GeneticAlgorithmService::RunWithConfigValues()
{
	GeneticAlgorithm geneticAlgorithm(
		UtilFunctions::CreateIndividualFunction(
			AlgorithmSettings::GetInstance()->GetOxSize(),
			AlgorithmSettings::GetInstance()->GetOySize(),
			AlgorithmSettings::GetInstance()->GetOzSize(),
			AlgorithmSettings::GetInstance()->GetElementSize(),
			AlgorithmSettings::GetInstance()->GetMaximStressAllowed()),
		AlgorithmSettings::GetInstance()->GetNumberOfIndividuals(),
		AlgorithmSettings::GetInstance()->GetNumberOfEpochs(),
		AlgorithmSettings::GetInstance()->GetCrossoverProbability(),
		AlgorithmSettings::GetInstance()->GetMutationProbability());

	geneticAlgorithm.Run();
}

void GeneticAlgorithmService::RunWithCustomFirstIndividual()
{
	int oxSize = AlgorithmSettings::GetInstance()->GetOxSize();
	int oySize = AlgorithmSettings::GetInstance()->GetOySize();
	int ozSize = AlgorithmSettings::GetInstance()->GetOzSize();

	auto cubesExistence = IOIndividualManager::ReadInitialIndividual(oxSize * oySize * ozSize);

	GeneticAlgorithm geneticAlgorithm(
		UtilFunctions::CreateIndividualFromFileFunction(
			oxSize,
			oySize,
			ozSize,
			AlgorithmSettings::GetInstance()->GetElementSize(),
			cubesExistence,
			AlgorithmSettings::GetInstance()->GetMaximStressAllowed()),
		AlgorithmSettings::GetInstance()->GetNumberOfIndividuals(),
		AlgorithmSettings::GetInstance()->GetNumberOfEpochs(),
		AlgorithmSettings::GetInstance()->GetCrossoverProbability(),
		AlgorithmSettings::GetInstance()->GetMutationProbability());

	geneticAlgorithm.Run();
}

void GeneticAlgorithmService::ShowResults()
{
	IOIndividualManager::ReadIndividualsDetailsAndCreateBuildings();
}
