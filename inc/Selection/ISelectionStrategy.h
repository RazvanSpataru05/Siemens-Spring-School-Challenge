#pragma once

#include <vector>
#include <map>
#include <memory>

#include "GeneticAlgorithm/IIndividual.h"

class ISelectionStrategy
{
public:
	ISelectionStrategy() = default;
	virtual ~ISelectionStrategy() = 0;

	virtual void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues) = 0;

private:
	ISelectionStrategy& operator=(const ISelectionStrategy& other) = delete;
	ISelectionStrategy(const ISelectionStrategy& other) = delete;
};