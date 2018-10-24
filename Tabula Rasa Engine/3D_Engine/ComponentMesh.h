#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include <vector>

struct Mesh;

class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* embedded_game_object);
	ComponentMesh(GameObject* embedded_game_object, std::vector<Mesh*> meshes);

private:

	std::vector<Mesh*> meshes;

};

#endif // __COMPONENT_MATERIAL_H__