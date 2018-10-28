#include "ComponentMesh.h"

ComponentMesh::ComponentMesh(GameObject * embedded_game_object) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}

ComponentMesh::ComponentMesh(GameObject * embedded_game_object, Mesh* mesh) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH), 
	mesh(mesh)
{
}

void ComponentMesh::SetMesh(Mesh * mesh)
{
	this->mesh = mesh;
}

const Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}
