#include <GeneticAlgorithm/GeneticAlgorithm.h>

GeneticAlgorithm::GeneticAlgorithm(
	std::function<IIndividual* ()> createIndividual,
	size_t populationSize, size_t numberOfEpochs,
	double crossoverProbabillity, double mutationProbability) :
	m_createIndividual{ createIndividual },
	m_populationSize{ populationSize },
	m_numberOfEpochs{ numberOfEpochs },
	m_crossoverProbability{ crossoverProbabillity },
	m_mutationProbability{ mutationProbability }
{
}

void GeneticAlgorithm::Run()
{
	InitializePopulation();

	for (int index = 0; index < m_numberOfEpochs; ++index)
	{
		std::cout << std::endl << "Epoch: " << index + 1 << std::endl;

		m_fitnessValues = CalculateFitnessValues();

		Selection();
		Crossover();
		Mutation();

		WriteWinners(index);
	}
}

IIndividual* GeneticAlgorithm::GetWinnerIndividual()
{
	double maxValue = 0.0;
	IIndividual* winner = nullptr;

	for (const auto value : m_fitnessValues)
		if (value.second > maxValue)
		{
			maxValue = value.second;
			winner = value.first;
		}

	return winner;
}

void GeneticAlgorithm::InitializePopulation()
{
	for (int index = 0; index < m_populationSize; ++index)
	{
		std::cout << "Created individual " << index + 1 << "\n";

		m_population.push_back(std::move(std::shared_ptr<IIndividual>(m_createIndividual())));
		m_workingPopulation.push_back(m_population[index]);
	}
}

std::map<IIndividual*, double> GeneticAlgorithm::CalculateFitnessValues()
{
	std::map<IIndividual*, double> fitnessValues;
	for (const auto& individual : m_workingPopulation)
	{
		double value = individual->Evaluate();
		fitnessValues[individual.get()] = value;
	}

	return fitnessValues;
}

double GeneticAlgorithm::CalculateSumOfFitnessValues()
{
	double sum{};
	for (const auto& individual : m_workingPopulation)
	{
		sum += m_fitnessValues[individual.get()];
	}

	return sum;
}

std::vector<double> GeneticAlgorithm::CalculateProbabilityOfSelection()
{
	std::vector<double> probabilityOfSelectionVector;
	double sum = CalculateSumOfFitnessValues();

	for (const auto& individual : m_workingPopulation)
	{
		probabilityOfSelectionVector.emplace_back(m_fitnessValues[individual.get()] / sum);
	}

	return probabilityOfSelectionVector;
}

std::vector<double> GeneticAlgorithm::CalcutateCumulativeProbabilityOfSelection()
{
	std::vector<double> cumulativeProbabilityOfSelectionVector;
	std::vector<double> probabilityOfSelectionVector = CalculateProbabilityOfSelection();

	for (int currentIndividualIndex = 0; currentIndividualIndex < m_populationSize; ++currentIndividualIndex)
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

void GeneticAlgorithm::Selection()
{
	std::vector<std::shared_ptr<IIndividual>> newPopulation;

	std::vector<double> cumulativeProbabilityOfSelectionVector = CalcutateCumulativeProbabilityOfSelection();
	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, m_populationSize);

	for (const auto& randomNumber : randomNumbers)
	{
		if (IsGraterThan(randomNumber, LOWER_BOUND) &&
			IsLessThanOrEqualTo(randomNumber, cumulativeProbabilityOfSelectionVector[0]))
		{
			newPopulation.push_back(m_workingPopulation[0]);
			continue;
		}

		for (size_t probabilityIndex = 0; probabilityIndex < cumulativeProbabilityOfSelectionVector.size() - 1; ++probabilityIndex)
		{
			if (IsGraterThan(randomNumber, cumulativeProbabilityOfSelectionVector[probabilityIndex]) &&
				IsLessThanOrEqualTo(randomNumber, cumulativeProbabilityOfSelectionVector[probabilityIndex + 1]))
			{
				newPopulation.push_back(m_workingPopulation[probabilityIndex + 1]);
				break;
			}
		}
	}

	m_workingPopulation = newPopulation;
}

void GeneticAlgorithm::Crossover()
{
	std::vector<std::shared_ptr<IIndividual>> selectedPopulationForCrossover;
	std::vector<std::shared_ptr<IIndividual>> newPopulation;

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, m_populationSize);

	for (size_t index = 0; index < m_populationSize; ++index)
	{
		if (randomNumbers[index] < m_crossoverProbability)
		{
			selectedPopulationForCrossover.push_back(m_workingPopulation[index]);
		}
	}

	if (selectedPopulationForCrossover.size() % 2 != 0)
	{
		selectedPopulationForCrossover.pop_back();
	}

	for (size_t index = 0; index < selectedPopulationForCrossover.size(); index += 2)
	{
		selectedPopulationForCrossover[index]->Crossover(*selectedPopulationForCrossover[index + 1]);
	}
}

void GeneticAlgorithm::Mutation()
{
	for (auto& individual : m_workingPopulation)
	{
		individual->Mutation(m_mutationProbability);
	}
}

bool GeneticAlgorithm::IsGraterThan(double value, double lowerBound) const
{
	return value > lowerBound;
}

bool GeneticAlgorithm::IsLessThanOrEqualTo(double value, double upperBound) const
{
	return value <= upperBound;
}

void GeneticAlgorithm::WriteWinners(int epoch)
{
	IIndividual* winner = GetWinnerIndividual();

	if (epoch == 0)
	{
		IOIndividualManager::WriteIndividualValueInFile(epoch + 1, m_fitnessValues[winner], false);
	}
	else
	{
		if (epoch == m_numberOfEpochs - 1)
		{
			IOIndividualManager::WriteIndividualDetailsInFile(winner);
		}
		IOIndividualManager::WriteIndividualValueInFile(epoch + 1, m_fitnessValues[winner], true);
	}
}
