#pragma once

#include "irrlicht.h"
#include "chrono_irrlicht/ChIrrApp.h"

class CameraRotationReceiver : public irr::IEventReceiver
{
public:
	CameraRotationReceiver(std::shared_ptr<chrono::irrlicht::ChIrrApp> application,
		bool* cameraAutoRotation);

	virtual bool OnEvent(const irr::SEvent& event);
	static void RotateCameraAroundTarget(irr::scene::ICameraSceneNode* camera, float angle);

private:
	std::shared_ptr<chrono::irrlicht::ChIrrApp> m_application;
	bool* m_cameraAutoRotation = nullptr;
};