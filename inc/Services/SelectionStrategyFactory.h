#pragma once
#include "Services/SelectionStrategy.h"
#include "Selection/RankedSelection.h"
#include "Selection/RouletteWheelSelection.h"
#include "Selection/StochasticUniversalSamplingSelection.h"
#include "Selection/TournamentSelection.h"

class SelectionStrategyFactory
{
public:
	static std::unique_ptr<SelectionStrategy> Create(int selectionMethod)
	{
		switch (selectionMethod)
		{
		case 0: return std::make_unique<RouletteWheelSelection>();
		case 1: return std::make_unique<TournamentSelection>(3);
		case 2: return std::make_unique<RankedSelection>();
		case 3: return std::make_unique<StochasticUniversalSamplingSelection>();
		default: return nullptr;
		}
	}
};