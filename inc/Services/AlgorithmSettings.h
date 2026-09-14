#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>
#include <memory>

#include "Selection/ISelectionStrategy.h"
#include "Services/Constants.h"

class AlgorithmSettings
{
public:
	static AlgorithmSettings* GetInstance();

	[[nodiscard]] const int GetNumberOfEpochs() const;
	[[nodiscard]] const int GetNumberOfIndividuals() const;
	[[nodiscard]] const int GetOxSize() const;
	[[nodiscard]] const int GetOySize() const;
	[[nodiscard]] const int GetOzSize() const;
	[[nodiscard]] const double GetElementSize() const;
	[[nodiscard]] const double GetCrossoverProbability() const;
	[[nodiscard]] const double GetMutationProbability() const;

	[[nodiscard]] const double GetMaximStressAllowed() const;
	[[nodiscard]] const double GetYoungModulus() const;
	[[nodiscard]] const double GetPoissonRatio() const;
	[[nodiscard]] const double GetDensity() const;

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