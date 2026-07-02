#include <Services/GAApplication.h>

GAApplication::GAApplication()
{
	auto* settings = AlgorithmSettings::GetInstance();
	m_oxSize = settings->GetOxSize();
	m_oySize = settings->GetOySize();
	m_ozSize = settings->GetOzSize();
	m_elementSize = settings->GetElementSize();

	m_cubesExistence = IOIndividualManager::ReadInitialIndividual(m_oxSize * m_oySize * m_ozSize);

	m_initialBuilding = Individual::CreateBuildingFromDetails(
		m_oxSize, m_oySize, m_ozSize, m_elementSize, m_cubesExistence);
	m_scene.SetOnStartGA([this](const GAConfig& config) {StartGA(config); });
}

void GAApplication::Run()
{
	m_scene.ShowInitialBuilding(m_initialBuilding);

	if (m_gaThread.joinable()) m_gaThread.join();

	if (m_hasResult)
	{
		m_scene.Shutdown();
		m_scene.SetOnStartGA({});
		m_scene.ShowEpochResults(m_result.epochBuildings, m_result.bestFitnessEpochIndex);
	}
}

void GAApplication::StartGA(const GAConfig& config)
{
	if (m_gaRunning.load()) return;
	if (m_gaThread.joinable()) m_gaThread.join();

	m_gaRunning = true;

	m_gaThread = std::jthread([this, config]()
		{
			auto selectionStrategy = SelectionStrategyFactory::Create(config.selectionMethod);
			auto individualFactory = UtilFunctions::CreateIndividualFromFileFunction(
				m_oxSize, m_oySize, m_ozSize, m_elementSize,
				m_cubesExistence, AlgorithmSettings::GetInstance()->GetMaximStressAllowed(),
				config.fitnessMethod);

			GeneticAlgorithm ga(individualFactory, config, std::move(selectionStrategy));
			ga.Run();

			m_scene.SetEpochStats(
				ga.GetBestRemovedPerEpoch(),
				ga.GetBestStressPerEpoch(),
				ga.GetBestFitnessPerEpoch());

			m_result.epochBuildings = GeneticAlgorithmService::ShowResults(ga.GetBestEpochIndividual());
			m_result.bestFitnessEpochIndex = ga.GetBestFitnessEpochIndex();
			m_hasResult = true;

			m_gaRunning = false;
			m_scene.RequestClose();
		});
}