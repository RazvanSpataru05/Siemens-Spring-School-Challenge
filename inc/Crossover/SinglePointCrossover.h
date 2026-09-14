#pragma once

#include "Crossover/ICrossoverStrategy.h"

class SinglePointCrossover : public ICrossoverStrategy
{
public:
	SinglePointCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~SinglePointCrossover() = default;
};
