#pragma once

#include <FitnessFunctions/IFitnessFunction.h>

class ExponentialPenaltyFitnessFunction : public IFitnessFunction
{
public:
    ExponentialPenaltyFitnessFunction() = default;
    ~ExponentialPenaltyFitnessFunction() = default;

    double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) override;
};