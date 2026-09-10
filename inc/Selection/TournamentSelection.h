#pragma once

#include "Services/SelectionStrategy.h"
#include "Services/RandomNumbersGenerator.h"

class TournamentSelection : public SelectionStrategy
{
public:
	TournamentSelection(int tournamentSize = 3);
	~TournamentSelection() = default;

	void Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
		const std::map<IIndividual*, double>& fitnessValues);

private:
	int m_tournamentSize;
};

