#include <Selection/RouletteWheelSelection.h>

RouletteWheelSelection::RouletteWheelSelection()
{
}

void RouletteWheelSelection::Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double>& fitnessValues)
{
	std::vector<std::shared_ptr<IIndividual>> newPopulation;

	std::vector<double> cumulativeProbabilities = GeneticAlgorithmService::CalcutateCumulativeProbabilityOfSelection(workingPopulation,
		fitnessValues);
	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, workingPopulation.size());

	for (const auto& randomNumber : randomNumbers)
	{
		for (size_t index = 0; index < workingPopulation.size(); ++index)
		{
			if (cumulativeProbabilities[index] >= randomNumber)
			{
				newPopulation.push_back(workingPopulation[index]);
				break;
			}
		}
	}
	workingPopulation = newPopulation;
}

RouletteWheelSelection::~RouletteWheelSelection()
{
}
