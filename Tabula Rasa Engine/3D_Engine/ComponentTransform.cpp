#include "ComponentTransform.h"
#include "GameObject.h"

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

void ComponentTransform::GetGlobalPosition(float3 * position, float3 * scale, Quat * rot)
{
	GameObject* tmp_embedded_go = embedded_go->GetParent();
	while (tmp_embedded_go != nullptr)
	{
		*position += tmp_embedded_go->GetTransform()->position;
		*scale = this->scale.Mul(tmp_embedded_go->GetTransform()->scale);
		*rot = this->rotation.Mul(tmp_embedded_go->GetTransform()->rotation);
		
		tmp_embedded_go = tmp_embedded_go->GetParent();
	}
}

float* ComponentTransform::GetMatrix()
{
	float3 tmp_pos = position;
	float3 tmp_scale = scale;
	Quat tmp_rot = rotation;
	GetGlobalPosition(&tmp_pos, &tmp_scale, &tmp_rot);
	

	return float4x4::FromTRS(tmp_pos, tmp_rot, tmp_scale).Transposed().ptr();
}

void ComponentTransform::SetPosition(const float3 position)
{
	this->position = position;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
}

void ComponentTransform::SetScale(const float3 scale)
{
	this->scale = scale;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
}

void ComponentTransform::SetRotation(const Quat rot)
{
	this->rotation = rot;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);
}
