#pragma once
#include <algorithm>

#include <Services/SelectionStrategy.h>
#include <Services/RandomNumbersGenerator.h>

class RankedSelection : public SelectionStrategy
{
public:
	RankedSelection() = default;
	~RankedSelection() = default;

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);
};