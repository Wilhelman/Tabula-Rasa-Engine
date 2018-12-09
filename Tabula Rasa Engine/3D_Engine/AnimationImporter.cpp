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
	anim->bone_keys = new ResourceAnimation::BoneTransformation[new_anim->mNumChannels];

	for (uint i = 0; i < new_anim->mNumChannels; ++i)
		ImportBoneTransform(new_anim->mChannels[i], anim->bone_keys[i]);

	(SaveAnimation(anim, output)) ?
		TR_LOG("Saved animation correctly in path: [%s]", output.c_str()) :
		TR_LOG("Error saving animation in path: [%s]", output.c_str());

	anim->SetExportedPath(output.c_str());

	return anim->GetUID();
}

bool AnimationImporter::SaveAnimation(const ResourceAnimation* anim_data, std::string & output) const
{
	uint name_size = anim_data->name.size(); // TODO: check this to do it with a define
	uint duration_size = sizeof(anim_data->duration);
	uint ticks_size = sizeof(anim_data->ticks_per_second);
	uint num_keys = sizeof(anim_data->num_keys);

	uint final_size = name_size + duration_size + ticks_size + num_keys;

	/*for (uint i = 0; i < anim_data->bone_keys.size(); i++)
	{
		uint id_size = sizeof(uint);

		uint name_size = anim_data->bone_keys[i]->bone_name.size(); // TODO: check this to do it with a define
		anim_data->bone_keys[i]->positions.

	}*/

	// Size name
	uint bytes = anim_data->name.size();
	char* cursor = nullptr;

	//uint name_size = (uint)anim_data->name.size();
	memcpy(cursor, &name_size, bytes);

	// Name
	cursor += bytes;
	bytes = name_size;
	memcpy(cursor, &anim_data->name, bytes);

	// Duration
	cursor += bytes;
	bytes = sizeof(double);
	memcpy(cursor, &anim_data->duration, bytes);

	// Ticks per second
	cursor += bytes;
	bytes = sizeof(double);
	memcpy(cursor, &anim_data->ticks_per_second, bytes);

	// Num keys


	// Keys 

	return false;
}

void AnimationImporter::ImportBoneTransform(const aiNodeAnim* anim_node, ResourceAnimation::BoneTransformation& bones_transform) const
{
	// Setting up bone name
	bones_transform.bone_name = anim_node->mNodeName.C_Str();

	// Allocating memory to store bones postions
	bones_transform.positions.Init(ResourceAnimation::BoneTransformation::Key::KeyType::POSITION, anim_node->mNumPositionKeys);

	// Setting up bone positions over time
	for (uint i = 0; i < anim_node->mNumPositionKeys; i++)
	{
		bones_transform.positions.value[i * 3] = anim_node->mPositionKeys[i].mValue.x;
		bones_transform.positions.value[i * 3 + 1] = anim_node->mPositionKeys[i].mValue.y;
		bones_transform.positions.value[i * 3 + 2] = anim_node->mPositionKeys[i].mValue.z;

		bones_transform.positions.time[i] = anim_node->mPositionKeys[i].mTime;
	}

	// Allocating memory to store bones rotations
	bones_transform.positions.Init(ResourceAnimation::BoneTransformation::Key::KeyType::ROTATION, anim_node->mNumRotationKeys);

	// Setting up bone rotations over time
	for (uint i = 0; i < anim_node->mNumRotationKeys; i++)
	{
		bones_transform.rotations.value[i * 3] = anim_node->mRotationKeys[i].mValue.x;
		bones_transform.rotations.value[i * 3 + 1] = anim_node->mRotationKeys[i].mValue.y;
		bones_transform.rotations.value[i * 3 + 2] = anim_node->mRotationKeys[i].mValue.z;
		bones_transform.rotations.value[i * 3 + 3] = anim_node->mRotationKeys[i].mValue.w;

		bones_transform.rotations.time[i] = anim_node->mRotationKeys[i].mTime;
	}

	// Allocating memory to store bones scalings
	bones_transform.positions.Init(ResourceAnimation::BoneTransformation::Key::KeyType::SCALE, anim_node->mNumScalingKeys);

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

		bones_transform.scalings.value[i * 3] = scale.x;
		bones_transform.scalings.value[i * 3 + 1] = scale.y;
		bones_transform.scalings.value[i * 3 + 2] = scale.z;

		bones_transform.scalings.time[i] = anim_node->mScalingKeys[i].mTime;
	}
}

bool AnimationImporter::EqualsWithEpsilon(float number_a, float number_b, float epsilon) const
{
	return fabs(number_a - number_b) < epsilon;
}

UID AnimationImporter::GenerateResourceFromFile(const char * file_path, UID uid_to_force)
{
	// Reading file
	char* buffer = nullptr;
	App->file_system->ReadFromFile(file_path, &buffer);

	// Checking for errors
	if (buffer == nullptr)
	{
		TR_LOG("AnimationImporter: Unable to open file...");
		return false;
	}

	ResourceAnimation* resource = (ResourceAnimation*)App->resources->CreateNewResource(Resource::Type::ANIMATION, uid_to_force);

	char* cursor = buffer;

	// Load anim name
	uint bytes = sizeof(char) * 25; // TODO: put anim name size as a define maybe
	char name[25];
	memcpy(&name, cursor, bytes);
	resource->name = name;

	// Load duration 


	//double duration;
	//double ticks_per_second;
	//uint num_keys = 0;
	//std::vector<BoneTransformation*> bone_keys;

	/*


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