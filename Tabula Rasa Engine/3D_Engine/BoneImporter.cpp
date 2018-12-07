#include "BoneImporter.h"
#include "trApp.h"
#include "trFileSystem.h"
#include "Assimp/include/mesh.h"

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

bool BoneImporter::Import(const aiBone* new_bone, UID mesh, std::string& output) const
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

	(SaveBone(bone, output)) ?
		TR_LOG("Saved bone correctly in path: [%s]", output.c_str()) : 
		TR_LOG("Error saving bone in path: [%s]", output.c_str());

	return bone->GetUID();
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

bool BoneImporter::SaveBone(const ResourceBone* bone, std::string& output) const
{
	bool ret = true;

	// Format: mesh UID + 16 float matrix + num_weigths uint + indices uint * num_weight + weight float * num_weights
	uint size = sizeof(bone->mesh_uid);
	size += sizeof(bone->offset_matrix);
	size += sizeof(bone->bone_weights_size);
	size += sizeof(uint) * bone->bone_weights_size;
	size += sizeof(float) * bone->bone_weights_size;

	// allocate mem
	char* data = new char[size];
	char* cursor = data;

	// store mesh UID
	uint bytes = sizeof(bone->mesh_uid);
	memcpy(cursor, &bone->mesh_uid, bytes);

	// store offset matrix
	cursor += bytes;
	bytes = sizeof(bone->offset_matrix);
	memcpy(cursor, &bone->offset_matrix.v, bytes);

	// store num_weights
	cursor += bytes;
	bytes = sizeof(bone->bone_weights_size);
	memcpy(cursor, &bone->bone_weights_size, bytes);

	// store indices
	cursor += bytes;
	bytes = sizeof(uint) * bone->bone_weights_size;
	memcpy(cursor, bone->bone_weights_indices, bytes);

	// store weights
	cursor += bytes;
	bytes = sizeof(float) * bone->bone_weights_size;
	memcpy(cursor, bone->bone_weights, bytes);

	// Saving file
	std::string tmp_str(L_BONES_DIR);
	tmp_str.append("/");
	tmp_str.append(std::to_string(bone->GetUID()));
	tmp_str.append(".trBone"); // Adding our own format extension

	App->file_system->WriteInFile(tmp_str.c_str(), data, size);
	output = tmp_str;

	// Deleting useless data
	RELEASE_ARRAY(data);

	return ret;
}