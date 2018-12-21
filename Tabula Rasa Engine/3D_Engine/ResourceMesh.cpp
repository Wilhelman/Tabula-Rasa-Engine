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

void ResourceMesh::GenerateAndBindMesh(bool deformable)
{
	glGenBuffers(1, (GLuint*) &(deformable ? this->deformable->vertex_buffer : vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, deformable ? this->deformable->vertex_buffer : vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * deformable ? this->deformable->vertex_size : vertex_size,
		deformable ? this->deformable->vertices : vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (uvs != nullptr) {
		glGenBuffers(1, (GLuint*) &(deformable ? this->deformable->uv_buffer : uv_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, deformable ? this->deformable->uv_buffer :uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * deformable ? this->deformable->size_uv : size_uv, deformable ? 
			this->deformable->uvs : uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glGenBuffers(1, (GLuint*) &(deformable ? this->deformable->index_buffer : index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, deformable ? this->deformable->index_buffer : index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) *deformable ? this->deformable->index_size : index_size,
		deformable ? this->deformable->indices : indices, GL_STATIC_DRAW);
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

void ResourceMesh::DuplicateMesh(ResourceMesh * mesh)
{
	if (indices && vertices && uvs) {
		deformable->vertex_size = mesh->vertex_size;
		deformable->vertices = new float[deformable->vertex_size];
		deformable->indices = new uint[mesh->index_size];
	}
	if (normals) 
		deformable->normals = new float[deformable->vertex_size];
}
