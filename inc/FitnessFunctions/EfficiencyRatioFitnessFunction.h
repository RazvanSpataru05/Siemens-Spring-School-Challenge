#pragma once

#include <FitnessFunctions/IFitnessFunction.h>

class EfficiencyRatioFitnessFunction : public IFitnessFunction
{
public:
    EfficiencyRatioFitnessFunction() = default;
    ~EfficiencyRatioFitnessFunction() = default;

    double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) override;

    std::unique_ptr<IFitnessFunction> Clone() const override;
};