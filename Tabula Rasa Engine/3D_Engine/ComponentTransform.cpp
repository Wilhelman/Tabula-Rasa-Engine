#include "ComponentTransform.h"
#include "GameObject.h"

#include "trApp.h"
#include "trMainScene.h"

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
		App->main_scene->ReDoQuadtree();
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
