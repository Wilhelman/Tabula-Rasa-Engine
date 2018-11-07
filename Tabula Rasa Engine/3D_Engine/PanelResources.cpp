#include "PanelResources.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
}

PanelResources::~PanelResources()
{}

void PanelResources::Draw()
{
	ImGui::Begin("Assets", &active, ImGuiWindowFlags_AlwaysAutoResize);




	ImGui::End();
}
