#include "ComponentTransform.h"

ComponentTransform::ComponentTransform(GameObject* embedded_game_object): Component(embedded_game_object, Component::component_type::COMPONENT_TRANSFORM)
{
}

ComponentTransform::ComponentTransform(GameObject* embedded_game_object, const float3 & translation, const float3 & scale, const Quat & rotation) :
	Component(embedded_game_object, Component::component_type::COMPONENT_TRANSFORM),
	translation(translation), scale(scale), rotation(rotation)
{
}

ComponentTransform::~ComponentTransform()
{
}
