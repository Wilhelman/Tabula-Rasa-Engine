#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "trDefs.h"
#include <string>

class Config;

class Resource
{

public:
	enum Type {
		UNKNOWN,

		MESH,
		TEXTURE,
		SCENE
	};

public:
	Resource(UID uid, Resource::Type type);
	virtual ~Resource();
	Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetFile() const;
	const char* GetExportedFile() const;
	bool IsLoadedToMemory() const;
	bool LoadToMemory();
	uint CountReferences() const;
	virtual void Save(Config& config) const;
	virtual void Load(const Config& config);

protected:

	virtual bool LoadInMemory() = 0;

protected:

	UID uid = 0;
	std::string file;
	std::string exported_file;

	Type type = UNKNOWN;
	uint loaded = 0;
};

#endif // __Resource_H__
