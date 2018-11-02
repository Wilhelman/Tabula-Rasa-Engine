#include "SDL/include/SDL.h"

#include "PanelInspector.h"
#include "trRenderer3D.h"

#include "trApp.h"
#include "trEditor.h"

#include "MathGeoLib/MathGeoLib.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

PanelInspector::PanelInspector() : Panel("Inspector", SDL_SCANCODE_I)
{
	active = false;
}

PanelInspector::~PanelInspector()
{
}


void PanelInspector::Draw()
{
	ImGui::Begin("Inspector", &active);

	GameObject* selected = App->editor->GetSelected();

	if (selected != nullptr) {
		
		strcpy_s(go_name, GAMEOBJECT_MAX_LENGTH, selected->GetName());
		if (ImGui::InputText("##GO_NAME", go_name, GAMEOBJECT_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			selected->SetName(go_name);

		ImGui::Separator();

		ComponentTransform* trans_co = selected->GetTransform();
		float3 position = float3::zero;
		float3 scale = float3::zero;
		Quat rotation = Quat::identity;

		trans_co->GetLocalPosition(&position, &scale, &rotation);

		if (ImGui::CollapsingHeader("TRANSFORM COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
			bool have_to_update = false;

			ImGui::Text("Position:");
			if (ImGui::DragFloat3("##POSITION", (float*)&position, 0.2f))
				have_to_update = true;

			ImGui::Text("Rotation:");
			float3 euler_rot = rotation.ToEulerXYZ();
			float3 deg_euler_rot = float3(math::RadToDeg(euler_rot.x), math::RadToDeg(euler_rot.y), math::RadToDeg(euler_rot.z));
			if (ImGui::DragFloat3("##ROTATION", (float*)&deg_euler_rot), 0.2f, -180, 180) {
				have_to_update = true;
				euler_rot.Set(math::DegToRad(deg_euler_rot.x), math::DegToRad(deg_euler_rot.y), math::DegToRad(deg_euler_rot.z));
				rotation = math::Quat::FromEulerXYZ(euler_rot.x, euler_rot.y, euler_rot.z);
			}

			ImGui::Text("Scale:");
			if (ImGui::DragFloat3("##SCALE", (float*)&scale, 0.02f))
				have_to_update = true;

			if(have_to_update)
				trans_co->Setup(position, scale, rotation);
		}
			
		ImGui::Separator();
		for (std::list<Component*>::iterator it = selected->components.begin(); it != selected->components.end(); it++) {
			switch ((*it)->GetType())
			{
			case Component::component_type::COMPONENT_TRANSFORM:
				continue;
			break;
			case Component::component_type::COMPONENT_MESH: 
			{
				ComponentMesh* mesh_co = (ComponentMesh*)(*it);
				const Mesh* mesh = mesh_co->GetMesh();
				if (ImGui::CollapsingHeader("MESH COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (mesh != nullptr) {
						ImGui::Text("Triangles: %i", mesh->face_size);
						ImGui::Text("Vertices: %i", mesh->vertex_size / 3);///bc vertices are stored in x/y/z format!
						ImGui::Text("Indices: %i", mesh->index_size);
						ImGui::Text("UVS: %i", mesh->size_uv);
						ImGui::Text("Source: %s", mesh->path.c_str());
					}
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_MATERIAL:
			{
				ComponentMaterial* mat_co = (ComponentMaterial*)(*it);
				const Texture* texture = mat_co->GetTexture();
				if (ImGui::CollapsingHeader("MATERIAL COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (texture != nullptr) {
						ImGui::Text("Source: %s", texture->path.c_str());
						ImGui::Text("Width: %i", texture->width);
						ImGui::Text("Height: %i", texture->height);
						ImGui::Image((ImTextureID)texture->id, ImVec2(200, 200));
					}
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_UNKNOWN:
				TR_LOG("Rly?");
				break;
			default:
				break;
			}

		}

	}

	ImGui::End();
}