#pragma once

class IIndividual
{
public:
	virtual ~IIndividual() = default;

	virtual double Evaluate() = 0;

	virtual void Mutation(double mutationProbability) = 0;
};