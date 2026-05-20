#pragma once

#include <functional>
#include <atomic>

#include <Services/StyleService.h>
#include <Services/GAConfig.h>
#include <Services/SelectionStrategy.h>

#include <Selection/RankedSelection.h>
#include <Selection/RouletteWheelSelection.h>
#include <Selection/StochasticUniversalSamplingSelection.h>
#include <Selection/TournamentSelection.h>

#include <chrono/physics/ChSystemSMC.h>
#include <chrono/solver/ChIterativeSolverLS.h>

#include <chrono_irrlicht/ChIrrApp.h>

#include <Imgui/imgui.h>
#include <Imgui/backends/imgui_impl_dx9.h>
#include <d3d9.h>

#include <System/ImGuiEventReceiver.h>

class ConfigureSystem
{
public:
	ConfigureSystem(const std::shared_ptr<chrono::ChSystemSMC>& system);
	ConfigureSystem(const std::shared_ptr<chrono::irrlicht::ChIrrApp>& application,
		const std::shared_ptr<chrono::ChSystemSMC>& system);

	ConfigureSystem(const ConfigureSystem& other) = delete;
	ConfigureSystem(ConfigureSystem&& other) = delete;

	ConfigureSystem& operator=(const ConfigureSystem& other) = delete;
	ConfigureSystem& operator=(ConfigureSystem&& other) = delete;

	~ConfigureSystem() = default;

	void SetOnStartGA(std::function<void(const GAConfig&)> callback);
	void SetEpochNavigation(int currentEpochIndex, int epochCount, int bestFitnessEpochIndex,
		std::function<void(int)> onEpochChange);

	void SetSystemTimestepper();
	void SetSystemSover();
	void Simulate(double duration);
	void RequestClose();

	void ConfigureIrrllichtScene();
	void InitializeIrrlichtScene();
	void SetIrrlichtSceneTimestep(double duration);
	void RunIrrlichtScene();

	void DrawEpochNavigationUI();

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::shared_ptr<chrono::ChSystemSMC> m_system;

	std::function<void(const GAConfig&)> m_onStartGA;
	std::function<void(int)> m_onEpochChange;
	int m_currentEpochIndex = 0;
	int m_epochCount = 0;
	int m_bestFitnessEpochIndex = -1;
	std::atomic<bool> m_shouldClose{ false };
};