#pragma once

class IFitnessFunction
{
public:
    IFitnessFunction() = default;

    virtual double Evaluate(int removedElements, int maxRemovedElements,
        double simulatedStress, double maxStress) = 0;

    virtual ~IFitnessFunction() = default;

private:
    IFitnessFunction(const IFitnessFunction& other) = delete;
    IFitnessFunction& operator=(const IFitnessFunction& other) = delete;
};