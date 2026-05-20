#include <System/ConfigureSystem.h>

ConfigureSystem::ConfigureSystem(const std::shared_ptr<chrono::ChSystemSMC>& system) :
	m_system{ system }
{
}

ConfigureSystem::ConfigureSystem(const std::shared_ptr<chrono::irrlicht::ChIrrApp>& application,
	const std::shared_ptr<chrono::ChSystemSMC>& system) :
	m_application{ application }, m_system{ system }
{
}

void ConfigureSystem::SetOnStartGA(std::function<void(const GAConfig&)> callback)
{
    m_onStartGA = std::move(callback);
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

void ConfigureSystem::RequestClose()
{
    m_shouldClose = true;
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

void ConfigureSystem::RunIrrlichtScene()
{
    irr::video::SExposedVideoData videoData = m_application->GetDevice()->getVideoDriver()->getExposedVideoData();
    LPDIRECT3DDEVICE9 d3dDevice = static_cast<LPDIRECT3DDEVICE9>(videoData.D3D9.D3DDev9);
    ImGuiEventReceiver receiver;
    m_application->GetDevice()->setEventReceiver(&receiver);

    irr::core::dimension2d<irr::u32> screenSize = m_application->GetDevice()->getVideoDriver()->getScreenSize();
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplDX9_Init(d3dDevice);
    StyleService::ApplyTheme();

    while (m_application->GetDevice()->run())
    {
        if (m_shouldClose.load())
        {
            m_application->GetDevice()->closeDevice();
            break;
        }
        static int frame2 = 0;
        if (frame2 < 3) std::cout << "[2] rendering frame " << frame2 << "\n";
            
        ImGui_ImplDX9_NewFrame();
        ImGui::GetIO().DisplaySize = ImVec2((float)screenSize.Width, (float)screenSize.Height);
        ImGui::NewFrame();

        m_application->BeginScene();
        m_application->DrawAll();

        ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
        ImGui::SetNextWindowSize(ImVec2(500, 250), ImGuiCond_FirstUseEver);
        ImGui::Begin("Genetic Algorithm Confiugration", nullptr, ImGuiWindowFlags_NoMove);

        static int epochs = 5;
        ImGui::SliderInt("Epochs", &epochs, 5, 1000);

        static int population = 50;
        ImGui::SliderInt("Population", &population, 50, 1000);

        static float crossoverProbability = 0.2;
        ImGui::SliderFloat("Crossover Probability", &crossoverProbability, 0.2f, 0.6f, "%.2f");

        static float mutationProbability = 0.01;
        ImGui::SliderFloat("Mutation Probability", &mutationProbability, 0.01f, 0.20f, "%.2f");

        const char* fitness[] = { "Default" };
        static int fitnessMethod = 0;
        ImGui::Combo("Fitness", &fitnessMethod, fitness, 1);

        const char* selection[] = { "Roulette Wheel", "Tournament", "Ranked", "Stochastic Universal Sampling" };
        static int selectedMethod = 0;
        ImGui::Combo("Selection", &selectedMethod, selection, 4);

        const char* crossover[] = { "3D Block", "2 Point Planar", "Symmetry Forced" };
        static int crossoverMethod = 0;
        ImGui::Combo("Crossover", &crossoverMethod, crossover, 3);
        
        if (ImGui::Button("Start Genetic Algorithm"))
        {
            if (m_onStartGA)
            {
                GAConfig config;
                config.epochs = epochs;
                config.population = population;
                config.selectionMethod = selectedMethod;
                config.crossoverMethod = crossoverMethod;
                config.fitnessMethod = fitnessMethod;
                config.crossoverProbability = crossoverProbability;
                config.mutationProbability = mutationProbability;   
                m_onStartGA(config);
            }
            ImGui::SetWindowCollapsed(true);
        }
        ImGui::End();

        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        m_application->EndScene();
    }
    std::cout << "[2] while exited, m_shouldClose=" << m_shouldClose.load() << "\n";

    ImGui_ImplDX9_Shutdown();
    ImGui::DestroyContext();
}
