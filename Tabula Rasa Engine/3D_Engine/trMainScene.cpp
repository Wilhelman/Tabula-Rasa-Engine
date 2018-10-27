#include "trDefs.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trFileLoader.h"
#include "PGrid.h"

#include "GameObject.h"

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

	default_mesh = new std::string(json_object_get_string(config, "default_mesh"));

	root = new GameObject("root", nullptr);

	GameObject* test = this->CreateGameObject("pablo", nullptr);
	this->CreateGameObject(test);
	this->CreateGameObject(nullptr);

	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	App->camera->Move(vec3(1.f, 1.f, 1.f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));

	grid = new PGrid();
	grid->axis = true;

	App->file_loader->Import3DFile(default_mesh->c_str());
	
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
	//delete default_mesh.data();
	if(default_mesh != nullptr)
		delete default_mesh;

	if(grid != nullptr)
		delete grid; //todo check

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
	return true;
}

GameObject * trMainScene::GetRoot() const
{
	return root;
}

GameObject * trMainScene::CreateGameObject(GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject("unnamed", parent);
}

GameObject * trMainScene::CreateGameObject(const char * name, GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject(name, parent);
}
