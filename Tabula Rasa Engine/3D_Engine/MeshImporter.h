#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "trImporter.h"
#include "trDefs.h"
#include "MathGeoLib\MathGeoLib.h"

struct Mesh;

class MeshImporter : public trImporter
{
public:

	MeshImporter();
	~MeshImporter();

	// Called before render is available
	bool Awake(JSON_Object* config = nullptr);

	// Called before the first frame
	bool Start();
	bool CleanUp();

	bool Import3DFile(const char* file_path);

private:

	Mesh * mesh_data = nullptr;

	std::vector<vec> scene_vertices;
	uint scene_num_vertex = 0;
	AABB* model_bouncing_box = nullptr;

};

#endif // __MESH_IMPORTER_H__