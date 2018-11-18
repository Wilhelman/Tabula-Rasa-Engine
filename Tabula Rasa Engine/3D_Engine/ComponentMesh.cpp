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
	if(res)
		res->Release();
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

	JSON_Value* value = json_object_get_value(component_obj, "path");
	const char* file_path = json_value_get_string(value);

	//todo clean
	std::string uid_force = file_path;
	const size_t last_slash = uid_force.find_last_of("\\/");
	if (std::string::npos != last_slash)
		uid_force.erase(0, last_slash + 1);
	const size_t extension = uid_force.rfind('.');
	if (std::string::npos != extension)
		uid_force.erase(extension);
	UID uid = static_cast<unsigned int>(std::stoul(uid_force));

	SetResource(App->resources->mesh_importer->GenerateResourceFromFile(file_path,uid));

	return ret;
}

bool ComponentMesh::SetResource(UID resource)
{
	if (Resource* res = (Resource*)GetResource()) {
		res->Release();
	}

	this->resource = resource;
	ResourceMesh* mesh_res = (ResourceMesh*)this->GetResource();
	
	if(mesh_res)
		uint num_references = mesh_res->LoadToMemory();

	embedded_go->bounding_box = AABB(float3(0.f, 0.f, 0.f), float3(0.f, 0.f, 0.f));
	embedded_go->bounding_box.Enclose((float3*)mesh_res->vertices, mesh_res->vertex_size / 3);

	return true;
}
