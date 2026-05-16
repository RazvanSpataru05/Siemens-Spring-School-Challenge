#pragma once

class IIndividual
{
public:
	virtual double Evaluate() = 0;

	virtual void Crossover(IIndividual& other) = 0;
	virtual void Mutation(double mutationProbability) = 0;
};