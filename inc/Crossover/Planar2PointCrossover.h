#pragma once
#include <Services/CrossoverStrategy.h>

class Planar2PointCrossover : public CrossoverStrategy
{
public:
	Planar2PointCrossover() = default;

	void Crossover(IIndividual& first, IIndividual& second) override;

	~Planar2PointCrossover() = default;
};
