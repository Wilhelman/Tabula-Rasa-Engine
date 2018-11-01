#include "ComponentMaterial.h"
#include "trApp.h"
#include "trTextures.h"

ComponentMaterial::ComponentMaterial(GameObject * embedded_game_object) :
Component(embedded_game_object, Component::component_type::COMPONENT_MATERIAL)
{
}

ComponentMaterial::~ComponentMaterial()
{
	App->texture->DeleteTextureBuffer(this->texture);
	RELEASE(texture);
}

void ComponentMaterial::SetAmbientColor(float4 ambient_color)
{
	this->ambient_color = ambient_color;
}

const float4 ComponentMaterial::GetAmbientColor() const
{
	return ambient_color;
}

const Texture * ComponentMaterial::GetTexture() const
{
	if (texture != nullptr)
		return texture;
	else
		return nullptr;
}

void ComponentMaterial::SetTexture(Texture * texture)
{
	this->texture = texture;
}
