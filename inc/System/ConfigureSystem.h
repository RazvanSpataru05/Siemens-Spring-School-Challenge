#pragma once

#include <functional>
#include <atomic>

#include <Services/StyleService.h>
#include <Services/GAConfig.h>

#include <Selection/ISelectionStrategy.h>
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
#include <System/CameraRotationReceiver.h>

class ConfigureSystem
{
public:
	static constexpr float K_BASE_ROTATION_SPEED{ 0.05f };
	static const irr::core::vector3df K_INITIAL_POSITION;

public:
	ConfigureSystem(const std::shared_ptr<chrono::ChSystemSMC>& system);
	ConfigureSystem(const std::shared_ptr<chrono::irrlicht::ChIrrApp>& application,
		const std::shared_ptr<chrono::ChSystemSMC>& system);

	~ConfigureSystem() = default;

	void SetOnStartGA(std::function<void(const GAConfig&)> callback);
	void SetEpochNavigation(int currentEpochIndex, int epochCount, int bestFitnessEpochIndex,
		std::function<void(int)> onEpochChange);

	void SetSystemTimestepper();
	void SetSystemSolver();
	void SetupCamera();
	void Simulate(double duration);
	void RequestClose();

	void ConfigureIrrllichtScene();
	void InitializeIrrlichtScene();
	void SetIrrlichtSceneTimestep(double duration);
	void RunIrrlichtScene();
	void ManageKeyStrokes();

	void SetEpochStats(std::vector<int> removed, std::vector<double> stress, std::vector<double> fitness);
	void DrawCameraControlsUI();
	void DrawEpochNavigationUI();
	void DrawIndividualStatsPanel();

private:
	ConfigureSystem(const ConfigureSystem& other) = delete;
	ConfigureSystem(ConfigureSystem&& other) = delete;

	ConfigureSystem& operator=(const ConfigureSystem& other) = delete;
	ConfigureSystem& operator=(ConfigureSystem&& other) = delete;

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::shared_ptr<chrono::ChSystemSMC> m_system;

	std::function<void(const GAConfig&)> m_onStartGA;
	std::function<void(int)> m_onEpochChange;
	int m_currentEpochIndex{ 0 };
	int m_epochCount{ 0 };
	int m_bestFitnessEpochIndex{ -1 };
	std::atomic<bool> m_shouldClose{ false };

	std::vector<int> m_removedElementsPerEpoch;
	std::vector<double> m_maxStresPerEpoch;
	std::vector<double> m_fitnessPerEpoch;

	bool m_panelsVisible{ true };
	bool m_cameraAutoRotation{ false };
	float m_cameraRotationSpeed{ 1.0f };
};