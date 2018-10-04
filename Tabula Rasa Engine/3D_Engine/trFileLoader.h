#ifndef __FILE_LOADER_H__
#define __FILE_LOADER_H__

#include <string>
#include <map>
#include "trModule.h"
#include "Math.h"
#include "trDefs.h"

struct MeshData
{
	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;
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

	bool ImportFile(const char* file_path);

private:

	MeshData mesh_data;

};

#endif // __FILE_LOADER_H__
