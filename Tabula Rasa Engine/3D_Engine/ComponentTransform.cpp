#include "ComponentTransform.h"
#include "GameObject.h"

#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "ComponentCamera.h"

#include "ImGuizmo/ImGuizmo.h"
#include "ImGui/imgui.h"

ComponentTransform::ComponentTransform(GameObject* embedded_game_object): Component(embedded_game_object, Component::component_type::COMPONENT_TRANSFORM),
position(float3::zero), scale(float3(1.f, 1.f, 1.f)), rotation(Quat::identity)
{

}

ComponentTransform::ComponentTransform(GameObject* embedded_game_object, const float3 & translation, const float3 & scale, const Quat & rotation) :
	Component(embedded_game_object, Component::component_type::COMPONENT_TRANSFORM),
	position(translation), scale(scale), rotation(rotation)
{

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Setup(const float3 & translation, const float3 & scale, const Quat & rotation)
{
	this->position = translation;
	this->scale = scale;
	this->rotation = rotation;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);

	embedded_go->RecalculateBoundingBox();
	
	if (embedded_go->is_static) {

		App->main_scene->EraseGoInQuadtree(embedded_go);
		embedded_go->is_static = false;
	}
		
}

const float3 & ComponentTransform::GetTranslation() const
{
	return position;
}

const float3 & ComponentTransform::GetScale() const
{
	return scale;
}

const Quat & ComponentTransform::GetRotation() const
{
	return rotation;
}

void ComponentTransform::GetLocalPosition(float3 * position, float3 * scale, Quat * rot) const
{
	*position = this->position;
	*scale = this->scale;
	*rot = this->rotation;
}

float4x4 ComponentTransform::GetMatrix()
{
	local_matrix = float4x4::FromTRS(position, rotation, scale);

	if (embedded_go->GetParent() != nullptr) {
			float4x4 parent_matrix = embedded_go->GetParent()->GetTransform()->GetMatrix();
			global_matrix = parent_matrix * local_matrix;
			return global_matrix;
	}
	else
		return local_matrix;
}

void ComponentTransform::SetPosition(const float3 position)
{
	this->position = position;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
	embedded_go->RecalculateBoundingBox();
}

void ComponentTransform::SetScale(const float3 scale)
{
	this->scale = scale;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
	embedded_go->RecalculateBoundingBox();
}

void ComponentTransform::SetRotation(const Quat rot)
{
	this->rotation = rot;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
	embedded_go->RecalculateBoundingBox();
}

void ComponentTransform::EnableGuizmoOnGo(GameObject* go)
{
	ImGuizmo::Enable(true);
	
	// Setting up default guizmo
	static ImGuizmo::OPERATION current_guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
	static ImGuizmo::MODE current_guizmo_mode = ImGuizmo::MODE::WORLD;
	float4x4 delta_matrix = float4x4::identity;

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	float4x4 matrix(float4x4::identity);

	ImGuizmo::DrawCube(App->camera->dummy_camera->GetViewMatrix(), App->camera->dummy_camera->GetViewMatrix(), matrix.ptr());

	ImGuizmo::Manipulate(App->camera->dummy_camera->GetViewMatrix(), 
						App->camera->dummy_camera->GetViewMatrix(),
					     current_guizmo_operation, current_guizmo_mode,
						 go->GetTransform()->GetMatrix().Transposed().ptr(), 
						 delta_matrix.ptr());
}