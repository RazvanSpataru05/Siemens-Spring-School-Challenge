#pragma once

#include <algorithm>

#include "Selection/ISelectionStrategy.h"

#include "Services/RandomNumbersGenerator.h"

class RankedSelection : public ISelectionStrategy
{
public:
	RankedSelection() = default;
	~RankedSelection() = default;

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);
};