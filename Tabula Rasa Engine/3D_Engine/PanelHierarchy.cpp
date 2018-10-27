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

	for (std::list<GameObject*>::const_iterator it = root->childs.begin(); it != root->childs.end(); it++)
		DrawGameObject(*it);
	
	ImGui::End();
}

void PanelHierarchy::DrawGameObject(const GameObject * game_object)
{
	uint tree_node_flags = 0u;

	if(game_object->childs.size() == 0)
		tree_node_flags |= ImGuiTreeNodeFlags_Leaf;

	if (ImGui::TreeNodeEx(game_object->GetName(), tree_node_flags))
	{
		for (std::list<GameObject*>::const_iterator it = game_object->childs.begin(); it != game_object->childs.end(); it++)
			DrawGameObject(*it);

		ImGui::TreePop();
	}
}
