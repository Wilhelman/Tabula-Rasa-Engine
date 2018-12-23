#ifndef __TR_RESOURCES_H__
#define __TR_RESOURCES_H__

#include "trDefs.h"
#include "trModule.h"
#include "Resource.h"

#include <map>
#include <vector>
#include <string>

class ResourceMesh;
class ResourceTexture;

class SceneImporter;
class MaterialImporter;
class BoneImporter;
class AnimationImporter;

class Directory;
class File;

class trResources : public trModule
{
public:
	trResources();
	~trResources();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool CleanUp();
	bool PostUpdate(float dt);

	UID Find(const char* file_in_assets) const;

	void Delete(Resource* res_to_delete);

	void CheckForChangesInAssets(Directory* current_dir);
	UID TryToImportFile(File* file);
	UID ImportFile(File* file_path, UID forced_uid = 0u);

	void CreateMetaFileFrom(Resource* resource, File* file_name);
	UID GenerateResourceFromFile(const char* meta_file, File* file);

	Resource::Type TypeFromExtension(const char* extension) const;

	Resource* Get(UID uid);
	Resource* CreateNewResource(Resource::Type type, UID uid_to_force = 0u, 
		const char* file_name = nullptr, const char* imported_path = nullptr, const char* exported_path = nullptr);

private:
	UID last_uid = 1;
	std::map<UID, Resource*> resources;

public:
	SceneImporter* mesh_importer = nullptr;
	MaterialImporter* material_importer = nullptr;
	BoneImporter* bone_importer = nullptr;
	AnimationImporter* animation_importer = nullptr;

};

#endif // __TR_RESOURCES_H__
