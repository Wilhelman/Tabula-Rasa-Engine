#ifndef __RESOURCE_MESH_H__
#define __RESOURCE_MESH_H__

#include "Resource.h"

class ResourceMesh : public Resource
{

public:
	ResourceMesh(UID uid);
	~ResourceMesh();

	void GenerateAndBindMesh();

	bool LoadInMemory() override;
	bool ReleaseMemory() override;

public:
	std::string path;

	uint index_buffer = 0u;
	uint index_size = 0u;
	uint* indices = nullptr;

	uint face_size = 0u;

	uint vertex_buffer = 0u;
	uint vertex_size = 0u;
	float* vertices = nullptr;

	uint normal_buffer = 0u;
	uint normal_size = 0u;
	float* normals = nullptr;

	uint uv_buffer = 0u;
	uint size_uv = 0u;
	float* uvs = nullptr;

	UID texture_uuid = 0u;
};

#endif // __RESOURCE_MESH_H__