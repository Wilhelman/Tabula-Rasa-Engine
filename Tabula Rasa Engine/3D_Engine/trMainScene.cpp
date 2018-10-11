#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trWindow.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "trPrimitives.h"

#include "PGrid.h"

trMainScene::trMainScene() : trModule()
{
	name = "main_scene";
}

// Destructor
trMainScene::~trMainScene()
{}

// Called before render is available
bool trMainScene::Awake(JSON_Object* config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{

	App->camera->Move(vec3(1.f, 1.f, 1.f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	grid = new PGrid();
	grid->axis = true;

	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate(float dt)
{

	return true;
}

bool trMainScene::Update(float dt)
{
		

	return true;
}

bool trMainScene::PostUpdate(float dt)
{

	return true;
}

// Called before quitting
bool trMainScene::CleanUp()
{
	delete grid;//todo check
	return true;
}

void trMainScene::Draw()
{
	grid->Render();
}

// Load Game State
bool trMainScene::Load(JSON_Object* config)
{
	return true;
}

// Save Game State
bool trMainScene::Save(JSON_Object* config) const
{
	//this have to iterate
	/*JSON_Object *root_object = json_value_get_object(&data);
	json_object_set_string(root_object, "name", "John Smith");
	json_object_set_number(root_object, "age", 25);
	json_object_dotset_string(root_object, "address.city", "Cupertino");
	json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));*/

	return true;
}