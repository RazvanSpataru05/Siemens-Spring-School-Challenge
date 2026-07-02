#include <FitnessFunctions/EfficiencyRatioFitnessFunction.h>

const double EPSILON = 0.001;

double EfficiencyRatioFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    return 1.0 * removedElements / (simulatedStress + EPSILON);
}

std::unique_ptr<IFitnessFunction> EfficiencyRatioFitnessFunction::Clone() const
{
    return std::make_unique<EfficiencyRatioFitnessFunction>();
}