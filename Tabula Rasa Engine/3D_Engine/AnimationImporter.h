#ifndef __ANIMATION_IMPORTER_H__
#define __ANIMATION_IMPORTER_H__

#include "Importer.h"
#include "trDefs.h"
#include "ResourceAnimation.h"

struct aiAnimation;
struct aiNodeAnim;

class ResourceAnimation;

class AnimationImporter : public Importer
{
public:

	AnimationImporter();
	~AnimationImporter();

	bool Import(const char* file_path, std::string& output_file); // TODO SOLVE THIS

	UID GenerateResourceFromFile(const char* file_path, UID uid_to_force = 0u);

	UID Import(const aiAnimation* new_anim, std::string& output);

	bool SaveAnimation(ResourceAnimation* anim, std::string& output);

	void ImportBoneTransform(const aiNodeAnim * anim_node, ResourceAnimation::BoneTransformation& bones_transform) const;

	bool EqualsWithEpsilon(float number_a, float number_b, float epsilon = 1e-3f) const;


};

#endif // __BONE_IMPORTER_H__