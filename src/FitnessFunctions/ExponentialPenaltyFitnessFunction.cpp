#include <FitnessFunctions/ExponentialPenaltyFitnessFunction.h>

#include <cmath>

double ExponentialPenaltyFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    return removedElements * exp(-(simulatedStress / maxStress));
}