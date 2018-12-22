#include "Component.h"
#include "trApp.h"
#include <algorithm>

#include "trResources.h"

// ---------------------------------------------------------
Component::Component(GameObject * embedded_go, component_type type)
{
	this->embedded_go = embedded_go;
	this->type = type;
	this->uuid = App->GenerateNewUUID();
}

// ---------------------------------------------------------

bool Component::Enable()
{
	return false;
}

bool Component::Start()
{
	return true;
}

bool Component::PreUpdate(float dt)
{
	return true;
}

bool Component::Update(float dt)
{
	return true;
}

bool Component::Disable()
{
	return true;
}

bool Component::Save(JSON_Object* component_obj) const
{
	return true;
}

bool Component::Load(const JSON_Object * component_obj)
{
	return true;
}

UID Component::GetUUID() const
{
	return uuid;
}

GameObject * Component::GetEmbeddedObject() const
{
	return embedded_go;
}

bool Component::IsActive() const
{
	return false;
}

const Component::component_type Component::GetType() const
{
	return type;
}

bool Component::SetResource(UID res)
{
	this->resource = res;
	return true;
}

const Resource * Component::GetResource() const
{
	return App->resources->Get(resource);
}

UID Component::GetResourceUID() const
{
	return resource;
}

