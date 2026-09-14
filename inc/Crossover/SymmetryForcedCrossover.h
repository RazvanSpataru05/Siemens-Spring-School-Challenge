#pragma once

#include "Crossover/ICrossoverStrategy.h"

class SymmetryForcedCrossover : public ICrossoverStrategy
{
public:
	SymmetryForcedCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~SymmetryForcedCrossover() = default;
};
