#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include "trModule.h"
#include "trDefs.h"
#include "MathGeoLib\MathGeoLib.h"

struct Mesh;

class MeshImporter;
class MaterialImporter;

class trFileLoader : public trModule
{
public:

	trFileLoader();
	~trFileLoader();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();
	bool CleanUp();

	void ImportFBX(const char* file_path);
	void ImportTexture(const char* file_path);
	void ImportScene(const char* file_path);

	bool LoadMeshFile(const char* file_path);

public:
	MeshImporter* mesh_importer = nullptr;
	MaterialImporter* material_importer = nullptr;

};

#endif // __FILE_LOADER_H__
