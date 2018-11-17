#include "trApp.h"
#include "PanelResources.h"

#include "trFileLoader.h"
#include "trFileSystem.h"
#include "trTimeManager.h"
#include "trMainScene.h"
#include "trDefs.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
	active = false;
	refresh_clock = 0.0f;
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
		if (ImGui::TreeNodeEx(dir->files_vec[i].name.c_str(), ImGuiTreeNodeFlags_Leaf))
		{
			if (ImGui::IsItemClicked(0))
			{
				// TODO: show import config window
			}
			
			if (ImGui::IsItemClicked(1))  // right click
				ImGui::OpenPopup("Options");

			std::string extension;
			App->file_system->GetExtensionFromFile(dir->files_vec[i].name.c_str(), extension);

			if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemClicked(0) && extension.compare(".trScene") == 0)
				ImGui::OpenPopup("WARNING!");

			ImGui::SetNextWindowSize(ImVec2(800.0f, 120.0f));
			if (ImGui::BeginPopupModal("WARNING!", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
			{
				ImVec2 new_txt_pos(ImGui::GetWindowPos().x + 25.0f, ImGui::GetCursorScreenPos().y);
				ImGui::SetCursorScreenPos(new_txt_pos);
				ImGui::Text("Current scene '%s' will be lost. Are you sure you want to import '%s'", App->main_scene->scene_name.c_str(), dir->files_vec[i].name.c_str());
				
				ImGui::NewLine();

				ImVec2 new_btn_pos(ImGui::GetWindowPos().x + (800.0f / 2.0f - 200.0f / 2.0f), ImGui::GetCursorScreenPos().y);
				ImGui::SetCursorScreenPos(new_btn_pos);

				if (ImGui::Button("OK", ImVec2(120, 40)))
				{
					// TODO: import scene here (scene file is 'dir->files_vec[i]')

					const char* curr_file = dir->files_vec[i].name.c_str();
					// App->file_loader->ImportScene(curr_file); // TODO: this has mem leaks
					import_clicked = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::SameLine();
				if (ImGui::Button("CANCEL", ImVec2(120, 40)))
				{
					import_clicked = false;
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
			
			// TODO: delete this
			/*if (ImGui::BeginPopup("Options"))
			{
				if (ImGui::MenuItem("Import"))
					import_clicked = true;

				ImGui::EndPopup();
			}

			if (import_clicked)
			{
				ImGui::OpenPopup("WARNING");
				ImGui::SetNextWindowSize(ImVec2(750, 160));
				if (ImGui::BeginPopupModal("WARNING", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove))
				{
					ImGui::Text("Current scene %s will be lost. Are you sure you want to import %s", App->main_scene->scene_name.c_str(), dir->files_vec[i].name.c_str());
					ImGui::NewLine();
					
					if (ImGui::Button("OK", ImVec2(120, 40)))
					{
						// TODO: implement import cases
						std::string file_format(".xyz");
						const char* curr_file = dir->files_vec[i].name.c_str();
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

						import_clicked = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::SameLine();
					if (ImGui::Button("CANCEL", ImVec2(120, 40)))
					{
						import_clicked = false;
						ImGui::CloseCurrentPopup();
					}

					ImGui::EndPopup();
				}*/
			ImGui::TreePop();
			}
		}	
}