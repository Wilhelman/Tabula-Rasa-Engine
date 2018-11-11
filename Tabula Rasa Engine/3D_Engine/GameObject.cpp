#include "GameObject.h"
#include <list>

#include "trApp.h"

#include "ComponentTransform.h"
#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"

#include "trMainScene.h"

#include "Event.h"

#include <algorithm>
#include "pcg/pcg_variants.h"

// ---------------------------------------------------------
GameObject::GameObject()
{}

GameObject::GameObject(const char * name, GameObject * parent)
{
	this->uuid = pcg32_random_r(&App->gen_uuid);
	this->name = name;
	this->parent = parent;

	this->is_static = true;

	if (parent != nullptr) {// if is not root
		CreateComponent(Component::component_type::COMPONENT_TRANSFORM);
		parent->childs.push_back(this);
	}
	else
		CreateComponent(Component::component_type::COMPONENT_TRANSFORM);

	bounding_box = AABB(float3(-1.f, -1.f, -1.f), float3(1.f, 1.f, 1.f));

}
// ---------------------------------------------------------
GameObject::~GameObject()
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		RELEASE(*it);

	for (std::list<GameObject*>::iterator it = childs.begin(); it != childs.end(); it++)
		RELEASE(*it);
}

bool GameObject::PreUpdate(float dt)
{
	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++)
		(*it)->PreUpdate(dt);
	return true;
}

// ---------------------------------------------------------
bool GameObject::Update(float dt)
{
	return true;
}

bool GameObject::Save(JSON_Array* array)const
{
	JSON_Value* root_value = nullptr;
	JSON_Object* root_obj = nullptr;
	root_value = json_value_init_object();
	root_obj = json_value_get_object(root_value);

	// TODO check duplicate error

	// GO info
	json_object_set_number(root_obj, "UUID", uuid);
	json_object_set_number(root_obj, "ParentUUID", (parent) ? parent->GetUUID() : 0);
	json_object_set_string(root_obj, "Name", name.c_str());

	// COMPONENTS info
	JSON_Value* array_value = json_value_init_array();
	JSON_Array * components_array = json_value_get_array(array_value);

	json_object_set_value(root_obj, "Components", array_value);

	for (std::list<Component*>::const_iterator it = components.begin(); it != components.end(); ++it)
	{
		JSON_Value* component_value = nullptr;
		JSON_Object* component_obj = nullptr;
		component_value = json_value_init_object();
		component_obj = json_value_get_object(component_value);

		json_object_set_number(component_obj, "Type", (*it)->GetType()); // TODO: Would be cool if instead of number we write the string ...

		(*it)->Save(components_array, component_obj);

		json_array_append_value(components_array, json_value_deep_copy(component_value));
	}

	json_array_append_value(array, json_value_deep_copy(root_value));

	// Iterate for all children
	for (std::list<GameObject*>::const_iterator it = childs.begin(); it != childs.end(); ++it)
		(*it)->Save(array);

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
	case Component::component_type::COMPONENT_CAMERA:
		tmp_component = new ComponentCamera(this);
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

Component * GameObject::FindComponentByType(Component::component_type type)
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

int GameObject::GetUUID() const
{
	return uuid;
}

void GameObject::RecalculateBoundingBox()
{
	
	ComponentMesh* mesh_co = (ComponentMesh*)FindComponentByType(Component::component_type::COMPONENT_MESH);
	
	if (mesh_co != nullptr) {
		bounding_box.SetNegativeInfinity();

		const Mesh* mesh = mesh_co->GetMesh();
		bounding_box.Enclose((float3*)mesh->vertices, mesh->vertex_size / 3);
	
		OBB obb(bounding_box);
		obb.Transform(transform->GetMatrix());

		bounding_box = obb.MinimalEnclosingAABB();
	}
	else { // GO without mesh
		bounding_box.SetNegativeInfinity();
		bounding_box = AABB(float3(-1.f, -1.f, -1.f), float3(1.f, 1.f, 1.f));
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

