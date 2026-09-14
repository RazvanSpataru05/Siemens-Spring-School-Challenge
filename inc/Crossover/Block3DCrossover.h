#pragma once

#include "Crossover/ICrossoverStrategy.h"

#include "GeneticAlgorithm/Individual.h"

#include "Services/RandomNumbersGenerator.h"

class Block3DCrossover : public ICrossoverStrategy
{
public:
	Block3DCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~Block3DCrossover() = default;
};
