#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>

#include <Services/constants.h>
#include <Services/SelectionStrategy.h>

class AlgorithmSettings
{
public:
	static AlgorithmSettings* GetInstance();

	const int GetNumberOfEpochs() const;
	const int GetNumberOfIndividuals() const;
	const int GetOxSize() const;
	const int GetOySize() const;
	const int GetOzSize() const;
	const double GetElementSize() const;
	const double GetCrossoverProbability() const;
	const double GetMutationProbability() const;

	const double GetMaximStressAllowed() const;
	const double GetYoungModulus() const;
	const double GetPoissonRatio() const;
	const double GetDensity() const;

private:

	AlgorithmSettings();

	static AlgorithmSettings* m_instance;

	int m_numberOfEpochs{ 5 };
	int m_numberOfIndividuals{ 50 };

	int m_oxSize{ 10 };
	int m_oySize{ 3 };
	int m_ozSize{ 3 };

	double m_elementSize{ 0.1 };

	double m_crossoverProbability{ 0.5 };
	double m_mutationProbability{ 0.01 };

	double m_maximumStress{ 4.5e+08 };

	double m_youngModulus{ 1e+21 };
	double m_poissonRatio{ 0.3 };
	double m_density{ 7800 };
};