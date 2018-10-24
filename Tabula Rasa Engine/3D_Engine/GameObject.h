#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "trDefs.h"

#include "Component.h"

#include <string>
#include <vector>


class GameObject
{
public:
	GameObject();
	~GameObject();

	bool Update(float dt);

	Component* CreateComponent(Component::component_type type);

	GameObject* GetParent() const;

	bool IsActive()const;

private:

	bool active = false;
	std::string name;

	GameObject* parent = nullptr;

public:

	std::vector<GameObject*> children;
	std::vector<Component*> components;

};

#endif // __GAMEOBJECT_H__