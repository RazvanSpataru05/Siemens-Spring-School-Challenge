#pragma once

#include <thread>
#include <atomic>
#include <functional>

#include "System/Scene.h"

#include "Services/GAResult.h"
#include "Services/GAConfig.h"
#include "Services/SelectionStrategyFactory.h"
#include "Services/AlgorithmSettings.h"
#include "Services/IOIndividualManager.h"

#include "GeneticAlgorithm/UtilFunctions.h"
#include "GeneticAlgorithm/GeneticAlgorithm.h"

class Application
{
public:
	explicit Application();

	void Run();

private:
	void StartGA(const GAConfig& config);

private:
	Scene m_scene;
	std::jthread m_gaThread;
	std::atomic<bool> m_gaRunning{ false };
	GAResult m_result;

	bool m_hasResult{ false };

	int m_oxSize{};
	int m_oySize{};
	int m_ozSize{};

	double m_elementSize{};

	std::vector<bool> m_cubesExistence;
	std::shared_ptr<Building> m_initialBuilding;
};

