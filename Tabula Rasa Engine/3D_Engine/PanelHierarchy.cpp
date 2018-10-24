#include "PanelHierarchy.h"
#include "imgui.h"

#include "trApp.h"
#include "GameObject.h"
#include "trMainScene.h"
#include "trDefs.h"
#include <list>

PanelHierarchy::PanelHierarchy() : Panel("Game Objects Hierarchy", SDL_SCANCODE_3)
{
	active = false;
}

PanelHierarchy::~PanelHierarchy()
{
}

void PanelHierarchy::Draw()
{
	ImGui::Begin("GameObjects Hierarchy", &active,
		ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoFocusOnAppearing | 
		ImGuiWindowFlags_HorizontalScrollbar);

	if (ImGui::BeginMenu("Options"))
	{
		ImGui::Text("POTATOE");
		ImGui::EndMenu();
	}

	GameObject* root = App->main_scene->GetRoot();

	//std::list<GameObject*>::const_iterator it = root->childs.begin();

	//todo iterate and draw
	
	ImGui::End();
}