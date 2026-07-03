#pragma once
#include <Imgui/imgui.h>

#include "System/CameraRotationReceiver.h"

#include <chrono_irrlicht/ChIrrApp.h>

class ImGuiEventReceiver : public irr::IEventReceiver
{
public:
	ImGuiEventReceiver(std::shared_ptr<chrono::irrlicht::ChIrrApp> application,
		bool* cameraAutoRotation);

	bool OnEvent(const irr::SEvent& event) override;

private:
	ImGuiKey IrrlichtKeyToImGuiKey(irr::EKEY_CODE key);

private:
	std::shared_ptr<CameraRotationReceiver> m_cameraReceiver;
};
