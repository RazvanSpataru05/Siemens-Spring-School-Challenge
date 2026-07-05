#include <Selection/RankedSelection.h>

void RankedSelection::Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double>& fitnessValues)
{
	std::vector<std::shared_ptr<IIndividual>> newPopulation;
	newPopulation.reserve(workingPopulation.size());
	std::sort(workingPopulation.begin(), workingPopulation.end(),
		[&fitnessValues](const auto& a, const auto& b) {
			return fitnessValues.at(a.get()) < fitnessValues.at(b.get());
		});

	size_t populationSize = workingPopulation.size();
	double rankSum = populationSize * (populationSize + 1) / 2;
	std::vector<double> cumulativeProbabilities;
	cumulativeProbabilities.reserve(workingPopulation.size());
	double probabilitySum{};

	for (size_t rank = 1; rank <= workingPopulation.size(); ++rank)
	{
		probabilitySum += rank / rankSum;
		cumulativeProbabilities.emplace_back(probabilitySum);
	}

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(0, 1, populationSize);
	std::sort(randomNumbers.begin(), randomNumbers.end());
	size_t individualIndex = 0;
	for (const auto& randomNumber : randomNumbers)
	{
		while (cumulativeProbabilities[individualIndex] < randomNumber)
			++individualIndex;
		newPopulation.push_back(workingPopulation[individualIndex]);
	}
	workingPopulation = std::move(newPopulation);
}
