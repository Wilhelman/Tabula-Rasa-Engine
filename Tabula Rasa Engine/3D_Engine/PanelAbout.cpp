#include "PanelAbout.h"

#include "imgui.h"

// ---------------------------------------------------------
PanelAbout::PanelAbout() : Panel("About")
{
	active = false;
}

// ---------------------------------------------------------
PanelAbout::~PanelAbout()
{}

// ---------------------------------------------------------
void PanelAbout::Draw()
{
	ImGui::Begin("About Tabula Rasa Engine", &active,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Version %s", VERSION);
	ImGui::Separator();
	ImGui::Text("By Guillermo Garcia Subirana and Victor Maso Garcia.");
	ImGui::Text("Tabula Rasa Engine is licensed under the Public Domain, see LICENSE for more information.");
	ImGui::End();
}
