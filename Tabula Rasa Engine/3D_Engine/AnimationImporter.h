#ifndef __ANIMATION_IMPORTER_H__
#define __ANIMATION_IMPORTER_H__

#include "Importer.h"
#include "trDefs.h"

class aiBone;
class aiAnimation;

class ResourceAnimation;

class AnimationImporter : public Importer
{
public:

	AnimationImporter();
	~AnimationImporter();

	bool Import(const char* file_path, std::string& output_file); // TODO SOLVE THIS

	UID GenerateResourceFromFile(const char* file_path, UID uid_to_force = 0u);

	UID Import(const aiAnimation* new_anim, UID mesh, std::string& output) const;

	bool SaveAnimation(const ResourceAnimation* anim, std::string& output) const;


};

#endif // __BONE_IMPORTER_H__