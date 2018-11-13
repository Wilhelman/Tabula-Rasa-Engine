#ifndef __TR_RESOURCES_H__
#define __TR_RESOURCES_H__

#include "trDefs.h"
#include "trModule.h"
#include "Resource.h"

#include <map>
#include <vector>

class ResourceMesh;
class ResourceTexture;

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
	UID ImportFile(const char* new_file_in_assets, bool force = false);
	UID GenerateNewUID();
	const Resource* Get(UID uid) const;
	Resource* Get(UID uid);
	Resource* CreateNewResource(Resource::Type type, UID force_uid = 0);

private:
	UID last_uid = 1;
	std::map<UID, Resource*> resources;

};

#endif // __TR_RESOURCES_H__
