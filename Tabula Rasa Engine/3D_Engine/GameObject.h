#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "trDefs.h"

#include "Component.h"

#include <string>
#include <list>


class GameObject
{
public:
	GameObject();
	GameObject(const char* name, GameObject* parent);
	~GameObject();

	bool Update(float dt);

	Component* CreateComponent(Component::component_type type);
	Component* CreateComponent(Component::component_type type, Component* component);

	GameObject* GetParent() const;
	const char* GetName() const;

	bool IsActive()const;

	void DestroyGameObjectsIfNeeded();

private:

	bool active = false;
	std::string name;

	GameObject* parent = nullptr;

public:

	std::list<GameObject*> childs;
	std::list<Component*> components;
	bool to_destroy = false;

};

#endif // __GAMEOBJECT_H__