#include "ComponentMesh.h"

#include "trApp.h"
#include "trResources.h"
#include "trRenderer3D.h"
#include "trFileLoader.h"
#include "MeshImporter.h"
#include "GameObject.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "trOpenGL.h"

ComponentMesh::ComponentMesh(GameObject * embedded_game_object) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
}

ComponentMesh::ComponentMesh(GameObject * embedded_game_object, UID resource) : 
	Component(embedded_game_object, Component::component_type::COMPONENT_MESH)
{
	this->resource = resource;
}

ComponentMesh::~ComponentMesh()
{
	Resource* res = (Resource*)GetResource();
	res->UnReference();
}

bool ComponentMesh::Save(JSON_Object* component_obj) const
{
	//todo: get resource path etc
	const Resource* res = this->GetResource();
	json_object_set_string(component_obj, "path", res->GetExportedFile());
	return true;
}

bool ComponentMesh::Load(const JSON_Object * component_obj)
{
	bool ret = true;

	// todo get resource path and set.

	JSON_Value* value = json_object_get_value(component_obj, "path");
	const char* file_path = json_value_get_string(value);
	SetResource(App->file_loader->mesh_importer->GenerateResourceFromFile(file_path));

	return ret;
}

bool ComponentMesh::SetResource(UID resource)
{
	if (Resource* res = (Resource*)GetResource()) {
		res->UnReference();
	}

	this->resource = resource;
	ResourceMesh* res = (ResourceMesh*)this->GetResource();
	
	uint num_references = res->LoadToMemory();

	embedded_go->bounding_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
	embedded_go->bounding_box.Enclose((float3*)res->vertices, res->vertex_size / 3);


	
	return true;
}
