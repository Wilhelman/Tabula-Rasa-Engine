#ifndef __MESH_IMPORTER_H__
#define __MESH_IMPORTER_H__

#include "Importer.h"
#include <vector>
#include <string>
#include "MathGeoLib/MathGeoLib.h"

class GameObject;
class ComponentMaterial;
class aiScene;
class aiNode;
class aiMaterial;
class ResourceMesh;

class MeshImporter : public Importer
{
public:
	MeshImporter();
	~MeshImporter();


public:

	bool Import(const char* path, std::string& output_file);
	//bool Load(const char* exported_file, Texture* resource);

	void ImportNodesRecursively(const aiNode* node, const aiScene* scene, char* file_path, GameObject * parent_go);
	ComponentMaterial* LoadTexture(aiMaterial* material, GameObject* go);

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
};

#endif // __MESH_IMPORTER_H__