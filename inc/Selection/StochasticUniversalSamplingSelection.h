#pragma once
#include <Services/SelectionStrategy.h>
#include <Services/RandomNumbersGenerator.h>
#include <Services/GeneticAlgorithmService.h>

class StochasticUniversalSamplingSelection : public SelectionStrategy
{
public:
	StochasticUniversalSamplingSelection();

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);

	~StochasticUniversalSamplingSelection();
};