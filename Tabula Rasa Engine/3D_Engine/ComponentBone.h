#ifndef __COMPONENT_BONE_H__
#define __COMPONENT_BONE_H__

#include "Component.h"


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

};

#endif // __COMPONENT_BONE_H__