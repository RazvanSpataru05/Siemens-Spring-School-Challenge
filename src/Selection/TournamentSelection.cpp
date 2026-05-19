#include <Selection/TournamentSelection.h>

TournamentSelection::TournamentSelection(int tournamentSize) :
	m_tournamentSize{ tournamentSize } {
}

void TournamentSelection::Select(std::vector<std::shared_ptr<IIndividual>>& workingPopulation,
	const std::map<IIndividual*, double>& fitnessValues)
{
	std::vector<std::shared_ptr<IIndividual>> newPopulation;
	newPopulation.reserve(workingPopulation.size());

	for (size_t index = 0; index < workingPopulation.size(); ++index)
	{
		std::shared_ptr<IIndividual> bestIndividual = nullptr;
		double bestFitness = std::numeric_limits<double>::lowest();
		for (size_t tournamentIndex = 0; tournamentIndex < m_tournamentSize; ++tournamentIndex)
		{
			int randomIndex = RandomNumbersGenerator::GenerateIntegerNumberInRange(0, workingPopulation.size() - 1);
			if (fitnessValues.at(workingPopulation[randomIndex].get()) > bestFitness)
			{
				bestFitness = fitnessValues.at(workingPopulation[randomIndex].get());
				bestIndividual = workingPopulation[randomIndex];
			}
		}
		newPopulation.push_back(bestIndividual);
	}
	workingPopulation = std::move(newPopulation);
}

TournamentSelection::~TournamentSelection()
{
}
