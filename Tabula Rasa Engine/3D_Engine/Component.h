#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "trDefs.h"

class GameObject;

class Component
{
public:

	enum component_type {
		COMPONENT_UNKNOWN,

		COMPONENT_TRANSFORM,
		COMPONENT_MESH,
		COMPONENT_MATERIAL
	};

public:

	Component(GameObject* embedded_go, component_type type);
	~Component();

	virtual bool Enable();
	virtual bool Update(float dt);
	virtual bool Disable();

	GameObject* GetEmbeddedObject() const;
	bool IsActive()const;

protected:

	bool active = false;
	
	GameObject* embedded_go = nullptr;
	component_type type = component_type::COMPONENT_UNKNOWN;

};

#endif // __GAMEOBJECT_H__