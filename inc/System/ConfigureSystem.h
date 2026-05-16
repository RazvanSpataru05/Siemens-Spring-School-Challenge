#pragma once

#include <chrono/physics/ChSystemSMC.h>
#include <chrono/solver/ChIterativeSolverLS.h>

#include <chrono_irrlicht/ChIrrApp.h>

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

	void SetSystemTimestepper();
	void SetSystemSover();
	void Simulate(double duration);

	void ConfigureIrrllichtScene();
	void InitializeIrrlichtScene();
	void SetIrrlichtSceneTimestep(double duration);
	void RunIrllichtScene();

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	std::shared_ptr<chrono::ChSystemSMC> m_system;
};