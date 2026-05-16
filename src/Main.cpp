#include <Services/GeneticAlgorithmService.h>

#include <Scene.h>

int main(int argc, char* argv[])
{
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);

	GeneticAlgorithmService::RunWithConfigValues();

	GeneticAlgorithmService::RunWithCustomFirstIndividual();

	GeneticAlgorithmService::ShowResults();

	// SCENE TESTS
	 Scene scene;
	 scene.Show(scene.CreateCustomWallScene());

	return 0;
}