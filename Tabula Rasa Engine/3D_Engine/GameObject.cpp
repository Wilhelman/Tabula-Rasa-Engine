#include "GameObject.h"
#include <list>

// ---------------------------------------------------------
GameObject::GameObject()
{}

GameObject::GameObject(const char * name, GameObject * parent)
{
	this->name = name;
	this->parent = parent;

	if(parent != nullptr)// if is not root
		parent->childs.push_back(this);
}
// ---------------------------------------------------------
GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); ++it)
		RELEASE(*it);

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); ++it)
		RELEASE(*it);
}

// ---------------------------------------------------------
bool GameObject::Update(float dt)
{
	return true;
}

// ---------------------------------------------------------
Component * GameObject::CreateComponent(Component::component_type type)
{
	return nullptr;
}

GameObject * GameObject::GetParent() const
{
	return this->parent;
}

const char * GameObject::GetName() const
{
	return this->name.c_str();
}

bool GameObject::IsActive() const
{
	return this->active;
}

void GameObject::DestroyGameObjectsIfNeeded()
{
	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end();)
	{
		if ((*it)->to_destroy)
		{
			RELEASE(*it);
			it = childs.erase(it);
		}
		else // Keep iterating childs
		{
			(*it)->DestroyGameObjectsIfNeeded();
			it++;
		}
	}
}



// ---------------------------------------------------------

