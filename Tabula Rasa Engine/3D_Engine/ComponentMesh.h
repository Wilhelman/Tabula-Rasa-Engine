#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"

struct Mesh;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* embedded_game_object);
	ComponentMesh(GameObject* embedded_game_object, Mesh* mesh);

	// todo clean here the mesh
	void GenerateAndBindMesh(Mesh* mesh);

	void SetMesh(Mesh* mesh);
	const Mesh* GetMesh() const;

private:

	Mesh* mesh;

};

#endif // __COMPONENT_MATERIAL_H__