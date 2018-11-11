#include "trApp.h"
#include "PanelResources.h"
#include "trFileSystem.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
	active = false;
}

PanelResources::~PanelResources()
{}

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);

	// TODO: check if dir assets exists before doing anything
	if (ImGui::TreeNodeEx("Assets"))
	{
		char** rc = App->file_system->GetFilesFromDir("Assets");
		for (char** i = rc; *i != nullptr; i++)
			DrawAssets(*i);

		ImGui::TreePop();
	}

	ImGui::End();
}

void PanelResources::DrawAssets(std::string asset_name)
{
	if (ImGui::TreeNodeEx(asset_name.c_str()))
	{
		
		ImGui::TreePop();
	}
}