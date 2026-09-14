#pragma once

#include "Selection/ISelectionStrategy.h"

#include "Services/RandomNumbersGenerator.h"
#include "Services/GeneticAlgorithmService.h"

class RouletteWheelSelection : public ISelectionStrategy
{
public:
	RouletteWheelSelection() = default;
	~RouletteWheelSelection() = default;

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);
};