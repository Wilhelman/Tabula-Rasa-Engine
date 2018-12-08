#include "AnimationImporter.h"

#include "trApp.h"
#include "trFileSystem.h"
#include "Assimp/include/anim.h"

#include "trResources.h"
#include "ResourceAnimation.h"

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

UID AnimationImporter::Import(const aiAnimation* new_anim, UID mesh, std::string& output) const
{
	bool ret = false;

	// Making sure the animation exists and has data inside
	if (new_anim == nullptr)
		return ret;

	if (new_anim->mNumChannels == 0)
		return ret;

	// Creating animation resource an filling its data
	ResourceAnimation* anim = (ResourceAnimation*)App->resources->CreateNewResource(Resource::Type::ANIMATION);

	anim->name = new_anim->mName.C_Str();
	anim->duration = new_anim->mDuration;
	anim->ticks_per_second = new_anim->mTicksPerSecond;

	anim->num_keys = new_anim->mNumChannels;
	anim->bone_keys.reserve(anim->num_keys);

	// Once we have the animation data we populate the animation keys with the bones' data
	// for (uint i = 0; i < new_anim->mNumChannels; ++i)
		//ImportBoneTransform(new_anim->mChannels[i], anim.bone_keys[i]);

	(SaveAnimation(anim, output)) ?
		TR_LOG("Saved animation correctly in path: [%s]", output.c_str()) :
		TR_LOG("Error saving animation in path: [%s]", output.c_str());

	anim->SetExportedPath(output.c_str());

	return anim->GetUID();
}

bool AnimationImporter::SaveAnimation(const ResourceAnimation * anim, std::string & output) const
{
	// TODO: save anim here...
	return false;
}

UID AnimationImporter::GenerateResourceFromFile(const char * file_path, UID uid_to_force)
{
	/*
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

	*/

	return 0;
}