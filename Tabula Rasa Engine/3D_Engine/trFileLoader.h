#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include "trModule.h"
#include "trDefs.h"
#include <list>
#include "MathGeoLib\MathGeoLib.h"

#include "Assimp/include/cimport.h"


struct Mesh
{
	uint buffer_index = 0u;
	uint num_index = 0u;
	uint* index = nullptr;

	uint buffer_vertex = 0u;
	uint num_vertex = 0u;
	float* vertex = nullptr;

	uint buffer_uv = 0u;
	uint num_uv = 0u;
	float* uv = nullptr;

	float* normals = nullptr;
	float* normal_faces = nullptr;
	uint num_faces = 0;

	float* colors = nullptr;
	aiColor4D mat_color;
};

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

	bool Import3DFile(const char* file_path);

private:

	Mesh* mesh_data = nullptr;
	
	std::vector<float*> scene_vertices;
	uint scene_num_vertex = 0;


};

#endif // __FILE_LOADER_H__
