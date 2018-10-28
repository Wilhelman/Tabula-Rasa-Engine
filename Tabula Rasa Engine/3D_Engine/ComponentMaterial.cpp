#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject * embedded_game_object) :
Component(embedded_game_object, Component::component_type::COMPONENT_MATERIAL)
{
}

const Texture * ComponentMaterial::GetTexture() const
{
	return texture;
}

void ComponentMaterial::SetTexture(Texture * texture)
{
	this->texture = texture;
}
