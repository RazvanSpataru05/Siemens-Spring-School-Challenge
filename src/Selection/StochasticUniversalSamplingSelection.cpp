#include <Selection/StochasticUniversalSamplingSelection.h>

void StochasticUniversalSamplingSelection::Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double>& fitnessValues)
{
	std::vector<double> cumulativeProbabilities = 
		GeneticAlgorithmService::CalcutateCumulativeProbabilityOfSelection(
		workingPopulation, fitnessValues);
	size_t populationSize = workingPopulation.size();
	double step = 1.0 / static_cast<double>(populationSize);
	double randomNumber = RandomNumbersGenerator::GenerateRealNumberInRange(0, step);

	std::vector<std::shared_ptr<IIndividual>> newPopulation;
	std::vector<double> pointers;
	newPopulation.reserve(workingPopulation.size());
	pointers.reserve(workingPopulation.size());

	for (size_t index = 0; index < populationSize; ++index)
	{
		pointers.emplace_back(randomNumber + index * step);
	}

	size_t individualIndex = 0;
	for (const auto& pointer : pointers)
	{
		while (cumulativeProbabilities[individualIndex] < pointer)
			++individualIndex;
		newPopulation.push_back(workingPopulation[individualIndex]);
	}
	workingPopulation = std::move(newPopulation);
}
