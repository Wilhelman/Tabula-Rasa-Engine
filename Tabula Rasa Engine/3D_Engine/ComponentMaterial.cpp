#include "ComponentMaterial.h"
#include "trApp.h"
#include "MaterialImporter.h"
#include "trFileSystem.h"
#include "trResources.h"
#include "Resource.h"
#include "ResourceTexture.h"

ComponentMaterial::ComponentMaterial(GameObject * embedded_game_object) :
Component(embedded_game_object, Component::component_type::COMPONENT_MATERIAL)
{
}

ComponentMaterial::~ComponentMaterial()
{
	Resource* res = (Resource*)GetResource();
	if (res)
		res->Release();
}

bool ComponentMaterial::Save(JSON_Object* component_obj) const
{
	const Resource* res = this->GetResource();
	json_object_set_string(component_obj, "path", res->GetExportedFile());
	return true;
}

bool ComponentMaterial::Load(const JSON_Object * component_obj)
{
	bool ret = true;

	// todo get resource path and set.

	JSON_Value* value = json_object_get_value(component_obj, "path");
	const char* file_path = json_value_get_string(value);

	SetResource(App->resources->material_importer->LoadImageFromPath(file_path));

	return ret;
}

void ComponentMaterial::SetAmbientColor(float4 ambient_color)
{
	this->ambient_color = ambient_color;
}

const float4 ComponentMaterial::GetAmbientColor() const
{
	return ambient_color;
}

bool ComponentMaterial::SetResource(UID resource)
{
	if (Resource* res = (Resource*)GetResource()) {
		res->Release();
	}

	this->resource = resource;
	ResourceTexture* res = (ResourceTexture*)this->GetResource();

	uint num_references = res->LoadToMemory();

	return true;
}
