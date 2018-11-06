#include "trFileLoader.h"

#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trTextures.h"
#include "trCamera3D.h"
#include "trMainScene.h"
#include "trEditor.h"

#include "GameObject.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"

#include "MathGeoLib/MathGeoLib.h"

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

	// Clean all log streams
	aiDetachAllLogStreams();

	return true;
}

bool trFileLoader::Import(const void * buffer, uint size, const char* file_path)
{
	TR_LOG("trFileLoader: Start importing a file with path: %s", file_path);

	if (size <= 0 || buffer != nullptr) {
		TR_LOG("trFileLoader: Error importing in our format file");
		return false;
	}

	const aiScene* scene = aiImportFileFromMemory((const char*)buffer, size, aiProcessPreset_TargetRealtime_MaxQuality, nullptr);

	if (scene != nullptr) {
		App->main_scene->ClearScene();
		// Removing and cleaning the last AABB
		scene_num_vertex = 0u;
		scene_vertices.clear();

		material_data = nullptr;
		ImportNodesRecursively(scene->mRootNode, scene, App->main_scene->GetRoot(), (char*)file_path);
		material_data = nullptr;

		// Camera AABB stuff
		if (scene->mNumMeshes == 1) { // if only one mesh, get the bounding_box of the last mesh
			App->editor->SetSelected(model_root);
			App->camera->dummy_camera->FocusOnSelectedGO();
		}
		else { // get the bouncing of all the meshes
			//TODO CALCULATE HERE THE AABB OF ALL THE SCENE
			model_bouncing_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
			model_bouncing_box.Enclose((float3*)&scene_vertices.front(), scene_num_vertex);
			model_root->bounding_box = model_bouncing_box;
			App->editor->SetSelected(model_root);
			App->camera->dummy_camera->FocusOnSelectedGO();
		}

		App->main_scene->GetRoot()->RecalculateBoundingBox();
		for (std::list<GameObject*>::iterator it = App->main_scene->GetRoot()->childs.begin(); it != App->main_scene->GetRoot()->childs.end(); it++) {
			if ((*it)->is_static) {
				App->main_scene->InsertGoInQuadtree((*it));
			}
			else {
				App->main_scene->EraseGoInQuadtree((*it));
			}
		}
		aiReleaseImport(scene);

		return true;

	}

	TR_LOG("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}

bool trFileLoader::Import(const char* file_path)
{
	TR_LOG("trFileLoader: Start importing a file with path: %s", file_path);
	
	const aiScene* scene = aiImportFile(file_path, aiProcessPreset_TargetRealtime_MaxQuality);
	

	if (scene != nullptr) {
		App->main_scene->ClearScene();
		// Removing and cleaning the last AABB
		scene_num_vertex = 0u;
		scene_vertices.clear();
		cursor_data = nullptr;
		material_data = nullptr;
		ImportNodesRecursively(scene->mRootNode, scene, App->main_scene->GetRoot(), (char*)file_path);

		// Camera AABB stuff
		if (scene->mNumMeshes == 1) { // if only one mesh, get the bounding_box of the last mesh
			App->editor->SetSelected(model_root);
			App->camera->dummy_camera->FocusOnSelectedGO();
		}
		else { // get the bouncing of all the meshes
			model_bouncing_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
			model_bouncing_box.Enclose((float3*)&scene_vertices.front(), scene_num_vertex);
			model_root->bounding_box = model_bouncing_box;
			App->editor->SetSelected(model_root);
			App->camera->dummy_camera->FocusOnSelectedGO();
		}

		App->main_scene->GetRoot()->RecalculateBoundingBox();

		for (std::list<GameObject*>::iterator it = App->main_scene->GetRoot()->childs.begin(); it != App->main_scene->GetRoot()->childs.end(); it++) {
			if ((*it)->is_static) {
				App->main_scene->InsertGoInQuadtree((*it));
			}
			else {
				App->main_scene->EraseGoInQuadtree((*it));
			}
		}

		aiReleaseImport(scene);

		return true;

	}
	
	TR_LOG("trFileLoader: Error importing a file: %s", file_path);

	aiReleaseImport(scene);

	return false;
}

void trFileLoader::ImportNodesRecursively(const aiNode * node, const aiScene * scene, GameObject * parent_go, char* file_path)
{
	
	GameObject* new_go = nullptr;

	if (node->mNumMeshes > 0) //if this node have a mesh
	{
	new_go = App->main_scene->CreateGameObject(node->mName.C_Str(), parent_go);
	std::string tmp = "";
	if (file_path != nullptr) {
		tmp = file_path;
		// Let's get the file name to print it in inspector:
		const size_t last_slash = tmp.find_last_of("\\/");
		if (std::string::npos != last_slash)
			tmp.erase(0, last_slash + 1);
		const size_t extension = tmp.rfind('.');
		if (std::string::npos != extension)
			tmp.erase(extension);
		file_name = tmp;
		//new_go->SetName(tmp.c_str());
		model_root = new_go;
		file_path = nullptr;
	}

	// Calculate the position, scale and rotation
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	float3 quat_to_euler = rot.ToEulerXYZ(); // transforming it to euler to show it in inspector

	ComponentTransform* transform_comp = (ComponentTransform*)new_go->CreateComponent(Component::component_type::COMPONENT_TRANSFORM);
	transform_comp->Setup(float3(translation.x, translation.y, translation.z), float3(scaling.x, scaling.y, scaling.z), rot);

		mesh_data = new Mesh(); // our mesh

		aiMesh* new_mesh = scene->mMeshes[node->mMeshes[0]];

		// Getting texture material if needed	
		if (scene->mMaterials[new_mesh->mMaterialIndex] != nullptr) {
			if(material_data == nullptr)
				material_data = LoadTexture(scene->mMaterials[new_mesh->mMaterialIndex], new_go);
			else
				material_data = (ComponentMaterial*)new_go->CreateComponent(Component::component_type::COMPONENT_MATERIAL, material_data);
		}

		// Vertex copy
		mesh_data->vertex_size = new_mesh->mNumVertices;
		mesh_data->vertices = new float[mesh_data->vertex_size * 3];
		memcpy(mesh_data->vertices, new_mesh->mVertices, sizeof(float) * mesh_data->vertex_size * 3);

		// Data for the bounding box of all the meshes
		for (uint i = 0; i < mesh_data->vertex_size; i++) {
			scene_vertices.push_back(float3(mesh_data->vertices[i], mesh_data->vertices[i + 1], mesh_data->vertices[i + 2]));
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

		// Generating bounding box
		new_go->bounding_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
		new_go->bounding_box.Enclose((float3*)mesh_data->vertices, mesh_data->vertex_size);

		ComponentMesh* mesh_comp = (ComponentMesh*)new_go->CreateComponent(Component::component_type::COMPONENT_MESH);
		mesh_comp->SetMesh(mesh_data);

		//testing zone
		// check if there is already a file
		// todo finish this
		//SaveMeshFile(file_name.c_str(), mesh_data);
		
	}
	for (uint i = 0; i < node->mNumChildren; i++)
		ImportNodesRecursively(node->mChildren[i], scene, new_go, file_path);
	

}


ComponentMaterial * trFileLoader::LoadTexture(aiMaterial* material, GameObject* go)
{
	// Getting the texture path
	aiString tmp_path;
	std::string texture_path;
	material->GetTexture(aiTextureType_DIFFUSE, 0, &tmp_path);
	texture_path = tmp_path.data;

	// Let's get ONLY the file name:
	const size_t last_slash = texture_path.find_last_of("\\/");
	if (std::string::npos != last_slash)
		texture_path.erase(0, last_slash + 1);
	

	// Let's search the texture in our path assets/textures
	if (!texture_path.empty()) {
		std::string posible_path = "assets/textures/";
		posible_path = posible_path + texture_path;
		TR_LOG("trFileLoader: Search in - %s", posible_path.c_str());
		ComponentMaterial* material_comp = (ComponentMaterial*)go->CreateComponent(Component::component_type::COMPONENT_MATERIAL);
		material_comp->SetTexture(App->texture->LoadImageFromPath(posible_path.c_str()));

		// Material color of the mesh
		aiColor4D tmp_color;
		aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &tmp_color);
		material_comp->SetAmbientColor(float4(tmp_color.r, tmp_color.g, tmp_color.b, tmp_color.a));

		return material_comp;
	}
	else {
		TR_LOG("trFileLoader: Didn't find any embeded texture");
		return nullptr;
	}
	
}

bool trFileLoader::SaveMeshFile(const char* file_name, Mesh* mesh_data)
{
	uint size_indices = sizeof(uint) * mesh_data->index_size;
	uint size_vertices = sizeof(float) * mesh_data->vertex_size * 3;
	uint size_uvs = sizeof(float) * mesh_data->size_uv * 2;

	// amount of indices / vertices / colors / normals / texture_coords / AABB
	uint ranges[4] = { mesh_data->index_size, mesh_data->vertex_size, mesh_data->size_uv, 1 };
	uint size = sizeof(ranges) + size_indices + size_vertices + size_uvs;

	char* data = new char[size]; // Allocate
	char* cursor = nullptr;
	if (cursor_data)
		cursor = cursor_data;
	else
		cursor = data;

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

	//before saving, lets point at the end with omega-cursor
	cursor_data = cursor;
	cursor_data += bytes;

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

	RELEASE_ARRAY(data);

	return true;
}