#ifndef __RESOURCE_BONE_H__
#define __RESOURCE_BONE_H__

#include "Resource.h"
#include "MathGeoLib/Math/float4x4.h"

class ResourceBone : public Resource
{

public:
	ResourceBone(UID uid);
	~ResourceBone();

	bool LoadInMemory() override;
	bool ReleaseMemory() override;

public:

	std::string name;
	math::float4x4 offset_matrix = math::float4x4::identity;
	uint bone_weights_size = 0u;
	float* bone_weights = nullptr;
	uint* bone_weights_indices = nullptr;
	UID mesh_uid = 0u;
	
};

#endif // __RESOURCE_BONE_H__