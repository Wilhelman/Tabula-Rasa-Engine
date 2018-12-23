#include "SDL/include/SDL.h"

#include "PanelInspector.h"
#include "trRenderer3D.h"

#include "trApp.h"
#include "trEditor.h"
#include "trMainScene.h"
#include "trWindow.h"
#include "trAnimation.h"

#include "MathGeoLib/MathGeoLib.h"

#include "imgui_timeline.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "ComponentBone.h"
#include "ComponentAnimation.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceBone.h"
#include "ResourceAnimation.h"


PanelInspector::PanelInspector() : Panel("Inspector", SDL_SCANCODE_I)
{
	active = true;
	btn_guizmo_mode = "Local";
	current_anim_time[0] = 0.0f;
	current_anim_time[1] = 0.0f;
}

PanelInspector::~PanelInspector()
{
}

void PanelInspector::Draw()
{
	
	ImGui::SetNextWindowPos(ImVec2(App->window->GetWidth() - App->window->GetWidth() * 0.21f, 46.f));
	ImGui::SetNextWindowSize(ImVec2(App->window->GetWidth() * 0.21f, App->window->GetHeight() - 246.f));
	ImGui::Begin("Inspector", &active, ImGuiWindowFlags_NoCollapse
		| ImGuiWindowFlags_AlwaysAutoResize);

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
				
					if (ImGui::SliderFloat("Near plane##nearplane", &camera_co->frustum.nearPlaneDistance, 0.5f, 995.0f));
						camera_co->projection_needs_update = true;

					if (ImGui::SliderFloat("Far plane##farplane", &camera_co->frustum.farPlaneDistance, 0.5f, 1000.0f))
						camera_co->projection_needs_update = true;

					ImGui::Separator();
				
					float fov = math::RadToDeg(camera_co->frustum.verticalFov);

					if (ImGui::SliderFloat("Field of View", &fov, 1.0f, 179.0f))
						camera_co->SetUpFov(fov);
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
						//ImGui::Text("Triangles: %i", mesh->face_size);
						ImGui::Text("Vertices: %i", mesh->vertex_size / 3);///bc vertices are stored in x/y/z format!
						ImGui::Text("Indices: %i", mesh->index_size);
						ImGui::Text("UVS: %i", mesh->size_uv);
						ImGui::Text("Source: %s", mesh->GetExportedFile());
						ImGui::Text("Resource References: %i", mesh->CountReferences());
					}
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_BONE:
			{
				ComponentBone* bone_co= (ComponentBone*)(*it);
				ResourceBone* bone= (ResourceBone*)bone_co->GetResource();
				if (ImGui::CollapsingHeader("BONE COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (bone != nullptr) {
						ImGui::Text("Weights number: %i", bone->bone_weights_size);
						ImGui::Text("Embedded mesh UUID: %i", bone->mesh_uid);
						ImGui::Text("Source: %s", bone->GetExportedFile());
						ImGui::Text("Resource References: %i", bone->CountReferences());
					}
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_ANIMATION:
			{
				ComponentAnimation* animation_co = (ComponentAnimation*)(*it);
				ResourceAnimation* animation = (ResourceAnimation*)animation_co->GetResource();

				if (ImGui::CollapsingHeader("ANIMATION COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) 
				{
					if (animation != nullptr) 
					{
						ImGui::Text("Name: %s", animation->name);

						// TODO: only update current_anim_time if animation is playing,
						// otherwise it should be able to be modified by the user producing
						// a change in the animation as in Maya. 
						// GetCurrentAnimationTime should recieve a parameter of the selected animation
						// once we handle multiple animations in the animation module
						current_anim_time[0] = 0.0f;
						current_anim_time[1] = App->animation->GetCurrentAnimationTime();
						
						ImGui::BeginTimeline("TimeLine##TIMELINE", (float)(animation->duration / animation->ticks_per_second));

						if (current_anim_time[1] < 0.0f)
							current_anim_time[1] = 0.0f;
						else if (current_anim_time[1] > (float)(animation->duration / animation->ticks_per_second))
							current_anim_time[1] = (float)(animation->duration / animation->ticks_per_second);

						if (ImGui::TimelineEvent("TimeLine##TIMELINE", current_anim_time))
							App->animation->SetCurrentAnimationTime(current_anim_time[1]);

						ImGui::EndTimeline();

						ImGui::NewLine();

						ImGui::Text("Time: %.2f %s", current_anim_time[1], "s");

						ImGui::NewLine();
						ImGui::Separator();
						ImGui::NewLine();

						ImGui::Text("Keys number: %i", animation->num_keys);

						if (ImGui::Button("BACKWARDS"))
							App->animation->StepBackwards();

						ImGui::SameLine();
						
						if (ImGui::Button("PLAY"))
							App->animation->PlayAnimation();

						ImGui::SameLine();

						if (ImGui::Button("PAUSE"))
							App->animation->PauseAnimation();

						ImGui::SameLine();

						if (ImGui::Button("STOP"))
							App->animation->StopAnimation();

						ImGui::SameLine();

						if (ImGui::Button("FORWARD"))
							App->animation->StepForward();

						ImGui::NewLine();
						ImGui::Separator();
						ImGui::NewLine();

						ImGui::SliderFloat("Speed", &App->animation->GetCurrentAnimation()->anim_speed, 0.0f, 5.0f);
						ImGui::Checkbox("Loop", &App->animation->GetCurrentAnimation()->loop);
						ImGui::Checkbox("Interpolate", &App->animation->GetCurrentAnimation()->interpolate);

						ImGui::NewLine();
						ImGui::Separator();
						ImGui::NewLine();

						ImGui::Text("Duration: %f", (float)(animation->duration / animation->ticks_per_second));
						ImGui::Text("Source: %s", animation->GetExportedFile());
						ImGui::Text("Resource References: %i", animation->CountReferences());

						ImGui::NewLine();
						ImGui::Separator();
						ImGui::NewLine();

						ImGui::Text("Animations to select");

						ImGui::Separator();

						for (uint i = 0; i < App->animation->GetAnimationsNumber(); ++i)
						{
							//if (ImGui::MenuItem(App->animation->GetAnimationName(i)))						// TODO CHECK THIS
								//App->animation->SetCurrentAnimation(i);
						}	

						ImGui::Separator();

						//ImGui::Text("Current animation: %s", App->animation->GetCurrentAnimation()->name.c_str());
					}
				}
				ImGui::Separator();
				break;
			}
			case Component::component_type::COMPONENT_MATERIAL:
			{
				ComponentMaterial* mat_co = (ComponentMaterial*)(*it);
				ResourceTexture* texture = (ResourceTexture*)mat_co->GetResource();
				if (ImGui::CollapsingHeader("MATERIAL COMPONENT", ImGuiTreeNodeFlags_DefaultOpen)) {
					if (texture != nullptr) {
						ImGui::Text("Source: %s", texture->GetExportedFile());
						ImGui::Text("Width: %i", texture->width);
						ImGui::Text("Height: %i", texture->height);
						ImGui::Image((ImTextureID)texture->gpu_id, ImVec2(200, 200));
						
						ImGui::SliderFloat("Alpha test", &mat_co->alpha_test, 0.0f, 1.0f);
						ImGui::Text("Resource References: %i", texture->CountReferences());
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