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
		for (uint i = 0; i < meshes.size(); i++)
		{
			ImGui::Text("Mesh name: %s", meshes.at(i)->name.c_str());
		}
	}

	ImGui::End();
}