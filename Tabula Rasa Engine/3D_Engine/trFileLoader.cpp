
#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trEditor.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

trFileLoader::trFileLoader()
{
}

trFileLoader::~trFileLoader()
{}

bool trFileLoader::Awake(pugi::xml_node &)
{
	App->editor->Log("Loading File Loader");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);

	return ret;
}

bool trFileLoader::Start()
{

	return true;
}

// Called before quitting or switching levels
bool trFileLoader::CleanUp()
{
	App->editor->Log("Cleaning File Loader");

	// Clean all log streams
	aiDetachAllLogStreams();

	return true;
}

bool trFileLoader::ImportFile(const char* file_path)
{
	App->editor->Log("trFileLoader: Start importing a file with path: %s", file_path);

	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* new_mesh = scene->mMeshes[i];

			/*mesh_data.num_vertex = new_mesh->mNumVertices;
			mesh_data.vertex = new float[mesh_data.num_vertex * 3];
			memcpy(m.vertices, new_mesh->mVertices, sizeof(float) * m.num_vertices * 3);
			App->editor->Log("New mesh with %d vertices", m.num_vertices);*/
		}
		
		aiReleaseImport(scene);
		return true;
	}
	
	App->editor->Log("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}
