#pragma once

#include <FitnessFunctions/IFitnessFunction.h>

class OriginalFitnessFunction : public IFitnessFunction
{
public:
    OriginalFitnessFunction() = default;
    ~OriginalFitnessFunction() = default;

    double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) override;

    std::unique_ptr<IFitnessFunction> Clone() const override;
};