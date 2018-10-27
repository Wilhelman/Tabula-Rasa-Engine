#ifndef __TR_MESH_IMPORTER_H__
#define __TR_MESH_IMPORTER_H__

#include "Importer.h"
#include "trModule.h"
#include "trDefs.h"
#include "MathGeoLib\MathGeoLib.h"

struct Mesh;

class trMeshImporter : public Importer, public trModule
{
public:
	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();
	bool CleanUp();

	bool Save(const char* path);

private:

	Mesh* mesh_data = nullptr;

	std::vector<vec> scene_vertices;
	uint scene_num_vertex = 0;
	AABB* model_bouncing_box = nullptr;

};

#endif // __MESH_IMPORTER_H__