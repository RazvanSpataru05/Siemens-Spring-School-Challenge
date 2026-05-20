#include <GeneticAlgorithm/UtilFunctions.h>

#include <FitnessFunctions/OriginalFitnessFunction.h>
#include <FitnessFunctions/EfficiencyRatioFitnessFunction.h>
#include <FitnessFunctions/ExponentialPenaltyFitnessFunction.h>
#include <FitnessFunctions/MultiObjectiveFitnessFunction.h>

std::function<IIndividual* ()> UtilFunctions::CreateIndividualFunction(int sizeOx, int sizeOy, int sizeOz,
    double elementSize, double maximStress, int fitnessMethod)
{
    return [sizeOx, sizeOy, sizeOz, elementSize, maximStress, fitnessMethod]() {
        std::unique_ptr<IFitnessFunction> fitnessFunction;
        switch (fitnessMethod)
        {
        case 0: fitnessFunction = std::make_unique<OriginalFitnessFunction>(); break;
        case 1: fitnessFunction = std::make_unique<EfficiencyRatioFitnessFunction>(); break;
        case 2: fitnessFunction = std::make_unique<ExponentialPenaltyFitnessFunction>(); break;
        case 3: fitnessFunction = std::make_unique<MultiObjectiveFitnessFunction>(); break;
        }
        Individual* individual = new Individual(sizeOx, sizeOy, sizeOz, elementSize, maximStress, std::move(fitnessFunction));
        return individual;
        };
}

std::function<IIndividual* ()> UtilFunctions::CreateIndividualFromFileFunction(int sizeOx, int sizeOy, int sizeOz,
    double elementSize, const std::vector<bool>& cubesExistence, double maximStress, int fitnessMethod)
{
    return [sizeOx, sizeOy, sizeOz, elementSize, cubesExistence, maximStress, fitnessMethod]() {
        std::unique_ptr<IFitnessFunction> fitnessFunction;
        switch (fitnessMethod)
        {
        case 0: fitnessFunction = std::make_unique<OriginalFitnessFunction>(); break;
        case 1: fitnessFunction = std::make_unique<EfficiencyRatioFitnessFunction>(); break;
        case 2: fitnessFunction = std::make_unique<ExponentialPenaltyFitnessFunction>(); break;
        case 3: fitnessFunction = std::make_unique<MultiObjectiveFitnessFunction>(); break;
        }
        Individual* individual = new Individual(sizeOx, sizeOy, sizeOz, elementSize, cubesExistence, maximStress, std::move(fitnessFunction));
        return individual;
        };
}