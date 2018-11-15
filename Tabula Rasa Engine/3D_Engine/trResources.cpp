#include "trApp.h"

#include "trResources.h"
#include "ResourceScene.h"

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
		TryToImportFile(current_dir->files_vec[i].c_str());

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
	switch (type) {
	case Resource::MESH:
		import_ok = material_importer->Import(file_name, exported_path);
		break;
	case Resource::TEXTURE:
		import_ok = material_importer->Import(file_name, exported_path);
		break;
	case Resource::SCENE:
		import_ok = mesh_importer->Import(file_name, exported_path);
		if (import_ok) {
			imported_path = A_MODELS_DIR;
			imported_path.append("/");
			imported_path.append(file_name);
		}
		break;
	}

	if (import_ok == true) { // If export was successful, create a new resource
		Resource* res = CreateNewResource(type);
		res->SetFileName(file_name);
		res->SetImportedPath(imported_path.c_str());
		res->SetExportedPath(exported_path.c_str());
		ret = res->GetUID();
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

Resource * trResources::CreateNewResource(Resource::Type type)
{
	Resource* ret = nullptr;
	uint res_uid = 0u;
	// force?
	res_uid = App->GenerateNewUUID();

	switch (type)
	{
	case Resource::TEXTURE:
		//ret = (Resource*) new ResourceTexture();
		break;
	case Resource::MESH:
		//ret = (Resource*) new ResourceMesh();
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
