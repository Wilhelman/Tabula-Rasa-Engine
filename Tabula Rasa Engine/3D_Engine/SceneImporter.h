#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Importer.h"
#include <vector>
#include <string>
#include <map>
#include "MathGeoLib/MathGeoLib.h"

class GameObject;
class ComponentMaterial;
class aiScene;
class aiNode;
class aiMaterial;
class aiMesh;
class ResourceMesh;

class SceneImporter : public Importer
{
public:
	SceneImporter();
	~SceneImporter();


public:

	bool Import(const char* path, std::string& output_file);
	//bool Load(const char* exported_file, Texture* resource);

	void ImportNodesRecursively(const aiNode* node, const aiScene* scene, char* file_path, GameObject * parent_go);

	// ---- Import Vic Methods ----

	void ImportMeshesRecursively(const aiNode* node, const aiScene* scene, char* file_path);
	void ImportMaterialsRecursively();
	void GenerateGameObjectsRecursively(const aiNode* src, GameObject* dst);

	// ----------------------------

	ComponentMaterial* LoadTexture(aiMaterial* material, GameObject* go, ResourceMesh* mesh);

	bool SaveMeshFile(const char* file_name, ResourceMesh* mesh_data, std::string& output_file);
	bool LoadMeshFile(const char* file_name, const char* file_path);
	UID GenerateResourceFromFile(const char* file_path, UID uid_to_force = 0u);

private:
	GameObject* imported_root_go = nullptr;

	//used as static
	ComponentMaterial* material_data = nullptr;
	char* cursor_data = nullptr;

	// Tabula Rasa's format
	std::string file_name;

	std::vector<float3> scene_vertices;
	uint scene_num_vertex = 0;

	AABB scene_bb;

	std::map<aiMesh*, ResourceMesh*> mesh_resources;
};

#endif // __MESH_IMPORTER_H__