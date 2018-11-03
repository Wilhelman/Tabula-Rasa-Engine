#ifndef __MATERIAL_IMPORTER_H__
#define __MATERIAL_IMPORTER_H__

#include "Importer.h"

class trMaterialImporter : public Importer
{
public:
	trMaterialImporter() {}
	~trMaterialImporter() {}


public:

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	//bool Load(const char* exported_file, Texture* resource);

};

#endif // __MATERIAL_IMPORTER_H__