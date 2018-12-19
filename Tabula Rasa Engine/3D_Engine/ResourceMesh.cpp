#include "ResourceMesh.h"

#include "trOpenGL.h"

ResourceMesh::ResourceMesh(UID uid) : Resource(uid, Resource::Type::MESH)
{
}

ResourceMesh::~ResourceMesh()
{
	if (indices != nullptr) { delete[] indices; indices = nullptr; }
	if (vertices != nullptr) { delete[] vertices; vertices = nullptr; }
	if (uvs != nullptr) { delete[] uvs; uvs = nullptr; }
	if (normals != nullptr) { delete[] normals; normals = nullptr; }

	path.clear();

	glDeleteBuffers(1, (GLuint*)&index_buffer);
	glDeleteBuffers(1, (GLuint*)&vertex_buffer);
	glDeleteBuffers(1, (GLuint*)&uv_buffer);
}

void ResourceMesh::GenerateAndBindMesh()
{
	glGenBuffers(1, (GLuint*) &(vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size, vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (uvs != nullptr) {
		glGenBuffers(1, (GLuint*) &(uv_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size_uv, uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glGenBuffers(1, (GLuint*) &(index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_size, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

bool ResourceMesh::LoadInMemory()
{
	GenerateAndBindMesh();
	return true;
}

bool ResourceMesh::ReleaseMemory()
{
	glDeleteBuffers(1, (GLuint*)&index_buffer);
	glDeleteBuffers(1, (GLuint*)&vertex_buffer);
	glDeleteBuffers(1, (GLuint*)&uv_buffer);

	return true;
}
