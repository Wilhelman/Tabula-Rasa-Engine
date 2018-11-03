#ifndef __SCENE_IMPORTER_H__
#define __SCENE_IMPORTER_H__

#include "Importer.h"

class trSceneImporter : public Importer
{
public:
	trSceneImporter() {}
	~trSceneImporter() {}


public:

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	//bool Load(const char* exported_file, Texture* resource);

};

#endif // __SCENE_IMPORTER_H__