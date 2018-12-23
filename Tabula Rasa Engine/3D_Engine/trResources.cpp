#include "trApp.h"

#include "trResources.h"
#include "ResourceScene.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"
#include "ResourceBone.h"
#include "ResourceAnimation.h"

#include "trFileSystem.h"

#include "SceneImporter.h"
#include "MaterialImporter.h"
#include "BoneImporter.h"
#include "AnimationImporter.h"
#include "trFileLoader.h"
#include "trAnimation.h"

trResources::trResources()
{
	this->name = "ResourceManager";
}

trResources::~trResources()
{
	
}

bool trResources::Awake(JSON_Object * config)
{
	mesh_importer = new SceneImporter();
	material_importer = new MaterialImporter();
	bone_importer = new BoneImporter();
	animation_importer = new AnimationImporter();

	return true;
}

bool trResources::Start()
{
	// Create basic folders
	App->file_system->MakeNewDir(ASSETS_DIR);
	App->file_system->MakeNewDir(A_MODELS_DIR);
	App->file_system->MakeNewDir(A_SCENES_DIR);
	App->file_system->MakeNewDir(A_TEXTURES_DIR);

	App->file_system->MakeNewDir(SETTINGS_DIR);

	App->file_system->MakeNewDir(LIBRARY_DIR);
	App->file_system->MakeNewDir(L_MESHES_DIR);
	App->file_system->MakeNewDir(L_MATERIALS_DIR);
	App->file_system->MakeNewDir(L_BONES_DIR);
	App->file_system->MakeNewDir(L_ANIMATIONS_DIR);


	CheckForChangesInAssets(App->file_system->GetAssetsDirectory());

	//Assignment 3
	App->animation->CleanAnimableGOS();
	
	App->file_loader->ImportScene("Street environment_V01.trScene", false);
	App->file_loader->ImportScene("Orc_Idle.trScene", false);

	return true;
}

bool trResources::CleanUp()
{
	//TODO warning
	TR_LOG("Cleaning Resources");

	RELEASE(mesh_importer);
	RELEASE(material_importer);
	RELEASE(bone_importer);
	RELEASE(animation_importer);

	for (std::map<UID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		RELEASE(it->second);

	resources.clear();

	return true;
}

bool trResources::PostUpdate(float dt)
{
	//CheckForChangesInAssets(App->file_system->GetAssetsDirectory());
	return true;
}

UID trResources::Find(const char * file_in_assets) const
{
	for (std::map<UID, Resource*>::const_iterator it = resources.begin(); it != resources.end(); ++it)
	{
		std::string file_name = it->second->GetFileName();
		if (file_name.compare(file_in_assets) == 0)
			return it->first;
	}
	return 0;
}

void trResources::Delete(Resource * res_to_delete)
{
	std::map<UID, Resource*>::iterator it = resources.find(res_to_delete->GetUID());
	if (it != resources.end()) {
		RELEASE(it->second);
		resources.erase(it);
	}
		
}

void trResources::CheckForChangesInAssets(Directory* current_dir)
{
	for (uint i = 0u; i < current_dir->files_vec.size(); i++) {
		std::string extension;
		App->file_system->GetExtensionFromFile(current_dir->files_vec[i].name.c_str(), extension);
		Resource::Type type = TypeFromExtension(extension.c_str());
		if(type != Resource::Type::UNKNOWN)
			TryToImportFile(&current_dir->files_vec[i]);
	}

	for (uint i = 0u; i < current_dir->dirs_vec.size(); i++)
		CheckForChangesInAssets(&current_dir->dirs_vec[i]);

	int a = 0;
}

UID trResources::TryToImportFile(File* file) {

	UID ret = 0u;
	std::string file_with_meta = file->path; file_with_meta.append(file->name.c_str()); file_with_meta.append(".meta");

	if (!App->file_system->DoesFileExist(file_with_meta.c_str())) {
		ret = ImportFile(file);
	}
	else { /// If have the resource have a meta file, try to generate the resource
		char* buffer = nullptr;
		uint size = App->file_system->ReadFromFile(file_with_meta.c_str(), &buffer);

		if (buffer != nullptr && size > 0)
		{
			ret = GenerateResourceFromFile(buffer, file);
			RELEASE_ARRAY(buffer);
		}
		else {
			ret = ImportFile(file);
		}
	}

	return ret;

}

UID trResources::ImportFile(File* file, UID forced_uid)
{
	UID ret = 0;
	bool import_ok = false;

	// Find out the type from the extension and send to the correct exporter
	std::string extension;
	App->file_system->GetExtensionFromFile(file->name.c_str(), extension);

	Resource::Type type = TypeFromExtension(extension.c_str());

	std::string exported_path;

	switch (type) {
	case Resource::TEXTURE:
		import_ok = material_importer->Import(file->path.c_str(),file->name.c_str(), exported_path, forced_uid);
		break;
	case Resource::SCENE:
		import_ok = mesh_importer->Import(file->name.c_str(), exported_path);
		break;
	}

	if (import_ok == true) { // If export was successful, create a new resource

		Resource* res = (forced_uid != 0u) ? CreateNewResource(type, forced_uid): CreateNewResource(type);
		res->SetFileName(file->name.c_str());
		std::string imported_path = file->path;imported_path.append(file->name.c_str());
		res->SetImportedPath(imported_path.c_str());
		res->SetExportedPath(exported_path.c_str());

		ret = res->GetUID();

		// Create .meta file
		CreateMetaFileFrom(res, file);
	}

	return ret;
}

void trResources::CreateMetaFileFrom(Resource * resource, File* file)
{
	JSON_Value* root_value = nullptr;
	JSON_Object* root_obj = nullptr;
	JSON_Array* array = nullptr;

	root_value = json_value_init_object();

	// Scene stuff
	root_obj = json_value_get_object(root_value);

	// if file is not a scene, dont do array, just save one uuid
	json_object_set_number(root_obj, "UUID", resource->GetUID());
	json_object_set_string(root_obj, "UUID_path", resource->GetExportedFile());

	std::string file_path = file->path; file_path.append(file->name.c_str());

	json_object_set_number(root_obj, "LastUpdate", file->last_modified);
	// TODO continue this
	/*// ARRAY JSON


	JSON_Value* go_value = json_value_init_array();
	array = json_value_get_array(go_value);
	json_object_set_value(root_obj, "GameObjects", go_value);

	/// Iterating between all gos
	for (std::list<GameObject*>::const_iterator it = root->childs.begin(); it != root->childs.end(); it++) {
	if (!(*it)->to_destroy)
	(*it)->Save(array);
	}
	
	*/

	char *serialized_string = NULL;

	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);

	std::string final_path = resource->GetImportedFile();
	final_path.append(".meta");

	json_serialize_to_file(root_value, final_path.c_str());

	json_free_serialized_string(serialized_string);
	json_value_free(root_value);
}

UID trResources::GenerateResourceFromFile(const char * buffer, File* file)
{
	UID ret = 0u;

	JSON_Value* vroot = json_parse_string(buffer);
	JSON_Object* root = json_value_get_object(vroot);

	JSON_Value* value = json_object_get_value(root, "LastUpdate");
	int last_file_update = json_value_get_number(value);

	value = json_object_get_value(root, "UUID");
	UID resource_uid = json_value_get_number(value);

	if (last_file_update == file->last_modified) { /// If the imported resource is not modified

		std::string extension;
		App->file_system->GetExtensionFromFile(file->name.c_str(), extension);

		value = json_object_get_value(root, "UUID_path");
		std::string resource_path = json_value_get_string(value);

		if (App->file_system->DoesFileExist(resource_path.c_str())) { /// If the saved resource exist 

			std::string extension;
			App->file_system->GetExtensionFromFile(file->name.c_str(), extension);
			Resource::Type type = TypeFromExtension(extension.c_str());

			std::string imported_path = file->path; imported_path.append(file->name.c_str());

			Resource* res = CreateNewResource(type, resource_uid, file->name.c_str(), imported_path.c_str(), resource_path.c_str());
			if(res)
				ret = res->GetUID();
			else {
				Resource* generated_res = Get(resource_uid);
				ret = generated_res->GetUID();
			}
		}
		else {	/// Import the resource forcing with the uuid of the meta file (and the options)
			ret = ImportFile(file, resource_uid);
		}

	}
	else {
		ret = ImportFile(file, resource_uid);
	}

	//end for

	return ret;
}

Resource::Type trResources::TypeFromExtension(const char * extension) const
{
	Resource::Type ret = Resource::UNKNOWN;

	if (extension != nullptr)
	{
		if (_stricmp(extension, ".dds") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, ".png") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, ".jpg") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, ".tga") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, ".fbx") == 0)
			ret = Resource::SCENE;
		else if (_stricmp(extension, ".FBX") == 0)
			ret = Resource::SCENE;
		else if (_stricmp(extension, ".dae") == 0)
			ret = Resource::SCENE;
		else if (_stricmp(extension, ".DAE") == 0)
			ret = Resource::SCENE;
	}

	return ret;
}

Resource * trResources::Get(UID uid)
{
	std::map<UID, Resource*>::iterator it = resources.find(uid);
	if (it != resources.end())
		return it->second;
	return nullptr;
}

Resource * trResources::CreateNewResource(Resource::Type type, UID uid_to_force, const char* file_name, 
										  const char* imported_path, const char* exported_path)
{
	Resource* ret = nullptr;
	uint res_uid = 0u;
	
	if (uid_to_force != 0u) 
	{
		res_uid = uid_to_force;

		if (Get(res_uid) != nullptr) //the resource is already done
			return Get(res_uid);
	}
	else
		res_uid = App->GenerateNewUUID();

	switch (type)
	{
	case Resource::TEXTURE:
		ret = (Resource*) new ResourceTexture(res_uid);
		break;
	case Resource::MESH:
		ret = (Resource*) new ResourceMesh(res_uid);
		break;
	case Resource::SCENE:
		ret = (Resource*) new ResourceScene(res_uid);
		break;
	case Resource::BONE:
		ret = (Resource*) new ResourceBone(res_uid);
		break;
	case Resource::ANIMATION:
		ret = (Resource*) new ResourceAnimation(res_uid);
	}

	if (ret != nullptr)
	{
		if (file_name) ret->SetFileName(file_name);
		if (imported_path) ret->SetImportedPath(imported_path);
		if (exported_path) ret->SetExportedPath(exported_path);
		resources[res_uid] = ret;
	}

	return ret;
}