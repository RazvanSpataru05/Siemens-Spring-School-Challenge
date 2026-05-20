#include <FitnessFunctions/OriginalFitnessFunction.h>
#include <cmath>

double OriginalFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    return pow(removedElements + 1, 2) * (maxStress - simulatedStress);
}

std::unique_ptr<IFitnessFunction> OriginalFitnessFunction::Clone() const
{
    return std::make_unique<OriginalFitnessFunction>();
}