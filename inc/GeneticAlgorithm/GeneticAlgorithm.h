#pragma once

#include <future>
#include <functional>
#include <iostream>
#include <map>

#include <GeneticAlgorithm/IIndividual.h>

#include <Services/SelectionStrategy.h>
#include <Services/CrossoverStrategy.h>
#include <Services/RandomNumbersGenerator.h>
#include <Services/IOIndividualManager.h>
#include <Services/GeneticAlgorithmService.h>
#include <Services/GAConfig.h>
#include <Services/constants.h>

#include <Crossover/SinglePointCrossover.h>
#include <Crossover/Block3DCrossover.h>
#include <Crossover/Planar2PointCrossover.h>
#include <Crossover/SymmetryForcedCrossover.h>

class GeneticAlgorithm
{
public:
	GeneticAlgorithm(
		std::function<IIndividual* ()> createIndividual,
		size_t populationSize,
		size_t numberOfEpochs,
		double crossoverProbabillity,
		double mutationProbability,
		std::unique_ptr<SelectionStrategy> selectionStrategy,
		std::unique_ptr<CrossoverStrategy> crossoverStrategy);

	GeneticAlgorithm(
		std::function<IIndividual* ()> createIndividual,
		const GAConfig& config,
		std::unique_ptr<SelectionStrategy> selectionStrategy);

	GeneticAlgorithm(const GeneticAlgorithm& other) = delete;
	GeneticAlgorithm(GeneticAlgorithm&& other) = delete;

	GeneticAlgorithm& operator=(const GeneticAlgorithm& other) = delete;
	GeneticAlgorithm& operator=(GeneticAlgorithm&& other) = delete;

	~GeneticAlgorithm() = default;

	void Run();

	IIndividual* GetWinnerIndividual();
	const std::vector<std::vector<bool>>& GetBestEpochIndividual() const;
	const std::vector<int>& GetBestRemovedPerEpoch() const;
	const std::vector<double>& GetBestStressPerEpoch() const;
	const std::vector<double>& GetBestFitnessPerEpoch() const;
	
	int GetBestFitnessEpochIndex() const;

private:
	void InitializePopulation();

	void CalculateFitnessValues();

	void Crossover();
	void Mutation();

	void WriteWinners(int epoch);

private:
	std::vector<std::shared_ptr<IIndividual>> m_population;
	std::vector<std::shared_ptr<IIndividual>> m_workingPopulation;

	std::vector<std::vector<bool>> m_bestEpochIndividual;
	std::vector<int> m_bestRemovedPerEpoch;
	std::vector<double> m_bestStressPerEpoch;
	std::vector<double> m_bestFitnessPerEpoch;

	std::unique_ptr<SelectionStrategy> m_selectionStrategy;
	std::unique_ptr<CrossoverStrategy> m_crossoverStrategy;

	std::function<IIndividual* ()> m_createIndividual;

	std::map<IIndividual*, double> m_fitnessValues;

	size_t m_populationSize;
	size_t m_numberOfEpochs;

	double m_crossoverProbability;
	double m_mutationProbability;
};