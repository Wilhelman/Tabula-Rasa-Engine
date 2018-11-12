#include "ComponentMesh.h"

#include "trRenderer3D.h"

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

ComponentMesh::ComponentMesh(GameObject * embedded_game_object) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}

ComponentMesh::ComponentMesh(GameObject * embedded_game_object, Mesh* mesh) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH), 
	mesh(mesh)
{
}

ComponentMesh::~ComponentMesh()
{
	RELEASE(mesh);
}

bool ComponentMesh::Save(JSON_Object* component_obj) const
{
	json_object_set_string(component_obj, "path", mesh->path.c_str());
	return true;
}

void ComponentMesh::GenerateAndBindMesh(Mesh * mesh)
{
	glGenBuffers(1, (GLuint*) &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->vertex_size, mesh->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (mesh->uvs != nullptr) {
		glGenBuffers(1, (GLuint*) &(mesh->uv_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * mesh->size_uv , mesh->uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glGenBuffers(1, (GLuint*) &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_size, mesh->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ComponentMesh::SetMesh(Mesh * mesh)
{
	this->mesh = mesh;
	GenerateAndBindMesh(mesh);
}

const Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

Mesh::~Mesh()
{
	if (indices != nullptr) { delete[] indices; indices = nullptr; }
	if (vertices != nullptr) { delete[] vertices; vertices = nullptr; }
	if (uvs != nullptr) { delete[] uvs; uvs = nullptr; }

	path.clear();

	glDeleteBuffers(1, (GLuint*)&index_buffer);
	glDeleteBuffers(1, (GLuint*)&vertex_buffer);
	glDeleteBuffers(1, (GLuint*)&uv_buffer);
}