#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trTextures.h"
#include "trCamera3D.h"
#include "trMainScene.h"

#include "GameObject.h"
#include "ComponentMesh.h"

#include <fstream>

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp-vc140-mt.lib")

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
		std::string tmp = file_path;
		// Let's get the file name to print it in inspector:
		const size_t last_slash = tmp.find_last_of("\\/");
		if (std::string::npos != last_slash)
			tmp.erase(0, last_slash + 1);
		const size_t extension = tmp.rfind('.');
		if (std::string::npos != extension)
			tmp.erase(extension);

		GameObject* father = App->main_scene->CreateGameObject(tmp.c_str());

		App->texture->CleanUp(); // Just to remove the last texture when a new mesh is dropped
		App->render->ClearScene(); // Cleaning the last meshes/buffers/etc
		// Removing and cleaning the last AABB
		if (model_bouncing_box != nullptr) { delete model_bouncing_box; model_bouncing_box = nullptr; } /// Should be deleted in preup?
		scene_num_vertex = 0u;
		scene_vertices.clear();

		for (uint i = 0; i < scene->mNumMeshes; i++)
		{
			
			mesh_data = new Mesh(); // our mesh

			mesh_data->name = tmp;
			mesh_data->path = file_path;
			std::string tmp_go_name = father->GetName();
			tmp_go_name.append("("); tmp_go_name.append(std::to_string(i+1)); tmp_go_name.append(")");
			GameObject* go = App->main_scene->CreateGameObject(tmp_go_name.c_str(),father);

			aiMesh* new_mesh = scene->mMeshes[i];

			// Calculate the position, scale and rotation
			if (scene->mRootNode != nullptr)
			{
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
			}
			
			// Material color of the mesh
			aiMaterial* material = scene->mMaterials[new_mesh->mMaterialIndex];
			aiColor4D tmp_color;
			aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &tmp_color);
			mesh_data->ambient_color = new float4(tmp_color.r, tmp_color.g, tmp_color.b, tmp_color.a);

			// Getting the texture path
			if (texture_path.empty()) {
				aiString tmp_path;
				material->GetTexture(aiTextureType_DIFFUSE, 0, &tmp_path);
				texture_path = tmp_path.data;

				// Let's get ONLY the file name:
				const size_t last_slash = texture_path.find_last_of("\\/");
				if (std::string::npos != last_slash)
					texture_path.erase(0, last_slash + 1);
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

			// UVs copy
			if (new_mesh->HasTextureCoords(0)) {//i?
				mesh_data->size_uv = new_mesh->mNumVertices;
				mesh_data->uvs = new float[mesh_data->size_uv * 2];
				for (int i = 0; i < mesh_data->size_uv; i++) {
					mesh_data->uvs[i * 2] = new_mesh->mTextureCoords[0][i].x;
					mesh_data->uvs[i * 2 + 1] = new_mesh->mTextureCoords[0][i].y;
				}
			}
			else {
				mesh_data->size_uv = 0;
				mesh_data->uvs = nullptr;
			}

			// Index copy
			if (new_mesh->HasFaces())
			{
				mesh_data->face_size = new_mesh->mNumFaces;
				mesh_data->index_size = new_mesh->mNumFaces * 3;
				mesh_data->indices = new uint[mesh_data->index_size]; // assume each face is a triangle
				for (uint i = 0; i < new_mesh->mNumFaces; ++i)
				{
					if (new_mesh->mFaces[i].mNumIndices != 3) {
						TR_LOG("WARNING, geometry face with != 3 indices!");
					}
					else
						memcpy(&mesh_data->indices[i * 3], new_mesh->mFaces[i].mIndices, 3 * sizeof(uint));
				}
			}

			// saving file in our own format
			//SaveMeshFile(mesh_data->name.c_str());

			mesh_data->bounding_box = new AABB(vec(0.f, 0.f, 0.f), vec(0.f, 0.f, 0.f));
			mesh_data->bounding_box->Enclose((float3*)mesh_data->vertices, mesh_data->vertex_size);

			ComponentMesh* comp = (ComponentMesh*)go->CreateComponent(Component::component_type::COMPONENT_MESH);
			comp->SetMesh(mesh_data);

			App->render->GenerateBufferForMesh(mesh_data);
		
		}

		// Let's search the texture in our path assets/textures
		if (!texture_path.empty()) { 
			std::string posible_path = "assets/textures/";
			posible_path = posible_path + texture_path;
			TR_LOG("trFileLoader: Search in - %s", posible_path.c_str());
			App->texture->LoadImageFromPath(posible_path.c_str());
		}
		else
			TR_LOG("trFileLoader: Didn't find any embeded texture");

		// Camera AABB stuff
		if (scene->mNumMeshes == 1) // if only one mesh, get the bounding_box of the last mesh
			App->camera->CenterOnScene(mesh_data->bounding_box);
		else { // get the bouncing of all the meshes
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

bool trFileLoader::SaveMeshFile(const char* file_name)
{
	uint size_indices = sizeof(uint) * mesh_data->index_size;
	uint size_vertices = sizeof(float) * mesh_data->vertex_size * 3;
	uint size_uvs = sizeof(float) * mesh_data->size_uv * 2;
	uint size_ambient_color = sizeof(float4);

	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[4] = { mesh_data->index_size, mesh_data->vertex_size, mesh_data->size_uv, 1 };
	uint size = sizeof(ranges) + size_indices + size_vertices + size_uvs + size_ambient_color;

	char* data = new char[size]; // Allocate
	char* cursor = data;

	uint bytes = sizeof(ranges); // First store ranges
	memcpy(cursor, ranges, bytes);
	
	cursor += bytes; // Store indices
	bytes = size_indices;
	memcpy(cursor, mesh_data->indices, bytes);

	cursor += bytes; // Store vertices
	bytes = size_vertices;
	memcpy(cursor, mesh_data->vertices, bytes);

	cursor += bytes; // Store uvs
	bytes = size_uvs;
	memcpy(cursor, mesh_data->uvs, bytes);

	cursor += bytes; // Store ambient color
	bytes = size_ambient_color;
	memcpy(cursor, mesh_data->ambient_color, bytes);

	// Saving file
	std::string tmp_str(file_name);
	tmp_str.append(".tr", 4); // adding our own format extension
	std::ofstream ofile(tmp_str.c_str(), std::ios::out);

	if (ofile.fail())
	{
		TR_LOG("Unable to open file...");
		return false;
	}

	ofile.write(cursor, size);
	ofile.close();

	// deleting useless data
	RELEASE_ARRAY(data);

	return true;
}

bool trFileLoader::LoadMeshFile(const char* file_path)
{
	// Open file rquested file
	std::ifstream in_file;
	in_file.open(file_path, std::ifstream::in);

	// Check for errors
	if (in_file.fail())
	{
		TR_LOG("Unable to open file...");
		return false;
	}

	// Get size of file
	in_file.seekg(0, in_file.end);
	size_t size = in_file.tellg();
	in_file.seekg(0, in_file.beg);

	// Allocate memory for the buffer
	char* data = new char[size];
	char* cursor = data;

	// Copy file data into buffer and close file
	in_file.read(cursor, size);
	in_file.close();

	// amount of indices / vertices / colors / normals / texture_coords
	uint ranges[2];
	uint bytes = sizeof(ranges);
	memcpy(ranges, cursor, bytes);

	mesh_data->index_size = ranges[0];
	mesh_data->vertex_size = ranges[1];

	// Load indices
	cursor += bytes;
	bytes = sizeof(uint) * mesh_data->index_size;
	mesh_data->indices = new uint[mesh_data->index_size]; // CRASH HERE!!!
	memcpy(mesh_data->indices, cursor, bytes);

	// Load vertices
	cursor += bytes;
	bytes = sizeof(uint) * mesh_data->vertex_size;
	mesh_data->vertices = new float[mesh_data->vertex_size];
	memcpy(mesh_data->vertices, cursor, bytes);

	App->render->GenerateBufferForMesh(mesh_data);

	RELEASE_ARRAY(data);

	return true;
}