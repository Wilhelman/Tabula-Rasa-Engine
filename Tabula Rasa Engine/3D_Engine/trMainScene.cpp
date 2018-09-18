#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"

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
	LOG("Wake me up. Wake me up inside!");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	LOG("Main Scene start");

	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate()
{
	return true;
}

bool trMainScene::Update(float dt)
{

	return true;
}

bool trMainScene::PostUpdate()
{
	return true;
}

// Called before quitting
bool trMainScene::CleanUp()
{
	LOG("Cleaning up trMainScene");
	return true;
}

// Load Game State
bool trMainScene::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool trMainScene::Save(pugi::xml_node& data) const
{

	return true;
}