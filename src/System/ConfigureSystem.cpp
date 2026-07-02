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
            ImGui::Begin("Genetic Algorithm Configuration", nullptr, ImGuiWindowFlags_NoMove);

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
            DrawIndividualStatsPanel();
        }

        ImGui::Render();
        ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

        m_application->EndScene();
    }

    ImGui_ImplDX9_Shutdown();
    ImGui::DestroyContext();
}

void ConfigureSystem::SetEpochStats(std::vector<int> removed, std::vector<double> stress, std::vector<double> fitness)
{
    m_removedElementsPerEpoch = std::move(removed);
    m_maxStresPerEpoch = std::move(stress);
    m_fitnessPerEpoch = std::move(fitness);
}

void ConfigureSystem::DrawEpochNavigationUI()
{
    const ImVec2 display = ImGui::GetIO().DisplaySize;
    const float buttonWidth = 92.0f;
    const float buttonHeight = 44.0f;
    const float spacing = 6.0f;
    const float margin = 24.0f;
    const float bottomY = display.y - buttonHeight - margin;

    const ImVec4 brightPetrol = ImVec4(0.00f, 0.60f, 0.65f, 1.00f);
    const ImVec4 highlight = ImVec4(0.00f, 1.00f, 0.73f, 1.00f);
    const ImVec4 highlightDim = ImVec4(0.00f, 0.78f, 0.57f, 1.00f);
    const ImVec4 textColor = ImVec4(0.95f, 0.97f, 0.97f, 1.00f);

    const bool isBestEpoch = m_bestFitnessEpochIndex >= 0 && m_currentEpochIndex == m_bestFitnessEpochIndex;
    const ImVec4 goldText = ImVec4(1.00f, 0.84f, 0.00f, 1.00f);
    const ImVec4 goldWindowBg = ImVec4(0.22f, 0.17f, 0.04f, 0.92f);
    const ImVec4 goldBorder = ImVec4(0.95f, 0.78f, 0.12f, 0.95f);
    const ImVec4 defaultBg = ImVec4(0.06f, 0.08f, 0.10f, 0.88f);
    const ImVec4 defaultBorder = ImVec4(0.00f, 0.60f, 0.65f, 0.70f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, isBestEpoch ? goldWindowBg : defaultBg);
    ImGui::PushStyleColor(ImGuiCol_Border, isBestEpoch ? goldBorder : defaultBorder);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(14.0f, 10.0f));

    ImGui::SetNextWindowPos(ImVec2(display.x - 220.0f, 18.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(isBestEpoch ? 200.0f : 180.0f, 52.0f), ImGuiCond_Always);
    ImGui::Begin("EpochIndicator", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);
    if (isBestEpoch)
        ImGui::TextColored(goldText, "Epoch %d / %d (best)", m_currentEpochIndex + 1, m_epochCount);
    else
        ImGui::Text("Epoch %d / %d", m_currentEpochIndex + 1, m_epochCount);
    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);

    ImGui::PushStyleColor(ImGuiCol_Button, brightPetrol);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, highlight);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, highlightDim);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10.0f, 8.0f));

    const ImGuiWindowFlags navFlags =
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBackground;

    const bool canGoPrev = m_currentEpochIndex > 0;
    const bool canGoNext = m_currentEpochIndex < m_epochCount - 1;
    const float clusterWidth = 3 * buttonWidth + 2 * spacing + 16.0f;

    ImGui::SetNextWindowPos(ImVec2(margin, bottomY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(clusterWidth, buttonHeight + 16.0f), ImGuiCond_Always);
    ImGui::Begin("EpochNavLeft", nullptr, navFlags);

    if (!canGoPrev) ImGui::BeginDisabled();
    if (ImGui::Button("|< First", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(0);
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("<<", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(std::max(0, m_currentEpochIndex - 10));
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("< Prev", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(m_currentEpochIndex - 1);
    if (!canGoPrev) ImGui::EndDisabled();

    ImGui::End();

    ImGui::SetNextWindowPos(ImVec2(display.x - clusterWidth - margin, bottomY), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(clusterWidth, buttonHeight + 16.0f), ImGuiCond_Always);
    ImGui::Begin("EpochNavRight", nullptr, navFlags);

    if (!canGoNext) ImGui::BeginDisabled();
    if (ImGui::Button("Next >", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(m_currentEpochIndex + 1);
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button(">>", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(std::min(m_epochCount - 1, m_currentEpochIndex + 10));
    ImGui::SameLine(0.0f, spacing);
    if (ImGui::Button("Last >|", ImVec2(buttonWidth, buttonHeight)))
        m_onEpochChange(m_epochCount - 1);
    if (!canGoNext) ImGui::EndDisabled();

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(4);
}

void ConfigureSystem::DrawIndividualStatsPanel()
{
    if (m_currentEpochIndex < 0 || m_currentEpochIndex >= m_epochCount) return;
    if (m_removedElementsPerEpoch.empty()) return;

    const ImVec2 display = ImGui::GetIO().DisplaySize;
    const float panelWidth = 720.0f;
    const float panelHeight = 70.0f;

    const ImVec4 panelBg = ImVec4(0.06f, 0.08f, 0.10f, 0.92f);
    const ImVec4 border = ImVec4(0.00f, 0.60f, 0.65f, 0.85f); 
    const ImVec4 labelColor = ImVec4(0.00f, 1.00f, 0.73f, 1.00f);  
    const ImVec4 valueColor = ImVec4(0.95f, 0.97f, 0.97f, 1.00f);  
    const ImVec4 separator = ImVec4(0.00f, 0.60f, 0.65f, 0.40f);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, panelBg);
    ImGui::PushStyleColor(ImGuiCol_Border, border);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(22.0f, 14.0f));

    ImGui::SetNextWindowPos(ImVec2((display.x - panelWidth) * 0.5f, 18.0f), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(panelWidth, panelHeight), ImGuiCond_Always);
    ImGui::Begin("IndividualStats", nullptr,
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar);

    const int removed = m_removedElementsPerEpoch[m_currentEpochIndex];
    const double stress = m_maxStresPerEpoch[m_currentEpochIndex];
    const double fitness = m_fitnessPerEpoch[m_currentEpochIndex];

    ImGui::TextColored(labelColor, "Removed elements:");
    ImGui::SameLine();
    ImGui::TextColored(valueColor, "%d", removed);

    ImGui::SameLine(panelWidth * 0.33f);
    ImGui::TextColored(separator, "|");
    ImGui::SameLine();

    ImGui::TextColored(labelColor, "Max stress:");
    ImGui::SameLine();
    ImGui::TextColored(valueColor, "%.2f Pa", stress);

    ImGui::SameLine(panelWidth * 0.66f);
    ImGui::TextColored(separator, "|");
    ImGui::SameLine();

    ImGui::TextColored(labelColor, "Fitness:");
    ImGui::SameLine();
    ImGui::TextColored(valueColor, "%.2f", fitness);

    ImGui::End();

    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor(2);
}
