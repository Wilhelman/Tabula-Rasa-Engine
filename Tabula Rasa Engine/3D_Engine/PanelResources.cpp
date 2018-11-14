#include "trApp.h"
#include "PanelResources.h"

#include "trFileLoader.h"
#include "trFileSystem.h"
#include "trTimeManager.h"
#include "trDefs.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
	active = false;
	refresh_clock = REFRESH_TIME;
}

PanelResources::~PanelResources() { }

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::TreeNodeEx(ASSETS_DIR))
	{
		DrawAssets(App->file_system->GetAssetsDirectory());
		ImGui::TreePop();
	}
	
	ImGui::End();

	if (refresh_clock >= REFRESH_TIME)
	{
		App->file_system->ClearAssetsDir();
		App->file_system->RefreshDirectory(ASSETS_DIR);
		refresh_clock = 0.0f;
	}

	refresh_clock += App->time_manager->GetRealTimeDt();
}

void PanelResources::DrawAssets(Directory* dir)
{
	uint dirs_size = dir->dirs_vec.size();
	uint files_size = dir->files_vec.size();

	for (uint i = 0; i < dirs_size; i++)
	{
		if (ImGui::TreeNodeEx(dir->dirs_vec[i].name.c_str()))
		{
			DrawAssets(&dir->dirs_vec[i]);
			ImGui::TreePop();
		}
	}

	for (uint i = 0; i < files_size; i++)
	{
		if (ImGui::TreeNodeEx(dir->files_vec[i].c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked(0))
			{
				// TODO: show import config window
			}
			if (ImGui::IsItemClicked(1))  // right click
				ImGui::OpenPopup("Options");

			if (ImGui::BeginPopup("Options"))
			{
				if (ImGui::MenuItem("Import"))
				{
					std::string file_format(".xyz");
					const char* curr_file = dir->files_vec[i].c_str();
					file_format[3] = curr_file[strlen(curr_file) - 1];
					file_format[2] = curr_file[strlen(curr_file) - 2];
					file_format[1] = curr_file[strlen(curr_file) - 3];
					file_format[0] = curr_file[strlen(curr_file) - 4];

					if (file_format.compare(".fbx") == 0 || file_format.compare(".FBX") == 0)
						App->file_loader->ImportFBX(curr_file);
					else if (file_format.compare("cene") == 0 || file_format.compare("CENE") == 0)
						App->file_loader->ImportScene(curr_file);
					else if (file_format.compare(".dds") == 0 || file_format.compare(".DDS") == 0 ||
							 file_format.compare(".jpg") == 0 || file_format.compare(".JPG") == 0 ||
							 file_format.compare("jpeg") == 0 || file_format.compare("JPEG") == 0 ||
							 file_format.compare(".png") == 0 || file_format.compare(".PNG") == 0)
						App->file_loader->ImportTexture(curr_file);
					else
						TR_LOG("Resources: Error importing unknown format...");
				}

				if (ImGui::MenuItem("Remove"))
				{
					// TODO: remove file here
				}
				ImGui::EndPopup();
			}
			ImGui::TreePop();
		}
	}
}