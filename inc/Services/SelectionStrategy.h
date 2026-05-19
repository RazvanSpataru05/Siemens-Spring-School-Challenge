#pragma once
#include <vector>
#include <map>
#include <memory>

#include <GeneticAlgorithm/IIndividual.h>

class SelectionStrategy
{
public:
	SelectionStrategy() = default;

	virtual void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues) = 0;

	virtual ~SelectionStrategy() = 0;

private:
	SelectionStrategy& operator=(const SelectionStrategy& other) = delete;
	SelectionStrategy(const SelectionStrategy& other) = delete;
};