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
	virtual ~Resource() {}
	Resource::Type GetType() const;
	UID GetUID() const;
	const char* GetFileName() const;
	const char* GetImportedFile() const;
	const char* GetExportedFile() const;
	void SetFileName(const char* file_name);
	void SetImportedPath(const char* imported_path);
	void SetExportedPath(const char* exported_path);
	bool IsLoadedToMemory() const;
	uint LoadToMemory();
	uint CountReferences() const;
	//virtual void Save(Config& config) const;
	//virtual void Load(const Config& config);

protected:

	virtual bool LoadInMemory() = 0;

protected:

	UID uid = 0;
	std::string file_name;
	std::string imported_path;
	std::string exported_path;

	Type type = UNKNOWN;
	uint references = 0u;
};

#endif // __Resource_H__
