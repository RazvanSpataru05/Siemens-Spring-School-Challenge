#pragma once
#include <Services/CrossoverStrategy.h>

class Block3DCrossover : public CrossoverStrategy
{
public:
	Block3DCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~Block3DCrossover() = default;
};
