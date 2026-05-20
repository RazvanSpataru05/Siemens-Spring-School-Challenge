#pragma once
#include <FitnessFunctions/IFitnessFunction.h>

class MultiObjectiveFitnessFunction : public IFitnessFunction
{
public:
    MultiObjectiveFitnessFunction(double alpha = 0.5);
    ~MultiObjectiveFitnessFunction() = default;

    double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) override;

private:
    double m_alpha;
};