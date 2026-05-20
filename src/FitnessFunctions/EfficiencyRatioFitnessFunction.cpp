#include <FitnessFunctions/EfficiencyRatioFitnessFunction.h>

const double EPSILON = 0.001;

double EfficiencyRatioFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    return removedElements / (simulatedStress + EPSILON);
}