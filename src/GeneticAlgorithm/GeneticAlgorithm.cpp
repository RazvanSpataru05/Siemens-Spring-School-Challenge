#include "GeneticAlgorithm/GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(
	std::function<IIndividual* ()> createIndividual,
	size_t populationSize, size_t numberOfEpochs,
	double crossoverProbabillity, double mutationProbability,
	std::unique_ptr<ISelectionStrategy> selectionStrategy,
	std::unique_ptr<ICrossoverStrategy> crossoverStrategy) :
	m_createIndividual{ createIndividual },
	m_populationSize{ populationSize },
	m_numberOfEpochs{ numberOfEpochs },
	m_crossoverProbability{ crossoverProbabillity },
	m_mutationProbability{ mutationProbability },
	m_selectionStrategy{ std::move(selectionStrategy) },
	m_crossoverStrategy{ std::move(crossoverStrategy) }
{
}

GeneticAlgorithm::GeneticAlgorithm(
	std::function<IIndividual* ()> createIndividual,
	const GAConfig& config,
	std::unique_ptr<ISelectionStrategy> selectionStrategy) :
	m_createIndividual{ createIndividual },
	m_populationSize{ config.population },
	m_numberOfEpochs{ config.epochs },
	m_crossoverProbability{ config.crossoverProbability },
	m_mutationProbability{ config.mutationProbability },
	m_selectionStrategy{ std::move(selectionStrategy) }
{
	switch (config.crossoverMethod)
	{
	case 0: m_crossoverStrategy = std::make_unique<SinglePointCrossover>(); break;
	case 1: m_crossoverStrategy = std::make_unique<Block3DCrossover>(); break;
	case 2: m_crossoverStrategy = std::make_unique<Planar2PointCrossover>(); break;
	case 3: m_crossoverStrategy = std::make_unique<SymmetryForcedCrossover>(); break;
	}
}

void GeneticAlgorithm::Run()
{
	InitializePopulation();

	for (size_t index = 0; index < m_numberOfEpochs; ++index)
	{
		std::cout << std::endl << "Epoch: " << index + 1 << "/" << m_numberOfEpochs << std::endl;

		CalculateFitnessValues();

		m_selectionStrategy->Select(m_workingPopulation, m_fitnessValues);
		Crossover();
		Mutation();

		WriteWinners(static_cast<int>(index));
		IIndividual* bestIndividual = GetWinnerIndividual();
		m_bestRemovedPerEpoch.emplace_back(dynamic_cast<Individual&>(*bestIndividual).GetNumberOfRemovedElementsWithoutInitialGene());
		m_bestStressPerEpoch.emplace_back(dynamic_cast<Individual&>(*bestIndividual).GetCurrentMaxStress());
		m_bestFitnessPerEpoch.emplace_back(m_fitnessValues[bestIndividual]);
		m_bestEpochIndividual.emplace_back(dynamic_cast<Individual&>(*bestIndividual).GetBuilding()->GetCubesExistence());
	}
}

size_t GeneticAlgorithm::GetBestFitnessEpochIndex() const
{
	if (m_bestFitnessPerEpoch.empty()) return 0;

	size_t bestIndex{};
	double bestFitness{};

	for (size_t index = 0; index < m_bestFitnessPerEpoch.size(); ++index)
	{
		if (m_bestFitnessPerEpoch[index] > bestFitness)
		{
			bestFitness = m_bestFitnessPerEpoch[index];
			bestIndex = index;
		}
	}
	return bestIndex;
}

IIndividual* GeneticAlgorithm::GetWinnerIndividual()
{
	double maxValue{};
	IIndividual* winner = nullptr;

	for (const auto& value : m_fitnessValues)
	{
		if (value.second > maxValue)
		{
			maxValue = value.second;
			winner = value.first;
		}
	}
	return winner;
}

const std::vector<std::vector<bool>>& GeneticAlgorithm::GetBestEpochIndividual() const
{
	return m_bestEpochIndividual;
}

const std::vector<int>& GeneticAlgorithm::GetBestRemovedPerEpoch() const
{
	return m_bestRemovedPerEpoch;
}

const std::vector<double>& GeneticAlgorithm::GetBestStressPerEpoch() const
{
	return m_bestStressPerEpoch;
}

const std::vector<double>& GeneticAlgorithm::GetBestFitnessPerEpoch() const
{
	return m_bestFitnessPerEpoch;
}

void GeneticAlgorithm::InitializePopulation()
{
	for (size_t index = 0; index < m_populationSize; ++index)
	{
		std::cout << "Created individual " << index + 1 << "\n";

		m_population.emplace_back(std::move(std::shared_ptr<IIndividual>(m_createIndividual())));
		m_workingPopulation.emplace_back(m_population[index]);
	}
}

void GeneticAlgorithm::CalculateFitnessValues()
{
	std::vector<std::shared_ptr<IIndividual>> distinctIndividuals;
	std::unordered_set<IIndividual*> alreadyQueued;
	distinctIndividuals.reserve(m_workingPopulation.size());

	for (const auto& individual : m_workingPopulation)
	{
		if (alreadyQueued.insert(individual.get()).second)
		{
			distinctIndividuals.emplace_back(individual);
		}
	}

	size_t batchSize = std::thread::hardware_concurrency();
	if (batchSize == 0) batchSize = 4;

	for (size_t begin = 0; begin < distinctIndividuals.size(); begin += batchSize)
	{
		const size_t end = std::min(begin + batchSize, distinctIndividuals.size());

		std::vector<std::future<double>> futures;
		futures.reserve(end - begin);

		for (size_t index = begin; index < end; ++index)
		{
			const auto& individual = distinctIndividuals[index];
			futures.push_back(std::async(std::launch::async,
				[&individual]() {return individual->Evaluate(); }));
		}

		for (size_t index = begin; index < end; ++index)
		{
			m_fitnessValues[distinctIndividuals[index].get()] = futures[index - begin].get();
		}
	}
}

void GeneticAlgorithm::Crossover()
{
	std::vector<std::shared_ptr<IIndividual>> selectedPopulationForCrossover;
	std::vector<std::shared_ptr<IIndividual>> newPopulation;

	const size_t populationSize = m_workingPopulation.size();

	std::vector<double> randomNumbers = RandomNumbersGenerator::GenerateRealNumbers(LOWER_BOUND, UPPER_BOUND, populationSize);

	for (size_t index = 0; index < populationSize; ++index)
	{
		if (randomNumbers[index] < m_crossoverProbability)
		{
			selectedPopulationForCrossover.emplace_back(m_workingPopulation[index]);
		}
	}

	if (selectedPopulationForCrossover.size() % 2 != 0)
	{
		selectedPopulationForCrossover.pop_back();
	}

	for (size_t index = 0; index < selectedPopulationForCrossover.size(); index += 2)
	{
		m_crossoverStrategy->Crossover(*selectedPopulationForCrossover[index],
			*selectedPopulationForCrossover[index + 1]);
	}
}

void GeneticAlgorithm::Mutation()
{
	for (auto& individual : m_workingPopulation)
	{
		individual->Mutation(m_mutationProbability);
	}
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
