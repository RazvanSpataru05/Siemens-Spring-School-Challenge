#include <Services/GeneticAlgorithmService.h>

#include <Scene.h>

int main(int argc, char *argv[]) {
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);
  
	// Comment / Decomment this to run the first genetic algorithm execution using random/configured individuals
	//GeneticAlgorithmService::RunWithConfigValues();

	// Comment / Decomment this to run the second genetic algorithm execution using 
	// the initial individual from "initial_individual.txt"
	GeneticAlgorithmService::RunWithCustomFirstIndividual();

	GeneticAlgorithmService::ShowResults();

	// SCENE TESTS
	Scene scene;
	scene.Show(scene.CreateCustomWallScene());

	return 0;
}