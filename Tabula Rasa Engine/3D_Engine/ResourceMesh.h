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

	//void Save(Config& config) const override;
	//void Load(const Config& config) override;

public:
	std::string path;

	uint index_buffer = 0u;
	uint index_size = 0u;
	uint* indices = nullptr;

	uint face_size = 0u;

	uint vertex_buffer = 0u;
	uint vertex_size = 0u;
	float* vertices = nullptr;

	uint uv_buffer = 0u;
	uint size_uv = 0u;
	float* uvs = nullptr;
};

#endif // __RESOURCE_MESH_H__