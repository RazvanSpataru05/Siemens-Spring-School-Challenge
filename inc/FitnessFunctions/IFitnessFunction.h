#pragma once

#include <memory>

class IFitnessFunction
{
public:
    IFitnessFunction() = default;

    virtual double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) = 0;
    virtual std::unique_ptr<IFitnessFunction> Clone() const = 0;
    virtual ~IFitnessFunction() = default;

private:
    IFitnessFunction(const IFitnessFunction&) = delete;
    IFitnessFunction& operator=(const IFitnessFunction&) = delete;
};