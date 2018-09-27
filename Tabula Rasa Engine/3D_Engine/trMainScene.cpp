#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trWindow.h"
#include "trRenderer3D.h"
#include "trInput.h"


#define VSYNC true

trMainScene::trMainScene() : trModule()
{
	name = "main_scene";
}

// Destructor
trMainScene::~trMainScene()
{}

// Called before render is available
bool trMainScene::Awake(pugi::xml_node& config)
{
	bool ret = true;

	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
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
	return true;
}

// Load Game State
bool trMainScene::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool trMainScene::Save(JSON_Value& data) const
{
	//this have to iterate
	JSON_Object *root_object = json_value_get_object(&data);
	json_object_set_string(root_object, "name", "John Smith");
	json_object_set_number(root_object, "age", 25);
	json_object_dotset_string(root_object, "address.city", "Cupertino");
	json_object_dotset_value(root_object, "contact.emails", json_parse_string("[\"email@example.com\",\"email2@example.com\"]"));

	return true;
}