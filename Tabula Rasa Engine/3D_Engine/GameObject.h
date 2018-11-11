#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "trDefs.h"

#include "Component.h"
#include "ComponentTransform.h"

#include "MathGeoLib/MathGeoLib.h"

#include <string>
#include <list>

class GameObject
{
public:
	GameObject();
	GameObject(const char* name, GameObject* parent);
	~GameObject();

	bool PreUpdate(float dt);
	bool Update(float dt);

	Component* CreateComponent(Component::component_type type);
	Component* CreateComponent(Component::component_type type, Component* component);

	// finds and returns the first component of the type sended
	Component* FindComponentByType(Component::component_type type);

	// getters/setters stuff
	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);
	const char* GetName() const;
	void SetName(const char* name);
	ComponentTransform* GetTransform()const;
	int GetUUID()const;

	void RecalculateBoundingBox();

	bool IsActive()const;

	void DestroyGameObjectsIfNeeded();

private:

	bool active = false;
	std::string name;
	GameObject* parent = nullptr;
	ComponentTransform* transform = nullptr; //Always should have one
	int uuid = 0;

public:

	AABB bounding_box;

	std::list<GameObject*> childs;
	std::list<Component*> components;

	bool to_destroy = false;
	bool is_static = false;

	bool in_camera = false;
	bool is_active = true;

};

#endif // __GAMEOBJECT_H__