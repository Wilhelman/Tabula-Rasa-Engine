#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include "trModule.h"
#include "trDefs.h"


#include "Assimp/include/cimport.h"




struct Mesh
{
	uint buffer_index = 0u;
	uint num_index = 0u;
	uint* index = nullptr;

	uint buffer_vertex = 0u;
	uint num_vertex = 0u;
	float* vertex = nullptr;

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
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();
	bool CleanUp();

	bool Import3DFile(const char* file_path);

private:

	Mesh mesh_data;

};

#endif // __FILE_LOADER_H__
