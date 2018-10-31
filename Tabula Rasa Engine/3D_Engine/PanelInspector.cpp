#include "SDL/include/SDL.h"

#include "PanelInspector.h"
#include "trRenderer3D.h"

#include "trApp.h"
#include "trEditor.h"

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
		ImGui::Text("GameObject name: %s", selected->GetName());
		ImGui::Separator();
		for (std::list<Component*>::iterator it = selected->components.begin(); it != selected->components.end(); it++) {
			switch ((*it)->GetType())
			{
			case Component::component_type::COMPONENT_TRANSFORM:
			{
				ComponentTransform* trans_co = (ComponentTransform*)(*it);
				float3 position = trans_co->GetTranslation();
				float3 scale = trans_co->GetScale();
				Quat rotation = trans_co->GetRotation();
				if (ImGui::CollapsingHeader("TRANSFORM COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::Text("Position:");
					float pos[3] = { position.x, position.y, position.z };
					ImGui::InputFloat3("##POS", pos, 2);
					ImGui::Text("Rotation:");
					float rot[3] = { rotation.x, rotation.y, rotation.z };
					ImGui::InputFloat3("##ROT", rot, 2);
					ImGui::Text("Scale:");
					float sca[3] = { scale.x, scale.y, scale.z };
					ImGui::InputFloat3("##SCA", sca, 2);
				}
				ImGui::Separator();
				break;
			}
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