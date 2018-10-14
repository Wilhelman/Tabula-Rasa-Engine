#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trTextures.h"
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

	if (model_bouncing_box != nullptr) {delete model_bouncing_box; model_bouncing_box = nullptr;}
	// Clean all log streams
	aiDetachAllLogStreams();

	return true;
}

bool trFileLoader::Import3DFile(const char* file_path)
{
	TR_LOG("trFileLoader: Start importing a file with path: %s", file_path);
	
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	std::string texture_path;
	if (scene != nullptr && scene->HasMeshes())
	{
		App->texture->CleanUp(); // Just to remove the last texture when a new mesh is dropped
		App->render->ClearScene(); // Cleaning the last meshes/buffers/etc
		// Removing and cleaning the last AABB
		if (model_bouncing_box != nullptr) { delete model_bouncing_box; model_bouncing_box = nullptr; } /// Should be deleted in preup?
		scene_num_vertex = 0u;
		scene_vertices.clear();
		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			mesh_data = new Mesh(); // our mesh
			aiTexture** tex = scene->mTextures;
			//(**tex).
			std::string tmp = file_path;
			// Let's get the file name to print it in inspector:
			const size_t last_slash = tmp.find_last_of("\\/");
			if (std::string::npos != last_slash)
				tmp.erase(0, last_slash + 1);
			const size_t extension = tmp.rfind('.');
			if (std::string::npos != extension)
				tmp.erase(extension);

			mesh_data->name = tmp;
			mesh_data->path = file_path;

			aiMesh* new_mesh = scene->mMeshes[i];

			// Calculate the position, scale and rotation
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			aiNode* node = scene->mRootNode;
			node->mTransformation.Decompose(scaling, rotation, translation);
			Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
			float3 quat_to_euler = rot.ToEulerXYZ(); // transforming it to euler to show it in inspector
			mesh_data->position.Set(translation.x, translation.y, translation.z);
			mesh_data->scale.Set(scaling.x, scaling.y, scaling.z);
			mesh_data->rotation.Set(quat_to_euler.x, quat_to_euler.y, quat_to_euler.z);
			
			// Material color of the mesh
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			aiColor4D tmp_color;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &tmp_color);
			mesh_data->ambient_color = new float4(tmp_color.r, tmp_color.g, tmp_color.b, tmp_color.a);

			// Getting the texture path
			if (texture_path.empty()) {
				TR_LOG("trFileLoader: Trying to find the embeded texture ...");
				aiString tmp_path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &tmp_path);
				texture_path = tmp_path.data;
			}

			// Vertex copy
			mesh_data->vertex_size = new_mesh->mNumVertices;
			mesh_data->vertices = new float[mesh_data->vertex_size * 3];
			memcpy(mesh_data->vertices, new_mesh->mVertices, sizeof(float) * mesh_data->vertex_size * 3);

			// Data for the bounding box of all the meshes
			for (uint i = 0; i < mesh_data->vertex_size; i++) {
				scene_vertices.push_back(vec(mesh_data->vertices[i], mesh_data->vertices[i + 1], mesh_data->vertices[i + 2]));
			}
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

			// Index copy
			if (new_mesh->HasFaces())
			{
				mesh_data->face_size = new_mesh->mNumFaces;
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

		if (!texture_path.empty()) { // Let's search the texture in our path assets/textures
			std::string posible_path = "assets/textures/";
			posible_path = posible_path + texture_path;
			TR_LOG("trFileLoader: Search in - %s", posible_path.c_str());
			App->texture->LoadImageFromPath(posible_path.c_str());
		}
		else
			TR_LOG("trFileLoader: Didn't find any embeded texture");

		if (scene->mNumMeshes == 1)
			App->camera->CenterOnScene(mesh_data->bounding_box);
		else {
			model_bouncing_box = new AABB(vec(0.f, 0.f, 0.f), vec(0.f, 0.f, 0.f));
			model_bouncing_box->Enclose((vec*)&scene_vertices.front(), scene_num_vertex);
			App->camera->CenterOnScene(model_bouncing_box);
		}
		
		
		aiReleaseImport(scene);

		return true;
	}
	
	TR_LOG("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}