#pragma once
#include <Services/CrossoverStrategy.h>

class SinglePointCrossover : public CrossoverStrategy
{
public:
	SinglePointCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~SinglePointCrossover() = default;
};
