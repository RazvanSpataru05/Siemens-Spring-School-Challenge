#pragma once
#include "GraphicalObjects/Building.h"

struct GAResult
{
	std::vector<std::shared_ptr<Building>> epochBuildings;
	int bestFitnessEpochIndex{};
};
