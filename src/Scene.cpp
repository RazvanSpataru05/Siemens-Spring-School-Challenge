#include <Scene.h>
#include <System/ConfigureSystem.h>

Scene::Scene()
{
}

void Scene::SetOnStartGA(std::function<void(const GAConfig&)> callback)
{
	m_onStartGA = std::move(callback);
}

void Scene::InitializeSystem(const std::shared_ptr<Building>& building)
{
	std::cout << "[2] InitSystem start\n";
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	m_system = building->GetSystem();
	std::cout << "[2] creating ChIrrApp\n";
	m_application = std::make_shared<chrono::irrlicht::ChIrrApp>(m_system.get(),
		L"Optimization result", irr::core::dimension2d<irr::u32>(screenWidth, screenHeight));
	std::cout << "[2] creating ConfigureSystem\n";
	m_configureSystem = std::make_unique<ConfigureSystem>(m_application, m_system);
	m_configureSystem->SetOnStartGA(m_onStartGA);
	std::cout << "[2] configuring\n";
	m_configureSystem->ConfigureIrrllichtScene();
	m_configureSystem->InitializeIrrlichtScene();
	m_configureSystem->SetIrrlichtSceneTimestep(0.001);
	m_configureSystem->SetSystemTimestepper();
	m_configureSystem->SetSystemSover();
	std::cout << "[2] simulating\n";
	m_configureSystem->Simulate(0.1);
	std::cout << "[2] entering Run loop\n";
	m_configureSystem->RunIrrlichtScene();
	std::cout << "[2] Run loop EXITED\n";
}

void Scene::Show(const std::shared_ptr<Building>& building)
{
	SetVisualizationProperties(building);

	if (!m_initialized)
	{
		InitializeSystem(building);
		m_initialized = true;
	}
}

void Scene::Shutdown()
{
	m_configureSystem.reset();
	m_application.reset();
	m_system.reset();
	m_initialized = false;

	MSG msg{};
	while (PeekMessage(&msg, nullptr, WM_QUIT, WM_QUIT, PM_REMOVE)) {}
}

std::shared_ptr<Building> Scene::CreateSimpleWallScene()
{
	auto customBuilding = std::make_shared<Building>(1, 1, 1, 2);

	customBuilding->Build();
	customBuilding->AddConstraints();

	return customBuilding;
}

std::shared_ptr<Building> Scene::CreateComplexWallScene()
{
	auto customBuilding = std::make_shared<Building>(20, 8, 5, 0.05);
	customBuilding->Build();
	customBuilding->AddConstraints();

	std::vector<bool> importanceForEliminatingCubes = std::vector<bool>(20 * 8 * 5, true);

	for (int i = 0; i < importanceForEliminatingCubes.size(); ++i)
		importanceForEliminatingCubes[i] = false;

	customBuilding->EliminateCubesBasedOnCubesExistence(importanceForEliminatingCubes);

	for (int i = 0; i < importanceForEliminatingCubes.size(); ++i)
		importanceForEliminatingCubes[i] = true;

	customBuilding->AddCubesBasedOnCubesExistence(importanceForEliminatingCubes);

	importanceForEliminatingCubes[0] = false;
	importanceForEliminatingCubes[1] = false;
	importanceForEliminatingCubes[19] = false;
	importanceForEliminatingCubes[18] = false;

	customBuilding->EliminateCubesBasedOnCubesExistence(importanceForEliminatingCubes);

	importanceForEliminatingCubes[1] = true;
	importanceForEliminatingCubes[5] = true;
	importanceForEliminatingCubes[19] = true;

	customBuilding->AddCubesBasedOnCubesExistence(importanceForEliminatingCubes);

	return customBuilding;
}

std::shared_ptr<Building> Scene::CreateCustomWallScene()
{
	return CreateSimpleWallScene();
}

void Scene::SetVisualizationProperties(const std::shared_ptr<Building>& building)
{
	ObjectProperties::SetVisualizationMesh(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(building->GetMesh().get())));
	ObjectProperties::SetVisualizationMeshReference(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(building->GetMesh().get())));
	ObjectProperties::SetVisualizationMeshPoints(building->GetMesh(),
		std::make_shared<chrono::fea::ChVisualizationFEAmesh>(*(building->GetMesh().get())));
}

void Scene::RequestClose()
{
	if (m_configureSystem) m_configureSystem->RequestClose();
}
