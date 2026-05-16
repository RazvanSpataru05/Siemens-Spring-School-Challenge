#pragma once

#include <tuple>
#include <vector>
#include <fstream>
#include <filesystem>

#include <GeneticAlgorithm/Individual.h>

#include <GraphicalObjects/Building.h>

#include <Services/constants.h>

#include <Scene.h>

class IOIndividualManager
{
public:
	static void ReadIndividualsDetailsAndCreateBuildings();
	static std::vector<bool> ReadInitialIndividual(int individualSize);

	static void WriteIndividualDetailsInFile(IIndividual* individual);
	static void WriteIndividualValueInFile(int epoch, double fitnessValue, bool append);
};