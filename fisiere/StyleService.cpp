#include "Services/StyleService.h"

void StyleService::ApplyTheme()
{
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	const ImVec4 boldPetrol = ImVec4(0.00f, 0.39f, 0.43f, 1.00f);
	const ImVec4 brightPetrol = ImVec4(0.00f, 0.60f, 0.65f, 1.00f);
	const ImVec4 highlight = ImVec4(0.00f, 1.00f, 0.73f, 1.00f);
	const ImVec4 highlightDim = ImVec4(0.00f, 0.78f, 0.57f, 1.00f);
	const ImVec4 bg = ImVec4(0.07f, 0.08f, 0.10f, 0.95f);
	const ImVec4 panel = ImVec4(0.12f, 0.14f, 0.16f, 1.00f);
	const ImVec4 panelHover = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
	const ImVec4 border = ImVec4(0.20f, 0.30f, 0.32f, 1.00f);
	const ImVec4 text = ImVec4(0.92f, 0.94f, 0.94f, 1.00f);
	const ImVec4 textDim = ImVec4(0.60f, 0.65f, 0.66f, 1.00f);

	colors[ImGuiCol_Text] = text;
	colors[ImGuiCol_TextDisabled] = textDim;
	colors[ImGuiCol_WindowBg] = bg;
	colors[ImGuiCol_Border] = border;
	colors[ImGuiCol_FrameBg] = panel;
	colors[ImGuiCol_FrameBgHovered] = panelHover;
	colors[ImGuiCol_FrameBgActive] = brightPetrol;
	colors[ImGuiCol_TitleBg] = boldPetrol;
	colors[ImGuiCol_TitleBgActive] = boldPetrol;
	colors[ImGuiCol_TitleBgCollapsed] = brightPetrol;
	colors[ImGuiCol_Button] = boldPetrol;
	colors[ImGuiCol_ButtonHovered] = brightPetrol;
	colors[ImGuiCol_ButtonActive] = highlight;
	colors[ImGuiCol_Header] = boldPetrol;
	colors[ImGuiCol_HeaderHovered] = brightPetrol;
	colors[ImGuiCol_HeaderActive] = highlightDim;
	colors[ImGuiCol_SliderGrab] = highlightDim;
	colors[ImGuiCol_SliderGrabActive] = highlight;
	colors[ImGuiCol_CheckMark] = highlight;
	colors[ImGuiCol_PopupBg] = panel;
	colors[ImGuiCol_ScrollbarBg] = bg;
	colors[ImGuiCol_ScrollbarGrab] = boldPetrol;
	colors[ImGuiCol_ScrollbarGrabHovered] = brightPetrol;
	colors[ImGuiCol_ScrollbarGrabActive] = highlight;
	colors[ImGuiCol_Separator] = border;
	colors[ImGuiCol_SeparatorHovered] = brightPetrol;
	colors[ImGuiCol_SeparatorActive] = highlight;

	style.WindowRounding = 4.0f;
	style.FrameRounding = 3.0f;
	style.GrabRounding = 3.0f;
	style.ScrollbarRounding = 3.0f;
	style.WindowPadding = ImVec2(12, 10);
	style.FramePadding = ImVec2(8, 5);
	style.ItemSpacing = ImVec2(8, 6);
}
