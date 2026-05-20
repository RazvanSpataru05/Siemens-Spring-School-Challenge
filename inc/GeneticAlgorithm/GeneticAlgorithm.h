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
#include <Services/constants.h>

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

	GeneticAlgorithm(const GeneticAlgorithm& other) = delete;
	GeneticAlgorithm(GeneticAlgorithm&& other) = delete;

	GeneticAlgorithm& operator=(const GeneticAlgorithm& other) = delete;
	GeneticAlgorithm& operator=(GeneticAlgorithm&& other) = delete;

	~GeneticAlgorithm() = default;

	void Run();

	IIndividual* GetWinnerIndividual();

private:
	void InitializePopulation();

	void CalculateFitnessValues();

	void Crossover();
	void Mutation();

	bool IsGreaterThan(double value, double lowerBound) const;
	bool IsLessThanOrEqualTo(double value, double upperBound) const;

	void WriteWinners(int epoch);

private:
	std::vector<std::shared_ptr<IIndividual>> m_population;
	std::vector<std::shared_ptr<IIndividual>> m_workingPopulation;

	std::unique_ptr<SelectionStrategy> m_selectionStrategy;
	std::unique_ptr<CrossoverStrategy> m_crossoverStrategy;

	std::function<IIndividual* ()> m_createIndividual;

	std::map<IIndividual*, double> m_fitnessValues;

	size_t m_populationSize;
	size_t m_numberOfEpochs;

	double m_crossoverProbability;
	double m_mutationProbability;
};