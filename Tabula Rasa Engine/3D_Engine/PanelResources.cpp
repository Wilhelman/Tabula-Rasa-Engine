#include "trApp.h"
#include "PanelResources.h"
#include "trFileSystem.h"
#include "trDefs.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
	active = false;

	if (App->file_system->DoesDirExist(ASSETS_DIR))
	{
		std::string assets_dir(ASSETS_DIR);
		assets_dir.append("/");
		App->file_system->GetFilesFromDir(assets_dir.c_str(), file_list, dir_list);
	}
}

PanelResources::~PanelResources()
{}

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);

		if (ImGui::TreeNodeEx(ASSETS_DIR))
		{
			for (std::list<std::string>::iterator it_dir = dir_list.begin(); it_dir != dir_list.end(); it_dir++)
			{
				if (ImGui::TreeNodeEx((*it_dir).c_str()))
				{

					ImGui::TreePop();
				}
			}
			ImGui::TreePop();
		}	
	ImGui::End();
}

void PanelResources::DrawAssets(std::string asset_name)
{

}