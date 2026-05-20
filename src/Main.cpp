#include <Services/GeneticAlgorithmService.h>
#include <Services/GAConfig.h>
#include <Services/AlgorithmSettings.h>

#include <Scene.h>

#include <thread>
#include <atomic>

std::atomic<bool> gaRunning{ false };
std::thread gaThread;

int main(int argc, char* argv[]) {
	chrono::SetChronoDataPath(CHRONO_DATA_DIR);

	auto* settings = AlgorithmSettings::GetInstance();
	int oxSize = settings->GetOxSize();
	int oySize = settings->GetOySize();
	int ozSize = settings->GetOzSize();
	double elementSize = settings->GetElementSize();
	auto cubesExistence = IOIndividualManager::ReadInitialIndividual(oxSize * oySize * ozSize);
	auto createIndividual = [&, oxSize, oySize, ozSize, elementSize, cubesExistence](int fitnessMethod) {
		return UtilFunctions::CreateIndividualFromFileFunction(oxSize, oySize, ozSize, elementSize,
			cubesExistence, AlgorithmSettings::GetInstance()->GetMaximStressAllowed(), fitnessMethod);
	};

	auto initialBuilding = Individual::CreateBuildingFromDetails(
		oxSize, oySize, ozSize, elementSize, cubesExistence);

	std::shared_ptr<Building> resultBuilding;
	Scene scene;
	scene.SetOnStartGA([&](const GAConfig& config)
		{
			if (gaRunning.load()) return;
			if (gaThread.joinable()) gaThread.join();

			gaRunning = true;
			gaThread = std::thread([&, config]()
				{
					std::unique_ptr<SelectionStrategy> selectionStrategy;
					switch (config.selectionMethod)
					{
					case 0: selectionStrategy = std::make_unique<RouletteWheelSelection>(); break;
					case 1: selectionStrategy = std::make_unique<TournamentSelection>(3); break;
					case 2: selectionStrategy = std::make_unique<RankedSelection>(); break;
					case 3: selectionStrategy = std::make_unique<StochasticUniversalSamplingSelection>(); break;
					}
					auto individualFactory = createIndividual(config.fitnessMethod);
					GeneticAlgorithm ga(individualFactory, config, std::move(selectionStrategy));
					ga.Run();
					std::cout << "GA RUN FINISHED";

					auto results = GeneticAlgorithmService::ShowResults(ga.GetBestEpochIndividual());
					if (!results.empty())
					{
						resultBuilding = results.back();
					}
					gaRunning = false;
					scene.RequestClose();
				});
		});
	scene.Show(initialBuilding);

	if (gaThread.joinable()) gaThread.join();

	std::cout << "first scene closed\n";
	if (resultBuilding)
	{
		std::cout << "have result, processing it\n";
		scene.Shutdown();
		scene.SetOnStartGA({});
		scene.Show(resultBuilding);
	}
	else
	{
		std::cout << "resultBuilding is NULL - GA didn't produce results\n";
	}
	return 0;
}