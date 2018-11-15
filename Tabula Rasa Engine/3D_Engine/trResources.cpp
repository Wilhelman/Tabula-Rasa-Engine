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

	CheckForChangesInAssets(App->file_system->assets_dir); // todo: this have to be done each x time

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
		TryToImportFile(current_dir->files_vec[i].name.c_str());

	for (uint i = 0u; i < current_dir->dirs_vec.size(); i++)
		CheckForChangesInAssets(&current_dir->dirs_vec[i]);
}

void trResources::TryToImportFile(const char* file) {

	std::string file_with_meta = file;
	file_with_meta.append(".meta");

	if (!App->file_system->DoesFileExist(file_with_meta.c_str())) {
		ImportFile(file);
	}
	else {
		if (MetaFileIsCorrect(file_with_meta.c_str())) {

		}
	}
	// Check if the file have .meta
	// if NOT
	// ImportFile() so we generate both meta and resource
	// else (if have .meta)
	// prepare the importer of the file readin the .meta info

}

UID trResources::ImportFile(const char * file_name)
{
	UID ret = 0;
	bool import_ok = false;

	// Find out the type from the extension and send to the correct exporter
	std::string extension;
	App->file_system->GetExtensionFromFile(file_name, extension);

	Resource::Type type = TypeFromExtension(extension.c_str());

	std::string imported_path;
	std::string exported_path;
	UID uid_to_force = 0u;

	//	----------------------------	TODO SOLVE ALL PATHS STUFF!---------------------
	switch (type) {
	case Resource::TEXTURE:
		import_ok = material_importer->Import(file_name, exported_path, uid_to_force);
		if (import_ok) { // <- todo this sucks
			imported_path = A_TEXTURES_DIR;
			imported_path.append("/");
			imported_path.append(file_name);
		}
		break;
	case Resource::SCENE:
		import_ok = mesh_importer->Import(file_name, exported_path);
		if (import_ok) { // <- todo this sucks
			imported_path = A_MODELS_DIR;
			imported_path.append("/");
			imported_path.append(file_name);
		}
		break;
	}

	if (import_ok == true) { // If export was successful, create a new resource

		Resource* res = (uid_to_force != 0u) ? CreateNewResource(type, uid_to_force): CreateNewResource(type);

		res->SetFileName(file_name);
		res->SetImportedPath(imported_path.c_str());
		res->SetExportedPath(exported_path.c_str());
		ret = res->GetUID();

		// Create .meta file
		CreateMetaFileFrom(res, file_name);
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

bool trResources::MetaFileIsCorrect(const char * meta_file)
{
	return false;
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

Resource * trResources::CreateNewResource(Resource::Type type, UID uid_to_force)
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
		resources[res_uid] = ret;
	}

	return ret;
}
