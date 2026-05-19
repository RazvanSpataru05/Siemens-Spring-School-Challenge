#include <Services/GeneticAlgorithmService.h>

void GeneticAlgorithmService::RunWithConfigValues(std::unique_ptr<SelectionStrategy> selectionStrategy)
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
		AlgorithmSettings::GetInstance()->GetMutationProbability(),
		std::move(selectionStrategy));

	geneticAlgorithm.Run();
}

void GeneticAlgorithmService::RunWithCustomFirstIndividual(std::unique_ptr<SelectionStrategy> selectionStrategy)
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
		AlgorithmSettings::GetInstance()->GetMutationProbability(),
		std::move(selectionStrategy));

	geneticAlgorithm.Run();
}

void GeneticAlgorithmService::ShowResults()
{
	IOIndividualManager::ReadIndividualsDetailsAndCreateBuildings();
}

double GeneticAlgorithmService::CalculateSumOfFitnessValues(const std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double> fitnessValues)
{
	double sum{};
	for (const auto& individual : workingPopulation)
	{
		sum += fitnessValues.at(individual.get());
	}
	return sum;
}

std::vector<double> GeneticAlgorithmService::CalculateProbabilityOfSelection(const std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double> fitnessValues)
{
	std::vector<double> probabilityOfSelectionVector;
	double sum = CalculateSumOfFitnessValues(workingPopulation, fitnessValues);

	for (const auto& individual : workingPopulation)
	{
		probabilityOfSelectionVector.emplace_back(fitnessValues.at(individual.get()) / sum);
	}
	return probabilityOfSelectionVector;
}

std::vector<double> GeneticAlgorithmService::CalcutateCumulativeProbabilityOfSelection(
	const std::vector<std::shared_ptr<IIndividual>>& workingPopulation, const std::map<IIndividual*, double> fitnessValues)
{
	std::vector<double> cumulativeProbabilityOfSelectionVector;
	std::vector<double> probabilityOfSelectionVector = CalculateProbabilityOfSelection(workingPopulation, fitnessValues);

	for (int currentIndividualIndex = 0; currentIndividualIndex < workingPopulation.size(); ++currentIndividualIndex)
	{
		double probability{};

		for (int index = 0; index <= currentIndividualIndex; ++index)
		{
			probability += probabilityOfSelectionVector[index];
		}

		cumulativeProbabilityOfSelectionVector.emplace_back(probability);
	}

	return cumulativeProbabilityOfSelectionVector;
}
