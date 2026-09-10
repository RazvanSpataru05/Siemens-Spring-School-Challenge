#pragma once

#include <GeneticAlgorithm/IIndividual.h>

class CrossoverStrategy
{
public:
	CrossoverStrategy() = default;

	virtual void Crossover(IIndividual& first, IIndividual& second) = 0;

	virtual ~CrossoverStrategy() = 0;

private:
	CrossoverStrategy& operator=(const CrossoverStrategy& other) = delete;
	CrossoverStrategy(const CrossoverStrategy& other) = delete;
};
