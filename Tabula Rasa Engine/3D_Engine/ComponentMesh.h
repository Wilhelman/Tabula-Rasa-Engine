#ifndef __COMPONENT_MESH_H__
#define __COMPONENT_MESH_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"


class ComponentMesh : public Component
{
public:

	ComponentMesh(GameObject* embedded_game_object);
	ComponentMesh(GameObject* embedded_game_object, UID resource);
	~ComponentMesh();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	bool SetResource(UID resource);

public:

};

#endif // __COMPONENT_MATERIAL_H__