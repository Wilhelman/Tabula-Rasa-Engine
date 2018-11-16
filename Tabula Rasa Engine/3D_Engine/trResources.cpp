#include "trApp.h"

#include "trResources.h"
#include "ResourceScene.h"
#include "ResourceMesh.h"
#include "ResourceTexture.h"

#include "trFileSystem.h"

#include "MeshImporter.h"
#include "MaterialImporter.h"


trResources::trResources()
{
	this->name = "ResourceManager";
}

trResources::~trResources()
{
}

bool trResources::Awake(JSON_Object * config)
{
	mesh_importer = new MeshImporter();
	material_importer = new MaterialImporter();

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

	// TODO: Check if there is something in assets directory. If so, import it!
	// where copy paste the file?

	CheckForChangesInAssets(App->file_system->GetAssetsDirectory()); // todo: this have to be done each x time

	return true;
}

bool trResources::CleanUp()
{
	TR_LOG("Cleaning Resources");

	RELEASE(mesh_importer);
	RELEASE(material_importer);

	for (std::map<UID, Resource*>::iterator it = resources.begin(); it != resources.end(); ++it)
		RELEASE(it->second);

	resources.clear();

	return true;
}

void trResources::CheckForChangesInAssets(Directory* current_dir)
{
	for (uint i = 0u; i < current_dir->files_vec.size(); i++)
		TryToImportFile(&current_dir->files_vec[i]);

	for (uint i = 0u; i < current_dir->dirs_vec.size(); i++)
		CheckForChangesInAssets(&current_dir->dirs_vec[i]);
}

void trResources::TryToImportFile(File* file) {

	std::string file_with_meta = file->name;
	file_with_meta.append(".meta");

	if (!App->file_system->DoesFileExist(file_with_meta.c_str())) {
		ImportFile(file);
	}
	else { /// If have the resource have a meta file, try to generate the resource
		char* buffer = nullptr;
		uint size = App->file_system->ReadFromFile(file_with_meta.c_str(), &buffer);

		if (buffer != nullptr && size > 0)
		{
			GenerateResourceFromMeta(buffer);
			RELEASE_ARRAY(buffer);
		}
		else {
			ImportFile(file);
		}
	}

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

	//	----------------------------	TODO SOLVE ALL PATHS STUFF!---------------------
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
		std::string imported_path = file->path; imported_path.append("/"); imported_path.append(file->name.c_str());
		res->SetImportedPath(imported_path.c_str());
		res->SetExportedPath(exported_path.c_str());

		ret = res->GetUID();

		// Create .meta file
		CreateMetaFileFrom(res, file->name.c_str());
	}

	return ret;
}

void trResources::CreateMetaFileFrom(Resource * resource, const char * file_name)
{
	// TODO save all gos
	JSON_Value* root_value = nullptr;
	JSON_Object* root_obj = nullptr;
	JSON_Array* array = nullptr;

	root_value = json_value_init_object();

	// Scene stuff
	root_obj = json_value_get_object(root_value);

	// if file is not a scene, dont do array, just save one uuid
	json_object_set_number(root_obj, "UUID", resource->GetUID());

	json_object_set_number(root_obj, "LastUpdate", 0/*TODO GET THE LAST MOD FROM PHYSFS*/);

	/*// ARRAY JSON
	JSON_Value* go_value = json_value_init_array();
	array = json_value_get_array(go_value);
	json_object_set_value(root_obj, "GameObjects", go_value);

	/// Iterating between all gos
	for (std::list<GameObject*>::const_iterator it = root->childs.begin(); it != root->childs.end(); it++) {
	if (!(*it)->to_destroy)
	(*it)->Save(array);
	}*/

	char *serialized_string = NULL;

	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);

	std::string final_path = resource->GetImportedFile();
	final_path.append(".meta");

	json_serialize_to_file(root_value, final_path.c_str());

	json_free_serialized_string(serialized_string);
	json_value_free(root_value);
}

bool trResources::GenerateResourceFromMeta(const char * buffer)
{
	bool ret = false;

	JSON_Value* vroot = json_parse_string(buffer);
	JSON_Object* root = json_value_get_object(vroot);

	JSON_Value* value = json_object_get_value(root, "LastUpdate");
	int last_file_update = json_value_get_number(value); // TODO: this is not an int

	value = json_object_get_value(root, "UUID");
	UID resource_uid = json_value_get_number(value);

	if (last_file_update == 0/*TODO last_mod_from imported file*/) { /// If the imported resource is not modified

		std::string resource_path = std::to_string(resource_uid); //todo set better the path from type of resource

		if (App->file_system->DoesFileExist(resource_path.c_str())) { /// If the saved resource exist

			std::string extension;
			App->file_system->GetExtensionFromFile(buffer, extension); //TODO THIS FILE_NAME IS WRONG
			Resource::Type type = TypeFromExtension(extension.c_str());

			Resource* res = CreateNewResource(type, resource_uid/*TODO ADD HERE ALL ABOVE PATHS*/);
			res->SetFileName(buffer); // TODO THIS FILE_NAME IS WRONG
			res->SetImportedPath(buffer); //TODO THIS FILE_NAME IS WRONG
			res->SetExportedPath(resource_path.c_str());
			ret = true;
		}
		else {	/// Import the resource forcing with the uuid of the meta file (and the options)
			ImportFile(buffer/*WRONG*/, resource_uid);
		}

	}
	else {
		ImportFile(buffer/*WRONG*/);
	}
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

Resource * trResources::CreateNewResource(Resource::Type type, UID uid_to_force,
	const char* file_name, const char* imported_path, const char* exported_path)
{
	Resource* ret = nullptr;
	uint res_uid = 0u;
	
	res_uid = (uid_to_force != 0u) ? uid_to_force : App->GenerateNewUUID();

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
