#include "GameObject.h"

// ---------------------------------------------------------
GameObject::GameObject()
{}

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

bool GameObject::IsActive() const
{
	return this->active;
}



// ---------------------------------------------------------

