
#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void StreamCallback(const char* msg, char* user_msg) {
	TR_LOG("trFileLoader: %s",msg);
}

trFileLoader::trFileLoader()
{
}

trFileLoader::~trFileLoader()
{}

bool trFileLoader::Awake(JSON_Object* config)
{
	TR_LOG("trFileLoader: Loading File Loader");
	bool ret = true;

	// Stream log messages to Debug window
	struct aiLogStream stream;
	stream.callback = StreamCallback;
	aiAttachLogStream(&stream);
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	
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
			aiColor4D tmp_color;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &tmp_color);
			mesh_data->ambient_color = new float4(tmp_color.r, tmp_color.g, tmp_color.b, tmp_color.a);

			// Vertex copy
			mesh_data->vertex_size = new_mesh->mNumVertices;
			mesh_data->vertices = new float[mesh_data->vertex_size * 3];
			memcpy(mesh_data->vertices, new_mesh->mVertices, sizeof(float) * mesh_data->vertex_size * 3);

			for (uint i = 0; i < mesh_data->vertex_size; i++)
				scene_vertices.push_back(&mesh_data->vertices[i]);

			scene_num_vertex += mesh_data->vertex_size;

			// Textures copy
			if (new_mesh->HasTextureCoords(0)) {//i?
				mesh_data->size_uv = new_mesh->mNumVertices;
				mesh_data->uvs = new float[mesh_data->size_uv * 2];
				for (int i = 0; i < mesh_data->size_uv; i++) {
					mesh_data->uvs[i * 2] = new_mesh->mTextureCoords[0][i].x;
					mesh_data->uvs[i * 2 + 1] = new_mesh->mTextureCoords[0][i].y;
				}
			}

			// Vertices' colors copy
			aiColor4D* mesh_colors = *new_mesh->mColors;

			TR_LOG("trFileLoader: Importing new mesh with %d vertices", mesh_data->vertex_size);

			// Index copy
			if (new_mesh->HasFaces())
			{
				mesh_data->index_size = new_mesh->mNumFaces * 3;
				mesh_data->indices = new uint[mesh_data->index_size]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3)
						TR_LOG("WARNING, geometry face with != 3 indices!");
					else
						memcpy(&mesh_data->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}

			mesh_data->bounding_box = new AABB(vec(0.f, 0.f, 0.f), vec(0.f, 0.f, 0.f));
			mesh_data->bounding_box->Enclose((float3*)mesh_data->vertices, mesh_data->vertex_size);

			App->render->GenerateBufferForMesh(mesh_data);
		}

		aiReleaseImport(scene);

		

		return true;
	}
	
	TR_LOG("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}