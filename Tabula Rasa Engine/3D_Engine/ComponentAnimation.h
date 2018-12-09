#ifndef __COMPONENT_ANIMATION_H__
#define __COMPONENT_ANIMATION_H__

#include "Component.h"


class ComponentAnimation : public Component
{
public:

	ComponentAnimation(GameObject* embedded_game_object);
	ComponentAnimation(GameObject* embedded_game_object, UID resource);
	~ComponentAnimation();

	bool Save(JSON_Object* component_obj)const;
	bool Load(const JSON_Object* component_obj);

	bool SetResource(UID resource);

public:

};

#endif // __COMPONENT_ANIMATION_H__