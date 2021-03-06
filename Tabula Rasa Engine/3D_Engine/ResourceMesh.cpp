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
	if (vertex_buffer == 0) {
		vertex_buffer = 1;
		uv_buffer = 2;
		index_buffer = 3;
	}
	glGenBuffers(1, (GLuint*) &( vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertex_size, deformable ? this->deformable->vertices : vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	if (uvs != nullptr) {
		glGenBuffers(1, (GLuint*) &(uv_buffer));
		glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * size_uv, deformable ? this->deformable->uvs : uvs, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glGenBuffers(1, (GLuint*) &(index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,  index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * index_size, deformable ? this->deformable->indices : indices, GL_STATIC_DRAW);
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
		deformable->index_size = mesh->index_size;
		deformable->size_uv = mesh->size_uv;

		deformable->index_buffer = mesh->index_buffer;
		deformable->vertex_buffer = mesh->vertex_buffer;
		deformable->uv_buffer = mesh->uv_buffer;

		deformable->vertices = new float[deformable->vertex_size];
		deformable->indices = new uint[mesh->index_size];
		deformable->uvs = new float[mesh->size_uv];

		memcpy(deformable->vertices, mesh->vertices, sizeof(float) * deformable->vertex_size);
		memcpy(deformable->uvs, mesh->uvs, sizeof(float) * deformable->size_uv);
		memcpy(deformable->indices, mesh->indices, sizeof(uint) * deformable->index_size);
	}
	if (normals) {
		deformable->normal_buffer = mesh->normal_buffer;
		deformable->normals = new float[deformable->vertex_size];
		memcpy(deformable->normals, mesh->normals, sizeof(float) * mesh->normal_size);
	}
		
}
