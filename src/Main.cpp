#include <Services/GeneticAlgorithmService.h>

#include <Selection/TournamentSelection.h>
#include <Selection/RankedSelection.h>
#include <Selection/StochasticUniversalSamplingSelection.h>

#include <Scene.h>

int main(int argc, char* argv[]) {
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);

	// Comment / Decomment this to run the first genetic algorithm execution using random/configured individuals
	//GeneticAlgorithmService::RunWithConfigValues();

	// Comment / Decomment this to run the second genetic algorithm execution using 
	// the initial individual from "initial_individual.txt"
	std::unique_ptr<SelectionStrategy> touramentSelection = std::make_unique<TournamentSelection>(3);
	std::unique_ptr<SelectionStrategy> rankedSelection = std::make_unique<RankedSelection>();
	std::unique_ptr<SelectionStrategy> stochasticSelection = std::make_unique<StochasticUniversalSamplingSelection>();

	GeneticAlgorithmService::RunWithCustomFirstIndividual(std::move(stochasticSelection));
	GeneticAlgorithmService::ShowResults();

	// SCENE TESTS
	//Scene scene;
	//scene.Show(scene.CreateCustomWallScene());

	return 0;
}