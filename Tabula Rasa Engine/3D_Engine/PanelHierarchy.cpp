#include "PanelHierarchy.h"
#include "imgui.h"

#include "trApp.h"
#include "GameObject.h"
#include "trMainScene.h"
#include "trDefs.h"
#include <list>

#include "trEditor.h"

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

	if (root != nullptr) {
		std::list<GameObject*>::const_iterator it = root->childs.begin();
		while (it != root->childs.end()) {
			DrawGameObject(*it);
			it++;
		}
	}
	
	ImGui::End();
}

void PanelHierarchy::DrawGameObject(GameObject * game_object)
{
	uint tree_node_flags = 0u;

	if(game_object->childs.size() == 0)
		tree_node_flags |= ImGuiTreeNodeFlags_Leaf;

	if (game_object == App->editor->GetSelected())
		tree_node_flags |= ImGuiTreeNodeFlags_Selected;

	// TODO : solve first click error with a gameobject with childs

	if (ImGui::TreeNodeEx(game_object->GetName(), tree_node_flags))
	{
		if (ImGui::IsItemClicked(0))  // left click
			App->editor->SetSelected(game_object);

		if (ImGui::IsItemClicked(1))  // right click
			ImGui::OpenPopup("Options");

		if (ImGui::BeginPopup("Options"))
		{
			//if (ImGui::MenuItem("Clone"))
				//TR_LOG("TODO CLONE");

			if (ImGui::MenuItem("Remove"))
				game_object->to_destroy = true;

			ImGui::EndPopup();
		}

		std::list<GameObject*>::const_iterator it = game_object->childs.begin();
		while (it != game_object->childs.end()) {
			DrawGameObject(*it);
			it++;
		}

		ImGui::TreePop();
	}
}
