#include "SDL/include/SDL.h"

#include "PanelInspector.h"
#include "trRenderer3D.h"

#include "trApp.h"
#include "trEditor.h"
#include "trMainScene.h"

#include "MathGeoLib/MathGeoLib.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "ResourceMesh.h"



PanelInspector::PanelInspector() : Panel("Inspector", SDL_SCANCODE_I)
{
	active = false;
	btn_guizmo_mode = "Local";
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	ImGui::Begin("Inspector", &active);

	GameObject* selected = App->editor->GetSelected();

	if (selected != nullptr) {
		ImGui::Checkbox("##ACTIVE", &selected->is_active);
		ImGui::SameLine();
		strcpy_s(go_name, GAMEOBJECT_MAX_LENGTH, selected->GetName());
		if (ImGui::InputText("##GO_NAME", go_name, GAMEOBJECT_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			selected->SetName(go_name);

		ImGui::SameLine();

		if (ImGui::Checkbox("Static##STATIC", &selected->is_static)) {
			if (selected->is_static) {
				App->main_scene->InsertGoInQuadtree(selected);
			}
			else {
				App->main_scene->EraseGoInQuadtree(selected);
			}
		}

		ImGui::Separator();

		ComponentTransform* trans_co = selected->GetTransform();
		float3 position = float3::zero;
		float3 scale = float3::zero;
		Quat rotation = Quat::identity;

		trans_co->GetLocalPosition(&position, &scale, &rotation);

		if (ImGui::CollapsingHeader("TRANSFORM COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) 
		{
			bool have_to_update = false;

			if (ImGui::Button("Reset"))
			{
				have_to_update = true;
				position = float3::zero;
				scale = float3::one;
				rotation = Quat::identity;
			}

			ImGui::SameLine();

			if (ImGui::Button(btn_guizmo_mode.c_str()))
			{
				if (App->editor->guizmo_mode == ImGuizmo::MODE::WORLD)
				{
					App->editor->guizmo_mode = ImGuizmo::MODE::LOCAL;
					btn_guizmo_mode = "Global";
				}
				else
				{
					App->editor->guizmo_mode = ImGuizmo::MODE::WORLD;
					btn_guizmo_mode = "Local";
				}
			}

			ImGui::Text("Position:");
			if (ImGui::DragFloat3("##POSITION", (float*)&position, 0.2f))
				have_to_update = true;

			ImGui::Text("Rotation:");
			float3 rot_axis;
			float rot_angle;
			rotation.ToAxisAngle(rot_axis, rot_angle);
			rot_axis *= rot_angle;

			if (rot_axis.x < 0.0f)
				rot_axis.x += 2 * pi;
			if (rot_axis.y < 0.0f)
				rot_axis.y += 2 * pi;
			if (rot_axis.z < 0.0f)
				rot_axis.z += 2 * pi;

			rot_axis = math::RadToDeg(rot_axis);

			if (ImGui::DragFloat3("##ROTATION", (float*)&rot_axis, 0.1f))
			{
				have_to_update = true;
				rot_axis = math::DegToRad(rot_axis);
				rotation.SetFromAxisAngle(rot_axis.Normalized(), rot_axis.Length());
			}

			ImGui::Text("Scale:");
			if (ImGui::DragFloat3("##SCALE", (float*)&scale, 0.02f))
				have_to_update = true;

			if (have_to_update)
				trans_co->Setup(position, scale, rotation);
		}
			
		ImGui::Separator();
		for (std::list<Component*>::iterator it = selected->components.begin(); it != selected->components.end(); it++) {
			switch ((*it)->GetType())
			{
			case Component::component_type::COMPONENT_TRANSFORM:
				continue;
			break;
			case Component::component_type::COMPONENT_CAMERA: {
				ComponentCamera* camera_co = (ComponentCamera*)(*it);
				if (ImGui::CollapsingHeader("CAMERA COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {

					// TODO: all this should affect the camera prespective
					ImGui::Checkbox("Frustum culling##FRUSTUM_CULLING", &camera_co->frustum_culling);
					
					ImGui::Separator();
					ImGui::Text("Clipping Planes");

					if (ImGui::SliderFloat("Near plane##nearplane", &camera_co->frustum.nearPlaneDistance, 0.5f, camera_co->frustum.farPlaneDistance - 1.0f));
						camera_co->projection_needs_update = true;

					if (ImGui::SliderFloat("Far plane##farplane", &camera_co->frustum.farPlaneDistance, 0.5f, 1000.0f))
						camera_co->projection_needs_update = true;

					ImGui::Separator();
				
					float fov = math::RadToDeg(camera_co->frustum.verticalFov);

					if (ImGui::SliderFloat("Field of View", &fov, 1.0f, 179.0f))
						camera_co->SetVerticalFov(fov);
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_MESH: 
			{
				ComponentMesh* mesh_co = (ComponentMesh*)(*it);
				ResourceMesh* mesh = (ResourceMesh*)mesh_co->GetResource();
				if (ImGui::CollapsingHeader("MESH COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (mesh != nullptr) {
						ImGui::Text("Triangles: %i", mesh->face_size);
						ImGui::Text("Vertices: %i", mesh->vertex_size / 3);///bc vertices are stored in x/y/z format!
						ImGui::Text("Indices: %i", mesh->index_size);
						ImGui::Text("UVS: %i", mesh->size_uv);
						ImGui::Text("Source: %s", mesh->path.c_str());
						ImGui::Text("Resource References: %i", mesh->CountReferences());
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