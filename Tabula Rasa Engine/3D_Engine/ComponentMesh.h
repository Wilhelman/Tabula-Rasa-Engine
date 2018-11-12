#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"

struct Mesh
{
	std::string path;

	uint index_buffer = 0u;
	uint index_size = 0u;
	uint* indices = nullptr;

	uint face_size = 0u;

	uint vertex_buffer = 0u;
	uint vertex_size = 0u;
	float* vertices = nullptr;

	uint uv_buffer = 0u;
	uint size_uv = 0u;
	float* uvs = nullptr;

	~Mesh();
};

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* embedded_game_object);
	ComponentMesh(GameObject* embedded_game_object, Mesh* mesh);
	~ComponentMesh();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	void GenerateAndBindMesh(Mesh* mesh);

	void SetMesh(Mesh* mesh);
	const Mesh* GetMesh() const;

private:

	Mesh* mesh = nullptr;

};

#endif // __COMPONENT_MATERIAL_H__