#pragma once

#include "GeneticAlgorithm/IIndividual.h"

class ICrossoverStrategy
{
public:
	ICrossoverStrategy() = default;

	virtual void Crossover(IIndividual& first, IIndividual& second) = 0;

	virtual ~ICrossoverStrategy() = 0;

private:
	ICrossoverStrategy& operator=(const ICrossoverStrategy& other) = delete;
	ICrossoverStrategy(const ICrossoverStrategy& other) = delete;
};
