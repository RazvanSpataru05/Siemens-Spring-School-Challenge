#pragma once

#include <GeneticAlgorithm/GeneticAlgorithm.h>
#include <GeneticAlgorithm/UtilFunctions.h>

#include <Services/AlgorithmSettings.h>
#include <Services/CrossoverStrategy.h>

class GeneticAlgorithmService
{
public:
	static void RunWithConfigValues(
		std::unique_ptr<SelectionStrategy> selectionStrategy,
		std::unique_ptr<CrossoverStrategy> crossoverStrategy);
	static void RunWithCustomFirstIndividual(
		std::unique_ptr<SelectionStrategy> selectionStrategy,
		std::unique_ptr<CrossoverStrategy> crossoverStrategy);

	static std::vector<std::shared_ptr<Building>> ShowResults(
		const std::vector<std::vector<bool>>& bestEpochIndividual);

	static double CalculateSumOfFitnessValues(
		const std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double> fitnessValues);

	static std::vector<double> CalculateProbabilityOfSelection(
		const std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double> fitnessValues);

	static std::vector<double> CalcutateCumulativeProbabilityOfSelection(
		const std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double> fitnessValues);

};