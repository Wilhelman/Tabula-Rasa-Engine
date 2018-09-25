#include "PanelConsole.h"
#include "imgui.h"

void PanelConsole::Draw()
{
	ImGui::Begin("About Tabula Rasa Engine", &active,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse);


	ImGui::End();
}
