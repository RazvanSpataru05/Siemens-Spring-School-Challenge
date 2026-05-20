#pragma once
#include <Services/CrossoverStrategy.h>

class SymmetryForcedCrossover : public CrossoverStrategy
{
public:
	SymmetryForcedCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~SymmetryForcedCrossover() = default;
};
