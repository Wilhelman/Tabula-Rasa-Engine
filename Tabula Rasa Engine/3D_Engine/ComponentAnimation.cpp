#include "ComponentAnimation.h"

#include "ComponentMesh.h"
#include "trApp.h"
#include "trResources.h"
#include "trRenderer3D.h"
#include "trFileLoader.h"
#include "SceneImporter.h"
#include "GameObject.h"
#include "Resource.h"
#include "ResourceMesh.h"
#include "ResourceAnimation.h"
#include "AnimationImporter.h"
#include "trAnimation.h"

ComponentAnimation::ComponentAnimation(GameObject * embedded_game_object) :
	Component(embedded_game_object, Component::component_type::COMPONENT_ANIMATION)
{
}

ComponentAnimation::ComponentAnimation(GameObject * embedded_game_object, UID resource) :
	Component(embedded_game_object, Component::component_type::COMPONENT_ANIMATION)
{
	this->resource = resource;
}

ComponentAnimation::~ComponentAnimation()
{
	Resource* res = (Resource*)GetResource();
	if (res)
		res->Release();
}

bool ComponentAnimation::Save(JSON_Object* component_obj) const
{
	//todo: get resource path etc
	const Resource* res = this->GetResource();
	json_object_set_string(component_obj, "path", res->GetExportedFile());
	return true;
}

bool ComponentAnimation::Load(const JSON_Object * component_obj)
{
	bool ret = true;

	JSON_Value* value = json_object_get_value(component_obj, "path");
	const char* file_path = json_value_get_string(value);

	//todo clean
	if (file_path) {
		std::string uid_force = file_path;
		const size_t last_slash = uid_force.find_last_of("\\/");
		if (std::string::npos != last_slash)
			uid_force.erase(0, last_slash + 1);
		const size_t extension = uid_force.rfind('.');
		if (std::string::npos != extension)
			uid_force.erase(extension);
		UID uid = static_cast<unsigned int>(std::stoul(uid_force));

		SetResource(App->resources->animation_importer->GenerateResourceFromFile(file_path, uid)); // TODO CHECK GENERATE RESOURCE
	}

	return ret;
}

bool ComponentAnimation::SetResource(UID resource)
{
	if (Resource* res = (Resource*)GetResource()) {
		res->Release();
	}

	this->resource = resource;
	ResourceAnimation* bone_res = (ResourceAnimation*)this->GetResource();

	if (bone_res)
		uint num_references = bone_res->LoadToMemory();

	if(bone_res)
		App->animation->SetAnimationGos(bone_res);

	return true;
}
