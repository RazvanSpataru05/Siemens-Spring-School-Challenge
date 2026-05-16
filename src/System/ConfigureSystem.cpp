#include <System/ConfigureSystem.h>

ConfigureSystem::ConfigureSystem(const std::shared_ptr<chrono::ChSystemSMC>& system):
	m_system{ system }
{
}

ConfigureSystem::ConfigureSystem(const std::shared_ptr<chrono::irrlicht::ChIrrApp>& application,
	const std::shared_ptr<chrono::ChSystemSMC>& system) :
	m_application{ application }, m_system{ system }
{
}

void ConfigureSystem::SetSystemTimestepper()
{
	m_system->SetTimestepperType(chrono::ChTimestepper::Type::EULER_IMPLICIT_LINEARIZED);
}

void ConfigureSystem::SetSystemSover()
{
	m_system->SetSolver(std::make_shared<chrono::ChSolverMINRES>());
}

void ConfigureSystem::Simulate(double duration)
{
	m_system->DoEntireDynamics(duration);
}

void ConfigureSystem::ConfigureIrrllichtScene()
{
	m_application->AddTypicalLogo();
	m_application->AddTypicalSky();
	m_application->AddTypicalLights();
	m_application->AddTypicalCamera(irr::core::vector3df(3.5, (irr::f32)0.5, -3.0));
}

void ConfigureSystem::InitializeIrrlichtScene()
{
	m_application->AssetBindAll();
	m_application->AssetUpdateAll();
}

void ConfigureSystem::SetIrrlichtSceneTimestep(double duration)
{
	m_application->SetTimestep(duration);
}

void ConfigureSystem::RunIrllichtScene()
{
	while (m_application->GetDevice()->run())
	{
		m_application->BeginScene();
		m_application->DrawAll();
		m_application->DoStep();
		m_application->EndScene();
	}
}
