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

class MeshImporter;
class MaterialImporter;

class Directory;
class File;

class trResources : public trModule
{
public:
	enum FileState {
		UNKNOWN,

		META_0,
		META_1_FILE_0
	};
public:
	trResources();
	~trResources();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool CleanUp();

	UID Find(const char* file_in_assets) const;

	void CheckForChangesInAssets(Directory* current_dir);
	void TryToImportFile(File* file);
	UID ImportFile(File* file_path, UID forced_uid = 0u);

	void CreateMetaFileFrom(Resource* resource, const char* file_name);
	bool GenerateResourceFromMeta(const char* meta_file);

	Resource::Type TypeFromExtension(const char* extension) const;

	Resource* Get(UID uid);
	Resource* CreateNewResource(Resource::Type type, UID uid_to_force = 0u, 
		const char* file_name = nullptr, const char* imported_path = nullptr, const char* exported_path = nullptr);

private:
	UID last_uid = 1;
	std::map<UID, Resource*> resources;

	MeshImporter* mesh_importer = nullptr;
	MaterialImporter* material_importer = nullptr;

};

#endif // __TR_RESOURCES_H__
