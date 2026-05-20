#include <System/ImGuiEventReceiver.h>

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
        if (event.KeyInput.Char != 0 && event.KeyInput.PressedDown)
        {
            io.AddInputCharacter(event.KeyInput.Char);
        }
        return io.WantCaptureKeyboard;
    }

    return false;
}
