#pragma once
#pragma once

#include "Crossover/ICrossoverStrategy.h"

#include "GeneticAlgorithm/GeneticAlgorithm.h"
#include "GeneticAlgorithm/UtilFunctions.h"

#include "Services/AlgorithmSettings.h"

class GeneticAlgorithmService
{
public:
	static void RunWithConfigValues(
		std::unique_ptr<ISelectionStrategy> selectionStrategy,
		std::unique_ptr<ICrossoverStrategy> crossoverStrategy);

	static void RunWithCustomFirstIndividual(
		std::unique_ptr<ISelectionStrategy> selectionStrategy,
		std::unique_ptr<ICrossoverStrategy> crossoverStrategy);


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