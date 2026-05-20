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

void ConfigureSystem::SetEpochNavigation(int currentEpochIndex, int epochCount, int bestFitnessEpochIndex,
    std::function<void(int)> onEpochChange)
{
    m_currentEpochIndex = currentEpochIndex;
    m_epochCount = epochCount;
    m_bestFitnessEpochIndex = bestFitnessEpochIndex;
    m_onEpochChange = std::move(onEpochChange);
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
            
        ImGui_ImplDX9_NewFrame();
        ImGui::GetIO().DisplaySize = ImVec2((float)screenSize.Width, (float)screenSize.Height);
        ImGui::NewFrame();

        m_application->BeginScene();
        m_application->DrawAll();

        if (m_onStartGA)
        {
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

            const char* fitness[] = { "Original Fitness Function", "Efficiency Ratio Fitness Function", 
                "Exponential Penalty Fitness Function", "Multi Objective Fitness Function"};
            static int fitnessMethod = 0;
            ImGui::Combo("Fitness", &fitnessMethod, fitness, 4);

            const char* selection[] = { "Roulette Wheel", "Tournament", "Ranked", "Stochastic Universal Sampling" };
            static int selectedMethod = 0;
            ImGui::Combo("Selection", &selectedMethod, selection, 4);

            const char* crossover[] = { "Single Point Crossover", "3D Block", "2 Point Planar", "Symmetry Forced" };
            static int crossoverMethod = 0;
            ImGui::Combo("Crossover", &crossoverMethod, crossover, 4);
            
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
        }

        if (m_onEpochChange && m_epochCount > 0)
        {
            DrawEpochNavigationUI();
        }

        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        m_application->EndScene();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui::DestroyContext();
}

void ConfigureSystem::DrawEpochNavigationUI()
{
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    const float buttonSize = 56.0f;
    const float margin = 28.0f;

    const bool isBestEpoch = m_bestFitnessEpochIndex >= 0 && m_currentEpochIndex == m_bestFitnessEpochIndex;
    const ImVec4 goldText = ImVec4(1.0f, 0.84f, 0.0f, 1.0f);
    const ImVec4 goldWindowBg = ImVec4(0.22f, 0.17f, 0.04f, 0.92f);
    const ImVec4 goldBorder = ImVec4(0.95f, 0.78f, 0.12f, 0.95f);
    const ImVec4 defaultWindowBg = ImVec4(0.08f, 0.10f, 0.14f, 0.82f);
    const ImVec4 defaultBorder = ImVec4(0.35f, 0.55f, 0.95f, 0.65f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, isBestEpoch ? goldWindowBg : defaultWindowBg);
    ImGui::PushStyleColor(ImGuiCol_Border, isBestEpoch ? goldBorder : defaultBorder);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 10.0f));

    ImGui::SetNextWindowPos(ImVec2(display.x - 210.0f, 18.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(isBestEpoch ? 192.0f : 172.0f, 52.0f), ImGuiCond_Always);
    ImGui::Begin("EpochIndicator", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    if (isBestEpoch)
    {
        ImGui::TextColored(goldText, "Epoch %d / %d (best)", m_currentEpochIndex + 1, m_epochCount);
    }
    else
    {
        ImGui::Text("Epoch %d / %d", m_currentEpochIndex + 1, m_epochCount);
    }
    ImGui::End();

    const ImGuiWindowFlags navFlags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

    ImGui::SetNextWindowPos(ImVec2(margin, display.y - buttonSize - margin), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(buttonSize, buttonSize), ImGuiCond_Always);
    ImGui::Begin("EpochNavLeft", nullptr, navFlags);
    const bool canGoPrevious = m_currentEpochIndex > 0;
    if (!canGoPrevious)
    {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button("<##prev", ImVec2(buttonSize - 8.0f, buttonSize - 8.0f)) && canGoPrevious)
    {
        m_onEpochChange(m_currentEpochIndex - 1);
    }
    if (!canGoPrevious)
    {
        ImGui::EndDisabled();
    }
    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(display.x - buttonSize - margin, display.y - buttonSize - margin), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(buttonSize, buttonSize), ImGuiCond_Always);
    ImGui::Begin("EpochNavRight", nullptr, navFlags);
    const bool canGoNext = m_currentEpochIndex < m_epochCount - 1;
    if (!canGoNext)
    {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button(">##next", ImVec2(buttonSize - 8.0f, buttonSize - 8.0f)) && canGoNext)
    {
        m_onEpochChange(m_currentEpochIndex + 1);
    }
    if (!canGoNext)
    {
        ImGui::EndDisabled();
    }
    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}
