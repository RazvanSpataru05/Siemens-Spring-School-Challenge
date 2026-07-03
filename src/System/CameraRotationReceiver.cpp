#include "System/CameraRotationReceiver.h"

CameraRotationReceiver::CameraRotationReceiver(std::shared_ptr<chrono::irrlicht::ChIrrApp> application,
	bool* cameraAutoRotation) : 
	m_application{ application },
	m_cameraAutoRotation{cameraAutoRotation}
{
}

bool CameraRotationReceiver::OnEvent(const irr::SEvent& event)
{
	if (m_application == nullptr) return false;

	if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
	{
		if (event.KeyInput.Key == irr::KEY_KEY_A || event.KeyInput.Key == irr::KEY_KEY_D)
		{
			if (m_cameraAutoRotation != nullptr) *m_cameraAutoRotation = false;
		}

		irr::scene::ICameraSceneNode* camera = m_application->GetSceneManager()->getActiveCamera();
		if (camera == nullptr) return false;

		irr::core::vector3df pos = camera->getPosition();
		irr::core::vector3df target = camera->getTarget();

		irr::core::vector3df radiusVector = pos - target;
		const float angle = 3.0f;

		if (event.KeyInput.Key == irr::KEY_KEY_A)
		{
			RotateCameraAroundTarget(camera, -angle);
			return true;
		}
		if (event.KeyInput.Key == irr::KEY_KEY_D)
		{
			RotateCameraAroundTarget(camera, angle);
			return true;
		}
	}
	return false;
}

void CameraRotationReceiver::RotateCameraAroundTarget(irr::scene::ICameraSceneNode* camera, float angle)
{
	irr::core::vector3df pos = camera->getPosition();
	irr::core::vector3df target = camera->getTarget();

	irr::core::vector3df radiusVector = pos - target;
	radiusVector.rotateXZBy(angle);
	camera->setPosition(target + radiusVector);
	camera->updateAbsolutePosition();
}
