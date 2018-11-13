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
