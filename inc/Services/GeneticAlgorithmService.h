#pragma once

#include <GeneticAlgorithm/GeneticAlgorithm.h>
#include <GeneticAlgorithm/UtilFunctions.h>

#include <Services/AlgorithmSettings.h>

class GeneticAlgorithmService
{
public:
	static void RunWithConfigValues();
	static void RunWithCustomFirstIndividual();

	static void ShowResults();
};