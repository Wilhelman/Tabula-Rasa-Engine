#ifndef __COMPONENT_BONE_H__
#define __COMPONENT_BONE_H__

#include "Component.h"

class ComponentMesh;

class ComponentBone : public Component
{
public:

	ComponentBone(GameObject* embedded_game_object);
	ComponentBone(GameObject* embedded_game_object, UID resource);
	~ComponentBone();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	bool SetResource(UID resource);

public:

	ComponentMesh* attached_mesh = nullptr;

};

#endif // __COMPONENT_BONE_H__