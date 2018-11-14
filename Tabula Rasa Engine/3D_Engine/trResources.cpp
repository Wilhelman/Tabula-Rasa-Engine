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
	// where copy paste the file?

	CheckForChangesInAssets(App->file_system->assets_dir); // todo: this have to be done each x time

	return true;
}

bool trResources::CleanUp()
{
	TR_LOG("Cleaning Resources");

	RELEASE(mesh_importer);
	RELEASE(material_importer);

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

UID trResources::ImportFile(const char * file_path)
{
	UID ret = 0;
	bool import_ok = false;

	// Find out the type from the extension and send to the correct exporter
	std::string extension;
	App->file_system->GetExtensionFromFile(file_path, extension);

	Resource::Type type = TypeFromExtension(extension.c_str());

	std::string written_file;
	switch (type) {
	case Resource::MESH:
		import_ok = material_importer->Import(file_path, written_file);
		break;
	case Resource::TEXTURE:
		import_ok = material_importer->Import(file_path, written_file);
		break;
	case Resource::SCENE:
		import_ok = mesh_importer->Import(file_path, written_file);
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
