#include "BoneImporter.h"
#include "trApp.h"
#include "trFileSystem.h"

#include "trResources.h"
#include "ResourceBone.h"

BoneImporter::BoneImporter()
{
}

BoneImporter::~BoneImporter()
{
}

bool BoneImporter::Import(const char * file_path, std::string & output_file)
{
	return false;
}

bool BoneImporter::Import(const char * path, const char * file_name, std::string & output_file, UID & uid_to_force)
{
	// Reading file
	char* buffer = nullptr;
	App->file_system->ReadFromFile(path, &buffer);

	// Checking for errors
	if (buffer == nullptr)
	{
		TR_LOG("BoneImporter: Unable to open file...");
		return false;
	}

	char* cursor = buffer;
	ResourceBone* resource = (ResourceBone*)App->resources->CreateNewResource(Resource::Type::BONE);

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

	return true;
}