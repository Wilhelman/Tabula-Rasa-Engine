#include "AnimationImporter.h"

#include "trApp.h"
#include "trFileSystem.h"
#include "Assimp/include/mesh.h"

#include "trResources.h"
#include "ResourceBone.h"

AnimationImporter::AnimationImporter()
{
}

AnimationImporter::~AnimationImporter()
{
}

bool AnimationImporter::Import(const char * file_path, std::string & output_file)
{
	return false;
}

UID AnimationImporter::Import(const aiBone* new_bone, UID mesh, std::string& output) const
{
	bool ret = false;

	if (new_bone == nullptr)
		return ret;

	// Temporary object to make the load/Save process
	ResourceBone* bone = (ResourceBone*)App->resources->CreateNewResource(Resource::Type::BONE);

	bone->mesh_uid = mesh;
	bone->bone_weights_size = new_bone->mNumWeights;
	memcpy(bone->offset_matrix.v, &new_bone->mOffsetMatrix.a1, sizeof(float) * 16);

	bone->bone_weights_indices = new uint[bone->bone_weights_size];
	bone->bone_weights = new float[bone->bone_weights_size];

	for (uint k = 0; k < bone->bone_weights_size; ++k)
	{
		bone->bone_weights_indices[k] = new_bone->mWeights[k].mVertexId;
		bone->bone_weights[k] = new_bone->mWeights[k].mWeight;
	}

	//(SaveBone(bone, output)) ?
		//TR_LOG("Saved bone correctly in path: [%s]", output.c_str()) :
		//TR_LOG("Error saving bone in path: [%s]", output.c_str());

	bone->SetExportedPath(output.c_str());

	return bone->GetUID();
}

UID AnimationImporter::GenerateResourceFromFile(const char * file_path, UID uid_to_force)
{
	// Reading file
	char* buffer = nullptr;
	App->file_system->ReadFromFile(file_path, &buffer);

	// Checking for errors
	if (buffer == nullptr)
	{
		TR_LOG("BoneImporter: Unable to open file...");
		return false;
	}

	char* cursor = buffer;
	ResourceBone* resource = (ResourceBone*)App->resources->CreateNewResource(Resource::Type::BONE, uid_to_force);

	// Load mesh UID
	uint bytes = sizeof(UID);
	memcpy(&resource->mesh_uid, cursor, bytes);

	// Load offset matrix
	cursor += bytes;
	bytes = sizeof(resource->offset_matrix);
	memcpy(resource->offset_matrix.v, cursor, bytes);

	// Load num_weigths
	cursor += bytes;
	bytes = sizeof(resource->bone_weights_size);
	memcpy(&resource->bone_weights_size, cursor, bytes);

	// Allocate mem for indices and weights
	resource->bone_weights_indices = new uint[resource->bone_weights_size];
	resource->bone_weights = new float[resource->bone_weights_size];

	// Read indices
	cursor += bytes;
	bytes = sizeof(uint) * resource->bone_weights_size;
	memcpy(resource->bone_weights_indices, cursor, bytes);

	// Read weigths
	cursor += bytes;
	bytes = sizeof(float) * resource->bone_weights_size;
	memcpy(resource->bone_weights, cursor, bytes);

	RELEASE_ARRAY(buffer);

	return resource->GetUID();
}
