#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject * embedded_game_object) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}

ComponentMesh::ComponentMesh(GameObject * embedded_game_object, std::vector<Mesh*> meshes) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH), 
	meshes(meshes)
{
}
