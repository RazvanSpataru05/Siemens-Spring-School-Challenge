#pragma once

struct GAConfig
{
	size_t epochs{ 5 };
	size_t population{ 50 };
	int selectionMethod{ 0 };
	int crossoverMethod{ 0 };
	int fitnessMethod{ 0 };
	double crossoverProbability{ 0.1 };
	double mutationProbability{ 0.01 };
};