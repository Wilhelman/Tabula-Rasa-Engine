
#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
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
	TR_LOG("Loading File Loader");
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
	TR_LOG("Cleaning File Loader");

	// Clean all log streams
	aiDetachAllLogStreams();

	return true;
}

bool trFileLoader::Import3DFile(const char* file_path)
{
	TR_LOG("trFileLoader: Start importing a file with path: %s", file_path);
	
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		App->render->ClearScene();
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

			TR_LOG("trFileLoader: Importing new mesh with %d vertices", mesh_data->num_vertex);

			// Index copy
			if (new_mesh->HasFaces())
			{
				mesh_data->num_faces = new_mesh->mNumFaces;
				mesh_data->num_index = new_mesh->mNumFaces * 3;
				mesh_data->index = new uint[mesh_data->num_index]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						TR_LOG("WARNING, geometry face with != 3 indices!");
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
	
	TR_LOG("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}