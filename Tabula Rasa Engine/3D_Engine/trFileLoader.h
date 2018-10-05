#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include "trModule.h"
#include "trDefs.h"


#include "Assimp/include/cimport.h"




struct Mesh
{
	uint num_index = 0;
	uint* index = nullptr;

	uint num_vertex = 0;
	float* vertex = nullptr;

	float* normals = nullptr;

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
