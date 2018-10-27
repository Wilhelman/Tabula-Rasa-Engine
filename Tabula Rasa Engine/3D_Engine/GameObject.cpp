#include "GameObject.h"

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
{}

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



// ---------------------------------------------------------

