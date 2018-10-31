#include "Component.h"

// ---------------------------------------------------------
Component::Component(GameObject * embedded_go, component_type type)
{
	this->embedded_go = embedded_go;
	this->type = type;
}

// ---------------------------------------------------------

bool Component::Enable()
{
	return false;
}

bool Component::Update(float dt)
{
	return false;
}

bool Component::Disable()
{
	return false;
}

GameObject * Component::GetEmbeddedObject() const
{
	return nullptr;
}

bool Component::IsActive() const
{
	return false;
}

const Component::component_type Component::GetType() const
{
	return type;
}

