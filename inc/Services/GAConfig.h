#pragma once

struct GAConfig
{
	int epochs{ 5 };
	int population{ 50 };
	int selectionMethod{ 0 };
	int crossoverMethod{ 0 };
	int fitnessMethod{ 0 };
	double crossoverProbability{ 0.1 };
	double mutationProbability{ 0.01 };
};