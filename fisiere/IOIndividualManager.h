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
	static std::vector<std::shared_ptr<Building>> CreateAllBuildings(const std::vector<std::vector<bool>>& bestEpochIndividual);
	static void ReadIndividualsDetailsAndCreateBuildings();
	static std::vector<bool> ReadInitialIndividual(int individualSize);

	static void WriteIndividualDetailsInFile(IIndividual* individual);
	static void WriteIndividualValueInFile(int epoch, double fitnessValue, bool append);
};