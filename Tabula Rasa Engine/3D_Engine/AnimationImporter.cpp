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
	// Making sure the animation exists and has data inside
	if (new_anim == nullptr)
		return false;

	if (new_anim->mNumChannels == 0)
		return false;

	// Creating animation resource an filling its data
	ResourceAnimation* anim = (ResourceAnimation*)App->resources->CreateNewResource(Resource::Type::ANIMATION);

	anim->name = new_anim->mName.C_Str();
	anim->duration = new_anim->mDuration;
	anim->ticks_per_second = new_anim->mTicksPerSecond;

	anim->num_keys = new_anim->mNumChannels;

	// Once we have the animation data we populate the animation keys with the bones' data
	anim->bone_keys.reserve(anim->num_keys);

	for (uint i = 0; i < new_anim->mNumChannels; ++i)
		ImportBoneTransform(new_anim->mChannels[i], *anim->bone_keys[i]);

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

void AnimationImporter::ImportBoneTransform(const aiNodeAnim * anim_node, ResourceAnimation::BoneTransformation& bones_transform) const
{
	// Setting up bone name
	bones_transform.bone_name = anim_node->mNodeName.C_Str();

	// Allocating memory to store bones postions
	bones_transform.position.Init(ResourceAnimation::BoneTransformation::Key::KeyType::POSITION, anim_node->mNumPositionKeys);

	// Setting up bone positions over time
	for (uint i = 0; i < anim_node->mNumPositionKeys; i++)
	{
		bones_transform.position.value[i * 3] = anim_node->mPositionKeys[i].mValue.x;
		bones_transform.position.value[i * 3 + 1] = anim_node->mPositionKeys[i].mValue.y;
		bones_transform.position.value[i * 3 + 2] = anim_node->mPositionKeys[i].mValue.z;

		bones_transform.position.time[i] = anim_node->mPositionKeys[i].mTime;
	}

	// Allocating memory to store bones rotations
	bones_transform.position.Init(ResourceAnimation::BoneTransformation::Key::KeyType::ROTATION, anim_node->mNumRotationKeys);

	// Setting up bone rotations over time
	for (uint i = 0; i < anim_node->mNumRotationKeys; i++)
	{
		bones_transform.rotation.value[i * 3] = anim_node->mRotationKeys[i].mValue.x;
		bones_transform.rotation.value[i * 3 + 1] = anim_node->mRotationKeys[i].mValue.y;
		bones_transform.rotation.value[i * 3 + 2] = anim_node->mRotationKeys[i].mValue.z;
		bones_transform.rotation.value[i * 3 + 3] = anim_node->mRotationKeys[i].mValue.w;

		bones_transform.rotation.time[i] = anim_node->mRotationKeys[i].mTime;
	}

	// Allocating memory to store bones scalings
	bones_transform.position.Init(ResourceAnimation::BoneTransformation::Key::KeyType::SCALE, anim_node->mNumScalingKeys);

	// Setting up bone positions over time
	for (uint i = 0; i < anim_node->mNumScalingKeys; i++)
	{
		float3 scale(anim_node->mScalingKeys[i].mValue.x, 
					 anim_node->mScalingKeys[i].mValue.y, 
					 anim_node->mScalingKeys[i].mValue.z);
		
		// Forcing scale to be 1 if it's close to it to avoid floating-point errors
		// Note(victor): current epsilon is set to 1e-3f
		if (EqualsWithEpsilon(scale.x, 1.0f))
			scale.x = 1.0f;
		if (EqualsWithEpsilon(scale.y, 1.0f))
			scale.y = 1.0f;
		if (EqualsWithEpsilon(scale.z, 1.0f))
			scale.z = 1.0f;

		bones_transform.scale.value[i * 3] = scale.x;
		bones_transform.scale.value[i * 3 + 1] = scale.y;
		bones_transform.scale.value[i * 3 + 2] = scale.z;

		bones_transform.scale.time[i] = anim_node->mScalingKeys[i].mTime;
	}
}

bool AnimationImporter::EqualsWithEpsilon(float number_a, float number_b, float epsilon) const
{
	return fabs(number_a - number_b) < epsilon;
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