#pragma once

#include <chrono/fea/ChLinkPointFrame.h>
#include <chrono/physics/ChSystemSMC.h>
#include <chrono_irrlicht/ChIrrApp.h>

#include "Services/GAConfig.h"
#include "Services/AlgorithmSettings.h"

#include "GeneticAlgorithm/Individual.h"

#include "GraphicalObjects/Building.h"

#include "System/ConfigureSystem.h"

class ConfigureSystem;

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void SetOnStartGA(std::function<void(const GAConfig&)> callback);
	void ShowInitialBuilding(const std::shared_ptr<Building>& building);
	void ShowEpochResults(const std::vector<std::vector<bool>>& epochGenes,
		int bestFitnessEpochIndex, int startEpochIndex = -1);
	void RequestClose();
	void Shutdown();
	void SetEpochStats(std::vector<int> removed, std::vector<double> stress, std::vector<double> fitness);

private:

	void InitializeSystem(const std::shared_ptr<Building>& building, bool epochViewerMode = false);
	void SetVisualizationProperties(const std::shared_ptr<Building>& building);
	void LoadBuildingIntoHost(const std::vector<bool>& cubesExistence);
	void SwitchToEpoch(int epochIndex);

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::unique_ptr<ConfigureSystem> m_configureSystem;
	std::shared_ptr<chrono::ChSystemSMC> m_system;
	std::function<void(const GAConfig&)> m_onStartGA;
	std::vector<std::vector<bool>> m_epochGenes;

	int m_currentEpochIndex{ 0 };
	int m_bestFitnessEpochIndex{ -1 };
	bool m_epochViewerMode{ false };
	bool m_initialized{ false };

	std::vector<int> m_cachedRemoved;
	std::vector<double> m_cachedStress;
	std::vector<double> m_cachedFitness;
};