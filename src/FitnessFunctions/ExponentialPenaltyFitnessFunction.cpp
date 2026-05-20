#include <FitnessFunctions/ExponentialPenaltyFitnessFunction.h>

#include <cmath>

double ExponentialPenaltyFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    return removedElements * exp(-(simulatedStress / maxStress));
}

std::unique_ptr<IFitnessFunction> ExponentialPenaltyFitnessFunction::Clone() const
{
    return std::make_unique<ExponentialPenaltyFitnessFunction>();
}