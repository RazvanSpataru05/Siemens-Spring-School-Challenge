#pragma once

#include <fstream>
#include <iostream>
#include <algorithm>

#include <Services/constants.h>

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

	int m_numbeOfEpochs;
	int m_numberOfIndividuals;

	int m_oxSize;
	int m_oySize;
	int m_ozSize;

	double m_elementSize;

	double m_crossoverProbability;
	double m_mutationProbability;

	double m_maximumStress;

	double m_youngModulus;
	double m_poissonRatio;
	double m_density;
};