#include "SDL\include\SDL.h"

#include "PanelInspector.h"
#include "trRenderer3D.h"

PanelInspector::PanelInspector() : Panel("Inspector", SDL_SCANCODE_I)
{
	active = false;
}

PanelInspector::~PanelInspector()
{
	meshes.clear();
}

void PanelInspector::SetDataFrom(Mesh* mesh)
{
	meshes.push_back(mesh);
}

void PanelInspector::Draw()
{
	ImGui::Begin("Inspector", &active);

	if (!meshes.empty()) {
		ImGui::Text("Model name: %s", meshes.at(0)->name.c_str());
		ImGui::Text("Source: %s", meshes.at(0)->path.c_str());
		ImGui::Text("Number of meshes: %i", meshes.size());
		ImGui::Text("Total vertices: %i", meshes.size());
		ImGui::Text("Total indices: %i", meshes.size());
		ImGui::Text("Total UV: %i", meshes.size());
		for (uint i = 0; i < meshes.size(); i++)
		{
			std::string tmp = "Mesh ";
			tmp = tmp + std::to_string(i + 1);
			if (ImGui::CollapsingHeader(tmp.c_str()))
			{
				ImGui::Text("Mesh source path: %s", meshes.at(i)->path.c_str());
			}
		}
	}

	ImGui::End();
}