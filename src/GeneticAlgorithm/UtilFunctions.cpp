#include <GeneticAlgorithm/UtilFunctions.h>

#include <FitnessFunctions/OriginalFitnessFunction.h>
#include <FitnessFunctions/EfficiencyRatioFitnessFunction.h>
#include <FitnessFunctions/ExponentialPenaltyFitnessFunction.h>
#include <FitnessFunctions/MultiObjectiveFitnessFunction.h>

std::function<IIndividual* ()> UtilFunctions::CreateIndividualFunction(int sizeOx, int sizeOy, int sizeOz,
    double elementSize, double maximStress)
{
    return [sizeOx, sizeOy, sizeOz, elementSize, maximStress]() {
        Individual* individual = new Individual(sizeOx, sizeOy, sizeOz, elementSize, maximStress,
            std::make_unique<ExponentialPenaltyFitnessFunction>());
        return individual;
        };
}

std::function<IIndividual* ()> UtilFunctions::CreateIndividualFromFileFunction(int sizeOx, int sizeOy, int sizeOz,
    double elementSize, const std::vector<bool>& cubesExistence, double maximStress)
{
    return [sizeOx, sizeOy, sizeOz, elementSize, cubesExistence, maximStress]() {
        Individual* individual = new Individual(sizeOx, sizeOy, sizeOz, elementSize, cubesExistence, maximStress,
            std::make_unique<ExponentialPenaltyFitnessFunction>());
        return individual;
        };
}