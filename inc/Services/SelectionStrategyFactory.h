#pragma once

#include "Selection/RankedSelection.h"
#include "Selection/RouletteWheelSelection.h"
#include "Selection/StochasticUniversalSamplingSelection.h"
#include "Selection/TournamentSelection.h"

constexpr uint8_t TOURNAMENT_SIZE{ 3u };

class SelectionStrategyFactory
{
public:
	static std::unique_ptr<ISelectionStrategy> Create(int selectionMethod)
	{
		switch (selectionMethod)
		{
		case 0: return std::make_unique<RouletteWheelSelection>();
		case 1: return std::make_unique<TournamentSelection>(TOURNAMENT_SIZE);
		case 2: return std::make_unique<RankedSelection>();
		case 3: return std::make_unique<StochasticUniversalSamplingSelection>();
		default: return nullptr;
		}
	}
};