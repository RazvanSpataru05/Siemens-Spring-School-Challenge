#pragma once
#include <Imgui/imgui.h>

#include <chrono_irrlicht/ChIrrApp.h>

class ImGuiEventReceiver : public irr::IEventReceiver
{
public:
	bool OnEvent(const irr::SEvent& event) override;

private:
	ImGuiKey IrrlichtKeyToImGuiKey(irr::EKEY_CODE key);
};
