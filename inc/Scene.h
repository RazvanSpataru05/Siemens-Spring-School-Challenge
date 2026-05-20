#pragma once

#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#undef near
#undef far
#undef FAR

#include <Services/GAConfig.h>

#include <chrono/fea/ChLinkPointFrame.h>
#include <chrono/physics/ChSystemSMC.h>
#include <chrono_irrlicht/ChIrrApp.h>

#include <GraphicalObjects/Building.h>

class ConfigureSystem;

class Scene
{
public:
	Scene();
	~Scene() = default;

	void SetOnStartGA(std::function<void(const GAConfig&)> callback);
	void Show(const std::shared_ptr<Building>& building);
	void ShowEpochResults(const std::vector<std::shared_ptr<Building>>& epochBuildings,
		int bestFitnessEpochIndex, int startEpochIndex = -1);
	void RequestClose();
	void Shutdown();

	std::shared_ptr<Building> CreateCustomWallScene();

private:
	std::shared_ptr<Building> CreateSimpleWallScene();
	std::shared_ptr<Building> CreateComplexWallScene();

	void InitializeSystem(const std::shared_ptr<Building>& building, bool epochViewerMode = false);
	void SetVisualizationProperties(const std::shared_ptr<Building>& building);
	void LoadBuildingIntoHost(const std::shared_ptr<Building>& building);
	void SwitchToEpoch(int epochIndex);

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::unique_ptr<ConfigureSystem> m_configureSystem;
	std::shared_ptr<chrono::ChSystemSMC> m_system;
	std::function<void(const GAConfig&)> m_onStartGA;
	std::vector<std::shared_ptr<Building>> m_epochBuildings;
	int m_currentEpochIndex = 0;
	int m_bestFitnessEpochIndex = -1;
	bool m_epochViewerMode = false;
	bool m_initialized = false;
};