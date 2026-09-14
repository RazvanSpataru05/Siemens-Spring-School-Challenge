#pragma once

#include "Selection/ISelectionStrategy.h"

#include "Services/RandomNumbersGenerator.h"
#include "Services/GeneticAlgorithmService.h"

class StochasticUniversalSamplingSelection : public ISelectionStrategy
{
public:
	StochasticUniversalSamplingSelection() = default;
	~StochasticUniversalSamplingSelection() = default;

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);
};