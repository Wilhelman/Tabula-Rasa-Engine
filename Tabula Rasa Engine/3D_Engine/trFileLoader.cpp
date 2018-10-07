
#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trEditor.h"
#include "trRenderer3D.h"

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

	delete[] mesh_data->index;
	delete[] mesh_data->vertex;
	delete[] mesh_data->normals;
	delete[] mesh_data->colors;
	
	mesh_data->index = nullptr;
	mesh_data->vertex = nullptr;
	mesh_data->normals = nullptr;
	mesh_data->colors = nullptr;

	delete mesh_data;
	mesh_data = nullptr;

	return true;
}

bool trFileLoader::Import3DFile(const char* file_path)
{
	App->editor->Log("trFileLoader: Start importing a file with path: %s", file_path);
	
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);


	if (scene != nullptr && scene->HasMeshes())
	{
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			mesh_data = new Mesh(); // our mesh

			aiMesh* new_mesh = scene->mMeshes[i];
			
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &mesh_data->mat_color);

			
			// Reset the buffers index!!!
			mesh_data->buffer_index = 0u;
			mesh_data->buffer_vertex = 0u;

			// Vertex copy
			mesh_data->num_vertex = new_mesh->mNumVertices;
			mesh_data->vertex = new float[mesh_data->num_vertex * 3];
			memcpy(mesh_data->vertex, new_mesh->mVertices, sizeof(float) * mesh_data->num_vertex * 3);

			// Vertices' normals copy
			aiVector3D* mesh_normals = new_mesh->mNormals;
			mesh_data->normals = new float[mesh_data->num_vertex * 3];

			// Vertices' colors copy
			aiColor4D* mesh_colors = *new_mesh->mColors;
			mesh_data->colors = new float[mesh_data->num_vertex * 3];

			//App->editor->Log("New mesh with %d vertices", mesh_data.num_vertex); ep

			// Index copy
			if (new_mesh->HasFaces())
			{
				mesh_data->num_faces = new_mesh->mNumFaces;
				mesh_data->num_index = new_mesh->mNumFaces * 3;
				mesh_data->index = new uint[mesh_data->num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						App->editor->Log("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&mesh_data->index[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

				// Getting vertices' normals coordinates from loaded mesh
				for (int i = 0; i < mesh_data->num_vertex && mesh_normals != nullptr; i++)
				{
					memcpy(mesh_data->normals, mesh_normals, sizeof(float) * mesh_data->num_vertex * 3);

					if (mesh_colors != nullptr)
						memcpy(mesh_data->colors, &mesh_colors[i], sizeof(float) * mesh_data->num_vertex * 3);
				}
			}

			App->render->GenerateBufferForMesh(mesh_data);
		}

		aiReleaseImport(scene);
		return true;
	}
	
	App->editor->Log("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}
