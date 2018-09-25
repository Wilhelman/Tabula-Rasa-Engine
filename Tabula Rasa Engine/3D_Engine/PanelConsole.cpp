#include "PanelConsole.h"
#include "imgui.h"

PanelConsole::PanelConsole() : Panel("Console", SDL_SCANCODE_1)
{
	active = false;
}

PanelConsole::~PanelConsole()
{

}

void PanelConsole::Draw()
{
	ImGui::Begin("Console", &active,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse);

	ImGui::Text(text_buffer.begin());

	ImGui::End();
}

void PanelConsole::AddLogToConsole(const char * log)
{
	text_buffer.append(log);
}
