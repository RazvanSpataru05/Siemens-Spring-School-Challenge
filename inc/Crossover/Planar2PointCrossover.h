#pragma once

#include "Crossover/ICrossoverStrategy.h"

class Planar2PointCrossover : public ICrossoverStrategy
{
public:
	Planar2PointCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~Planar2PointCrossover() = default;
};
