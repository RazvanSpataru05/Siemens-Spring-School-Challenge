#include <FitnessFunctions/MultiObjectiveFitnessFunction.h>

MultiObjectiveFitnessFunction::MultiObjectiveFitnessFunction(double alpha)
    : m_alpha{ alpha }
{
}

double MultiObjectiveFitnessFunction::Evaluate(int removedElements, int maxRemovedElements,
    double simulatedStress, double maxStress)
{
    double materialTerm = static_cast<double>(removedElements) / maxRemovedElements;
    double safetyTerm = 1.0 - (simulatedStress / maxStress);

    return m_alpha * materialTerm + (1.0 - m_alpha) * safetyTerm;
}