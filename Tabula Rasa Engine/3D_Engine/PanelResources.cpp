#include "trApp.h"
#include "PanelResources.h"

#include "trFileLoader.h"
#include "trDefs.h"

#include "ImGui/imgui.h"

PanelResources::PanelResources() : Panel("Assets", SDL_SCANCODE_4)
{
	width = 500;
	height = 500;
	active = false;

	std::vector<trFileSystem::Directory> d_vec;
	std::vector<std::string> f_vec;
	//new_dir = new trFileSystem::Directory("Assets", d_vec, f_vec);

	App->file_system->GetFilesFromDir("Assets");

}

PanelResources::~PanelResources()
{
	//RELEASE(new_dir);
}

void PanelResources::Draw()
{
	ImGui::Begin("Resources", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing |
		ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::TreeNodeEx(ASSETS_DIR))
	{
		DrawAssets(ASSETS_DIR);
		ImGui::TreePop();
	}
	
	ImGui::End();
}

void PanelResources::DrawAssets(std::string dir_name)
{
	std::list<std::string> file_list;
	std::list<std::string> dir_list;

	std::string assets_dir(dir_name);
	assets_dir.append("/");
	App->file_system->GetFilesFromDirOld(assets_dir.c_str(), file_list, dir_list);

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
					file_format[3] = (*it_file).c_str()[strlen((*it_file).c_str()) - 1];
					file_format[2] = (*it_file).c_str()[strlen((*it_file).c_str()) - 2];
					file_format[1] = (*it_file).c_str()[strlen((*it_file).c_str()) - 3];
					file_format[0] = (*it_file).c_str()[strlen((*it_file).c_str()) - 4];

					if (file_format.compare(".fbx") == 0 || file_format.compare(".FBX") == 0)
						App->file_loader->ImportFBX((*it_file).c_str());
					else if (file_format.compare("cene") == 0 || file_format.compare("CENE") == 0)
						App->file_loader->ImportScene((*it_file).c_str());
					else if (file_format.compare(".dds") == 0 || file_format.compare(".DDS") == 0 ||
						     file_format.compare(".jpg") == 0 || file_format.compare(".JPG") == 0 ||
						     file_format.compare("jpeg") == 0 || file_format.compare("JPEG") == 0 ||
						     file_format.compare(".png") == 0 || file_format.compare(".PNG") == 0)
						     App->file_loader->ImportTexture((*it_file).c_str());
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

void PanelResources::FillFilesTree(std::string dir_name)
{
	std::list<std::string> file_list;
	std::list<std::string> dir_list;

	std::string assets_dir(dir_name);
	assets_dir.append("/");
	App->file_system->GetFilesFromDirOld(assets_dir.c_str(), file_list, dir_list);
	
	

	for (std::list<std::string>::iterator it_dir = dir_list.begin(); it_dir != dir_list.end(); it_dir++)
	{
		std::string tmp_dir = assets_dir;
		tmp_dir.append((*it_dir).c_str());
		FillFilesTree(tmp_dir);
	}

	for (std::list<std::string>::iterator it_file = file_list.begin(); it_file != file_list.end(); it_file++)
	{
		
	}
}