#include "trDefs.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trFileLoader.h"
#include "PGrid.h"
#include "ComponentCamera.h"

#include "GameObject.h"
#include "DebugDraw.h"

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

	main_camera = new GameObject("Main Camera", root);
	main_camera->CreateComponent(Component::component_type::COMPONENT_CAMERA);

	quadtree.Create(AABB(AABB(float3(-500, -100, -500), float3(500, 100, 500))));

	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	App->camera->dummy_camera->LookAt(float3(0.f, 0.f, 0.f));

	grid = new PGrid();
	grid->axis = true;

	//App->file_loader->Import(default_mesh->c_str());

	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate(float dt)
{
	root->DestroyGameObjectsIfNeeded();
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

void trMainScene::DrawDebug()
{
	// Draw quadtree AABBs
	std::vector<AABB> quad_aabbs;
	quadtree.FillWithAABBs(quad_aabbs);
	for (uint i = 0; i < quad_aabbs.size(); i++)
		DebugDraw(quad_aabbs[i], White);

	// Draw gameobjects AABBs
	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++) {
		DebugDraw((*it)->bounding_box, Red);
	}

	if (main_camera != nullptr) {
		ComponentCamera* camera_co = (ComponentCamera*)main_camera->FindComponentWithType(Component::Component::COMPONENT_CAMERA);
		DebugDraw(camera_co->frustum);
	}
}

// Called before quitting
bool trMainScene::CleanUp()
{
	RELEASE(default_mesh);
	RELEASE(grid);
	RELEASE(root);
	return true;
}

void trMainScene::ClearScene()
{
	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++)
		(*it)->to_destroy = true;
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

void trMainScene::InsertGoInQuadtree(GameObject * go)
{
	static_go.push_back(go);
	quadtree.Insert(go);
}

void trMainScene::EraseGoInQuadtree(GameObject * go)
{
	for (std::list<GameObject*>::iterator it = static_go.begin(); it != static_go.end(); it++) {
		if (go == (*it)) {
			static_go.erase(it);
			ReDoQuadtree();
			break;
		}
	}
}

void trMainScene::ReDoQuadtree()
{
	quadtree.Clear();
	quadtree.Create(AABB(AABB(float3(-500, -100, -500), float3(500, 100, 500))));

	for (std::list<GameObject*>::iterator it = static_go.begin(); it != static_go.end(); it++)
			quadtree.Insert((*it));
}

GameObject * trMainScene::CreateGameObject(GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject("unnamed", parent);
}

GameObject * trMainScene::CreateGameObject(const char * name,GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject(name, parent);
}