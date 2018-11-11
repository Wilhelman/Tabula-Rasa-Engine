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


}

PanelResources::~PanelResources()
{}

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);

	if (App->file_system->DoesDirExist(ASSETS_DIR))
	{
		if (ImGui::TreeNodeEx(ASSETS_DIR))
		{
			DrawAssets(ASSETS_DIR);
			ImGui::TreePop();
		}
	}
	ImGui::End();
}

void PanelResources::DrawAssets(std::string dir_name)
{
	std::list<std::string> file_list;
	std::list<std::string> dir_list;

	std::string assets_dir(dir_name);
	assets_dir.append("/");
	App->file_system->GetFilesFromDir(assets_dir.c_str(), file_list, dir_list);

	for (std::list<std::string>::iterator it_dir = dir_list.begin(); it_dir != dir_list.end(); it_dir++)
	{
		if (ImGui::TreeNodeEx((*it_dir).c_str()))
		{
			std::string tmp_dir = assets_dir;
			tmp_dir.append((*it_dir).c_str());
			DrawAssets(tmp_dir);
			ImGui::TreePop();
		}
	}

	for (std::list<std::string>::iterator it_file = file_list.begin(); it_file != file_list.end(); it_file++)
	{
		if (ImGui::TreeNodeEx((*it_file).c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked()) 
			{
				if (ImGui::IsItemClicked(0))
				{
					// TODO: show import config window
				}
			}

			ImGui::TreePop();
		}
	}
}