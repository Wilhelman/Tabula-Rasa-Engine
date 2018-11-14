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

class trResources : public trModule
{
public:
	trResources();
	~trResources();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool CleanUp();

	UID Find(const char* file_in_assets) const;

	void CheckForChangesInAssets(Directory* current_dir);
	void TryToImportFile(const char* file);
	UID ImportFile(const char* file_path);

	Resource::Type TypeFromExtension(const char* extension) const;
	UID GenerateNewUID();
	const Resource* Get(UID uid) const;
	Resource* Get(UID uid);
	Resource* CreateNewResource(Resource::Type type, UID force_uid = 0);

private:
	UID last_uid = 1;
	std::map<UID, Resource*> resources;

	MeshImporter* mesh_importer = nullptr;
	MaterialImporter* material_importer = nullptr;

};

#endif // __TR_RESOURCES_H__
