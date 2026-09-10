#include <Scene.h>
#include <System/ConfigureSystem.h>

#include <GeneticAlgorithm/Individual.h>
#include <Services/AlgorithmSettings.h>

Scene::Scene() {}

void Scene::SetOnStartGA(std::function<void(const GAConfig&)> callback)
{
	m_onStartGA = std::move(callback);
}

void Scene::InitializeSystem(const std::shared_ptr<Building>& building, bool epochViewerMode)
{
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_epochViewerMode = epochViewerMode;
	if (m_epochViewerMode)
	{
		m_system = std::make_shared<chrono::ChSystemSMC>();
	}
	else
	{
		m_system = building->GetSystem();
	}

	m_application = std::make_shared<chrono::irrlicht::ChIrrApp>(m_system.get(),
		L"Genetic Algorithm Configuration", irr::core::dimension2d<irr::u32>(screenWidth, screenHeight));
	m_configureSystem = std::make_unique<ConfigureSystem>(m_application, m_system);
	m_configureSystem->SetOnStartGA(m_onStartGA);

	if (!m_cachedRemoved.empty())
		m_configureSystem->SetEpochStats(m_cachedRemoved, m_cachedStress, m_cachedFitness);

	if (m_epochViewerMode)
	{
		m_configureSystem->SetEpochNavigation(
			m_currentEpochIndex,
			static_cast<int>(m_epochGenes.size()),
			m_bestFitnessEpochIndex,
			[this](int epochIndex) { SwitchToEpoch(epochIndex); });
	}
	m_configureSystem->ConfigureIrrllichtScene();

	if (m_epochViewerMode) LoadBuildingIntoHost(m_epochGenes[m_currentEpochIndex]);

	else m_configureSystem->InitializeIrrlichtScene();

	m_configureSystem->SetIrrlichtSceneTimestep(0.001);
	m_configureSystem->SetSystemTimestepper();
	m_configureSystem->SetSystemSolver();

	if (!m_epochViewerMode) m_configureSystem->Simulate(0.1);
	
	m_configureSystem->RunIrrlichtScene();
}

void Scene::ShowInitialBuilding(const std::shared_ptr<Building>& building)
{
	SetVisualizationProperties(building);

	if (!m_initialized)
	{
		InitializeSystem(building, false);
		m_initialized = true;
	}
}

void Scene::ShowEpochResults(const std::vector<std::vector<bool>>& epochGenes,
	int bestFitnessEpochIndex, int startEpochIndex)
{
	if (epochGenes.empty()) return;

	m_epochGenes = epochGenes;
	m_bestFitnessEpochIndex = bestFitnessEpochIndex;
	m_currentEpochIndex = startEpochIndex;

	if (m_currentEpochIndex < 0 || m_currentEpochIndex >= static_cast<int>(m_epochGenes.size()))
	{
		m_currentEpochIndex = static_cast<int>(m_epochGenes.size()) - 1;
	}

	if (!m_initialized)
	{
		InitializeSystem(nullptr, true);
		m_initialized = true;
	}
}

void Scene::SetEpochStats(std::vector<int> removed, std::vector<double> stress, std::vector<double> fitness)
{
	m_cachedRemoved = std::move(removed);
	m_cachedStress = std::move(stress);
	m_cachedFitness = std::move(fitness);

	if (m_configureSystem) 
		m_configureSystem->SetEpochStats(m_cachedRemoved, m_cachedStress, m_cachedFitness);
}

void Scene::LoadBuildingIntoHost(const std::vector<bool>& cubesExistence)
{
	auto* settings = AlgorithmSettings::GetInstance();
	auto displayBuilding = Individual::CreateBuildingFromDetails(
		settings->GetOxSize(),
		settings->GetOySize(),
		settings->GetOzSize(),
		settings->GetElementSize(),
		cubesExistence);

	SetVisualizationProperties(displayBuilding);

	m_system->Clear();

	auto sourceSystem = displayBuilding->GetSystem();
	m_system->Add(displayBuilding->GetMesh());

	for (const auto& body : sourceSystem->Get_bodylist())
	{
		m_system->Add(body);
	}

	for (const auto& link : sourceSystem->Get_linklist())
	{
		m_system->Add(link);
	}

	m_configureSystem->SetSystemTimestepper();
	m_configureSystem->SetSystemSolver();
	m_configureSystem->Simulate(0.1);
	m_application->AssetBindAll();
	m_application->AssetUpdateAll();
}

void Scene::SwitchToEpoch(int epochIndex)
{
	if (!m_epochViewerMode || epochIndex < 0 || epochIndex >= static_cast<int>(m_epochGenes.size()))
		return;

	m_currentEpochIndex = epochIndex;
	LoadBuildingIntoHost(m_epochGenes[m_currentEpochIndex]);
	m_configureSystem->SetEpochNavigation(
		m_currentEpochIndex,
		static_cast<int>(m_epochGenes.size()),
		m_bestFitnessEpochIndex,
		[this](int index) { SwitchToEpoch(index); });
}

void Scene::Shutdown()
{
	m_configureSystem.reset();
	m_application.reset();
	m_system.reset();
	m_epochGenes.clear();
	m_epochViewerMode = false;
	m_currentEpochIndex = 0;
	m_bestFitnessEpochIndex = -1;
	m_initialized = false;

	MSG msg{};

	/* while block must not be deleted, as the app will no longer compile */
	while (PeekMessage(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)) {} 
}

void Scene::SetVisualizationProperties(const std::shared_ptr<Building>& building)
{
	auto& mesh = *building->GetMesh();
	ObjectProperties::SetVisualizationMesh(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(mesh));
	ObjectProperties::SetVisualizationMeshReference(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(mesh));
	ObjectProperties::SetVisualizationMeshPoints(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(mesh));
}

void Scene::RequestClose()
{
	if (m_configureSystem) m_configureSystem->RequestClose();
}
