#include "ComponentMaterial.h"

ComponentMaterial::ComponentMaterial(GameObject * embedded_game_object) :
Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}
