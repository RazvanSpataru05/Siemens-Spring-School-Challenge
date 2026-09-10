#pragma once
#include <vector>

struct GAResult
{
	std::vector<std::vector<bool>> epochGenes;
	int bestFitnessEpochIndex{};
};
