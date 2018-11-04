#include "GameObject.h"
#include <list>

#include "trApp.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"

#include "trMainScene.h"

#include "Event.h"

// ---------------------------------------------------------
GameObject::GameObject()
{}

GameObject::GameObject(const char * name, GameObject * parent)
{
	this->name = name;
	this->parent = parent;

	this->is_static = true;

	if(parent != nullptr)// if is not root
		parent->childs.push_back(this);
	else
		CreateComponent(Component::component_type::COMPONENT_TRANSFORM);

}
// ---------------------------------------------------------
GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		RELEASE(*it);

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
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
	Component* tmp_component = nullptr;
	switch (type)
	{
	case Component::component_type::COMPONENT_TRANSFORM:
		tmp_component = new ComponentTransform(this);
		transform = (ComponentTransform*)tmp_component;
		break;
	case Component::component_type::COMPONENT_MESH:
		tmp_component = new ComponentMesh(this);
		break;
	case Component::component_type::COMPONENT_MATERIAL:
		tmp_component = new ComponentMaterial(this);
		break;
	case Component::component_type::COMPONENT_UNKNOWN:
		TR_LOG("Just how?");
		break;
	default:
		break;
	}

	this->components.push_back(tmp_component);

	return tmp_component;
}

Component * GameObject::CreateComponent(Component::component_type type,  Component* component)
{
	Component* tmp_component = nullptr;
	switch (type)
	{
	case Component::component_type::COMPONENT_TRANSFORM:
		tmp_component = new ComponentTransform(this);
		transform = (ComponentTransform*)tmp_component;
		break;
	case Component::component_type::COMPONENT_MESH:
		tmp_component = new ComponentMesh(this);
		break;
	case Component::component_type::COMPONENT_MATERIAL: {
		tmp_component = new ComponentMaterial(this);
		Texture* tmp_tex = new Texture(); // todo pass it to the constr.
		const Texture* tmp_ret_tex = ((ComponentMaterial*)component)->GetTexture();
		if (tmp_ret_tex != nullptr)
		{
			tmp_tex->height = ((ComponentMaterial*)component)->GetTexture()->height; // TODO: if tex == nullptr this crashes
			tmp_tex->width = ((ComponentMaterial*)component)->GetTexture()->width;   // maybe set a default color texture when this happens?
			tmp_tex->id = ((ComponentMaterial*)component)->GetTexture()->id;
			tmp_tex->path = ((ComponentMaterial*)component)->GetTexture()->path;
			((ComponentMaterial*)tmp_component)->SetTexture(tmp_tex);
		}
	
		break;
	}
	case Component::component_type::COMPONENT_UNKNOWN:
		TR_LOG("Just how?");
		break;
	default:
		break;
	}

	this->components.push_back(tmp_component);

	return tmp_component;
}

Component * GameObject::FindComponentWithType(Component::component_type type)
{
	for (std::list<Component*>::iterator co_it = components.begin(); co_it != components.end(); co_it++) {
		if ((*co_it)->GetType() == type)
			return (*co_it);
	}
	return nullptr;
}

GameObject * GameObject::GetParent() const
{
	return this->parent;
}

void GameObject::SetParent(GameObject * new_parent)
{
	if (this->parent == new_parent) {
		return;
	}

	if (parent != nullptr)
		parent->childs.remove(this);

	parent = new_parent;

	if (new_parent)
		new_parent->childs.push_back(this);

	this->GetTransform()->GetMatrix();
	this->RecalculateBoundingBox();
}

const char * GameObject::GetName() const
{
	return this->name.c_str();
}

void GameObject::SetName(const char * name)
{
	this->name = name;
}

ComponentTransform * GameObject::GetTransform() const
{
	return transform;
}

void GameObject::RecalculateBoundingBox()
{
	bounding_box.SetNegativeInfinity();

	ComponentMesh* mesh_co = (ComponentMesh*)FindComponentWithType(Component::component_type::COMPONENT_MESH);
	
	if (mesh_co != nullptr) {
		const Mesh* mesh = mesh_co->GetMesh();
		bounding_box.Enclose((float3*)mesh->vertices, mesh->vertex_size);
	
		OBB obb(bounding_box);
		obb.Transform(transform->GetMatrix());

		bounding_box = obb.MinimalEnclosingAABB();
	}

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		(*it)->RecalculateBoundingBox();
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
			// TODO notify to all modules
			Event tmp_event(Event::event_type::GAMEOBJECT_DESTROYED);
			App->SendTheCarrierPigeon(tmp_event);
		}
		else // Keep iterating childs
		{
			(*it)->DestroyGameObjectsIfNeeded();
			it++;
		}
	}
}



// ---------------------------------------------------------

