#include "ComponentTransform.h"
#include "GameObject.h"

ComponentTransform::ComponentTransform(GameObject* embedded_game_object): Component(embedded_game_object, Component::component_type::COMPONENT_TRANSFORM)
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

	this->transform_global = float4x4::FromTRS(this->position, this->rotation, this->scale);
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

void ComponentTransform::GetGlobalPosition(float3 * position, float3 * scale, Quat * rot)const
{
	GameObject* tmp_embedded_go = this->embedded_go;
	while (tmp_embedded_go != nullptr)
	{
		*position += tmp_embedded_go->GetTransform()->GetTranslation();
		*scale = this->GetScale().Mul(tmp_embedded_go->GetTransform()->GetScale());
		*rot = this->GetRotation().Mul(tmp_embedded_go->GetTransform()->GetRotation());

		if (tmp_embedded_go->GetParent()->GetTransform() == nullptr)// if root no need to iterate
			tmp_embedded_go = nullptr;
		else
			tmp_embedded_go = tmp_embedded_go->GetParent();
	}
}

float* ComponentTransform::GetMatrix() const
{
	float3 tmp_pos = float3::zero;
	float3 tmp_scale = float3::zero;
	Quat tmp_rot = Quat::identity;

	this->GetGlobalPosition(&tmp_pos, &tmp_scale, &tmp_rot);

	return float4x4::FromTRS(tmp_pos, tmp_rot, tmp_scale).Transposed().ptr();
}

void ComponentTransform::SetPosition(const float3 position)
{
	this->position = position;

	this->transform_global = float4x4::FromTRS(this->position, this->rotation, this->scale);
}

void ComponentTransform::SetScale(const float3 scale)
{
	this->scale = scale;

	this->transform_global = float4x4::FromTRS(this->position, this->rotation, this->scale);
}

void ComponentTransform::SetRotation(const Quat rot)
{
	this->rotation = rot;

	this->transform_global = float4x4::FromTRS(this->position, this->rotation, this->scale);
}
