#include <System/ImGuiEventReceiver.h>

ImGuiEventReceiver::ImGuiEventReceiver(std::shared_ptr<chrono::irrlicht::ChIrrApp> application,
	bool* cameraAutoRotation) :
	m_cameraReceiver{ std::make_shared<CameraRotationReceiver>(application, cameraAutoRotation) }
{
}

bool ImGuiEventReceiver::OnEvent(const irr::SEvent& event)
{
	ImGuiIO& io = ImGui::GetIO();

	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case irr::EMIE_MOUSE_MOVED:
			io.AddMousePosEvent((float)event.MouseInput.X,
				(float)event.MouseInput.Y);
			break;
		case irr::EMIE_LMOUSE_PRESSED_DOWN:
			io.AddMouseButtonEvent(0, true);
			break;
		case irr::EMIE_LMOUSE_LEFT_UP:
			io.AddMouseButtonEvent(0, false);
			break;
		case irr::EMIE_RMOUSE_PRESSED_DOWN:
			io.AddMouseButtonEvent(1, true);
			break;
		case irr::EMIE_RMOUSE_LEFT_UP:
			io.AddMouseButtonEvent(1, false);
			break;
		case irr::EMIE_MMOUSE_PRESSED_DOWN:
			io.AddMouseButtonEvent(2, true);
			break;
		case irr::EMIE_MMOUSE_LEFT_UP:
			io.AddMouseButtonEvent(2, false);
			break;
		case irr::EMIE_MOUSE_WHEEL:
			io.AddMouseWheelEvent(0.0f, event.MouseInput.Wheel);
			break;
		default:
			break;
		}
		return io.WantCaptureMouse;
	}

	if (event.EventType == irr::EET_KEY_INPUT_EVENT)
	{
		ImGuiIO& io = ImGui::GetIO();

		ImGuiKey key = IrrlichtKeyToImGuiKey(event.KeyInput.Key);
		if (key != ImGuiKey_None)
		{
			io.AddKeyEvent(key, event.KeyInput.PressedDown);
		}

		if (event.KeyInput.Char != 0 && event.KeyInput.PressedDown)
		{
			io.AddInputCharacter(event.KeyInput.Char);
		}
		if (!io.WantCaptureKeyboard && m_cameraReceiver)
		{
			m_cameraReceiver->OnEvent(event);
		}
		return io.WantCaptureKeyboard;
	}
	return false;
}

ImGuiKey ImGuiEventReceiver::IrrlichtKeyToImGuiKey(irr::EKEY_CODE key)
{
	switch (key)
	{
	case irr::KEY_KEY_H: return ImGuiKey_H;
	case irr::KEY_KEY_R: return ImGuiKey_R;
	case irr::KEY_ESCAPE: return ImGuiKey_Escape;
	case irr::KEY_SPACE: return ImGuiKey_Space;
	default: return ImGuiKey_None;
	}
}
