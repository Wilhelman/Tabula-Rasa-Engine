#include "trApp.h"

#include "trResources.h"
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


	return true;
}

bool trResources::CleanUp()
{
	TR_LOG("Cleaning Resources");

	RELEASE(mesh_importer);
	RELEASE(material_importer);

	return true;
}

UID trResources::ImportFile(const char * new_file_in_assets, bool force)
{
	UID ret = 0;
	bool import_ok = false;

	// Find out the type from the extension and send to the correct exporter
	std::string extension;
	App->file_system->GetExtensionFromFile(new_file_in_assets, extension);

	Resource::Type type = TypeFromExtension(extension.c_str());

	switch (type) {
	case Resource::MESH:
		//import_ok = App->tex->Import(new_file_in_assets, "", written_file); 
		break;
	case Resource::TEXTURE: 
		//import_ok = App->scene->Import(new_file_in_assets, written_file)
		break;
	case Resource::SCENE:
		//import_ok = App->scene->Import(new_file_in_assets, written_file)
		break;
	}

	/*if (import_ok == true) { // If export was successful, create a new resource
		Resource* res = CreateNewResource(type);
		res->file = new_file_in_assets;
		res->exported_file = written_file;
		ret = res->uid;
	}*/
	return ret;

}

Resource::Type trResources::TypeFromExtension(const char * extension) const
{
	Resource::Type ret = Resource::UNKNOWN;

	if (extension != nullptr)
	{
		if (_stricmp(extension, "dds") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, "png") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, "jpg") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, "tga") == 0)
			ret = Resource::TEXTURE;
		else if (_stricmp(extension, "fbx") == 0)
			ret = Resource::SCENE;
	}

	return ret;
}
