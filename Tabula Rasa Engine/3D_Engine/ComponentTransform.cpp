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

bool ComponentTransform::Save(JSON_Object* component_obj) const
{
	// Translation
	JSON_Value* component_value = json_value_init_array();
	JSON_Array* translation_array = json_value_get_array(component_value);
	json_object_set_value(component_obj, "Translation", component_value);
	json_array_append_number(translation_array, position.x);
	json_array_append_number(translation_array, position.y);
	json_array_append_number(translation_array, position.z);

	// Scale
	component_value = json_value_init_array();
	JSON_Array* scale_array = json_value_get_array(component_value);
	json_object_set_value(component_obj, "Scale", component_value);
	json_array_append_number(scale_array, scale.x);
	json_array_append_number(scale_array, scale.y);
	json_array_append_number(scale_array, scale.z);
	
	// Rotation
	component_value = json_value_init_array();
	JSON_Array* rotation_array = json_value_get_array(component_value);
	json_object_set_value(component_obj, "Rotation", component_value);
	json_array_append_number(rotation_array, rotation.w);
	json_array_append_number(rotation_array, rotation.x);
	json_array_append_number(rotation_array, rotation.y);
	json_array_append_number(rotation_array, rotation.z);

	return true;
}

bool ComponentTransform::Load(const JSON_Object * component_obj)
{
	// Translation
	JSON_Array* array = json_object_get_array(component_obj, "Translation");
	JSON_Value* value = json_array_get_value(array, 0);
	position.x = json_value_get_number(value);
	value = json_array_get_value(array, 1);
	position.y = json_value_get_number(value);
	value = json_array_get_value(array, 2);
	position.z = json_value_get_number(value);

	// Scale
	array = json_object_get_array(component_obj, "Scale");
	value = json_array_get_value(array, 0);
	scale.x = json_value_get_number(value);
	value = json_array_get_value(array, 1);
	scale.y = json_value_get_number(value);
	value = json_array_get_value(array, 2);
	scale.z = json_value_get_number(value);

	// Rotation
	array = json_object_get_array(component_obj, "Rotation");
	value = json_array_get_value(array, 0);
	rotation.w = json_value_get_number(value);
	value = json_array_get_value(array, 1);
	rotation.x = json_value_get_number(value);
	value = json_array_get_value(array, 2);
	rotation.y = json_value_get_number(value);
	value = json_array_get_value(array, 3);
	rotation.z = json_value_get_number(value);

	return true;
}

void ComponentTransform::Setup(const float3 & translation, const float3 & scale, const Quat & rotation, bool importing)
{
	this->position = translation;
	this->scale = scale;
	this->rotation = rotation;

	this->local_matrix = float4x4::FromTRS(this->position, this->rotation, this->scale);

	embedded_go->RecalculateBoundingBox();
	
	if (embedded_go->is_static && !importing) {
		App->main_scene->EraseGoInQuadtree(embedded_go);
		embedded_go->is_static = false;
	}
}

void ComponentTransform::SetupFromGlobalMatrix(float4x4 global_matrix)
{
	float4x4 new_local_mat;

	if (embedded_go->GetParent() != nullptr)
	{
		float4x4 parent_global_mat = embedded_go->GetParent()->GetTransform()->GetMatrix();
		parent_global_mat = parent_global_mat.Transposed();
		parent_global_mat = parent_global_mat.Inverted();
		new_local_mat = parent_global_mat * global_matrix;
	}
	else
		new_local_mat = global_matrix;

	new_local_mat.Decompose(position, rotation, scale);
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
			parent_matrix = parent_matrix.Transposed();
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