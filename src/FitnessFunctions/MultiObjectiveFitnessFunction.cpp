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

std::unique_ptr<IFitnessFunction> MultiObjectiveFitnessFunction::Clone() const
{
    return std::make_unique<MultiObjectiveFitnessFunction>(m_alpha);
}