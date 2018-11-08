#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trFileLoader.h"
#include "PGrid.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "trEditor.h" //TODO: check this

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

	App->render->active_camera = (ComponentCamera*)main_camera->CreateComponent(Component::component_type::COMPONENT_CAMERA);

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

	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++)
		(*it)->PreUpdate(dt);

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
		ComponentCamera* camera_co = (ComponentCamera*)main_camera->FindComponentByType(Component::Component::COMPONENT_CAMERA);
		DebugDraw(camera_co->frustum);
		DebugDraw(App->camera->pick_ray, Blue);
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
	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++) {
		if ((*it) != main_camera) {
			(*it)->to_destroy = true;
			(*it)->is_active = false; // doing this, renderer will ignore it till is destroyed
		}
	}
	static_go.clear();
	dinamic_go.clear();

	ReDoQuadtree();
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

void trMainScene::InsertGoInQuadtree(GameObject * go) // This GO is now static
{
	if (go != main_camera && !go->to_destroy) {
		static_go.push_back(go);
		quadtree.Insert(go);
		for (std::list<GameObject*>::iterator it = dinamic_go.begin(); it != dinamic_go.end(); it++) {
			if ((*it) == go) {
				dinamic_go.erase(it);
				break;
			}
		}
			
	}
}

void trMainScene::EraseGoInQuadtree(GameObject * go) // This go is now dinamic
{
	if (go != main_camera) {
		dinamic_go.push_back(go);
		for (std::list<GameObject*>::iterator it = static_go.begin(); it != static_go.end(); it++) {
			if (go == (*it)) {
				static_go.erase(it);
				ReDoQuadtree();
				break;
			}
		}
	}
}

void trMainScene::CollectDinamicGOs(std::vector<GameObject*>& dinamic_vector)
{
	for (std::list<GameObject*>::iterator it = dinamic_go.begin(); it != dinamic_go.end(); it++) {
		if (!(*it)->to_destroy)
			dinamic_vector.push_back((*it));
	}
		
}

void trMainScene::ReDoQuadtree()
{
	quadtree.Clear();
	quadtree.Create(AABB(AABB(float3(-500, -100, -500), float3(500, 100, 500))));

	for (std::list<GameObject*>::iterator it = static_go.begin(); it != static_go.end(); it++) {
		if(!(*it)->to_destroy)
			quadtree.Insert((*it));
	}
			
}

void trMainScene::TestAgainstRay(LineSegment line_segment)
{
	std::vector<GameObject*> intersect_vec;
	GameObject* selected_go = nullptr;

	// Collecting all gameobjects whose AABBs have intersected with the line segment
	quadtree.CollectIntersectingGOs(line_segment, intersect_vec);
	
	for (uint i = 0; i < intersect_vec.size(); i++)
	{
		ComponentMesh* mesh_comp = (ComponentMesh*)intersect_vec[i]->FindComponentByType(Component::COMPONENT_MESH);

		// Making sure the intersecting gameobject has a mesh component
		if (mesh_comp != nullptr)
		{
			// Transforming line segment into intersecting gameobject's local space
			LineSegment segment_local_space(line_segment);
			segment_local_space.Transform(intersect_vec[i]->GetTransform()->GetMatrix().Inverted());

			Triangle tri;
			const Mesh* mesh = mesh_comp->GetMesh();
			float min_distance = App->camera->dummy_camera->frustum.farPlaneDistance;
			uint index_counter = 0;

			while (index_counter < mesh->index_size)
			{
				// Creating gameobject's mesh triangles
				tri.a.x = mesh->vertices[mesh->indices[index_counter] * 3];
				tri.a.y = mesh->vertices[mesh->indices[index_counter] * 3 + 1];
				tri.a.z = mesh->vertices[mesh->indices[index_counter++] * 3 + 2];

				tri.b.x = mesh->vertices[mesh->indices[index_counter] * 3];
				tri.b.y = mesh->vertices[mesh->indices[index_counter] * 3 + 1];
				tri.b.z = mesh->vertices[mesh->indices[index_counter++] * 3 + 2];

				tri.c.x = mesh->vertices[mesh->indices[index_counter] * 3];
				tri.c.y = mesh->vertices[mesh->indices[index_counter] * 3 + 1];
				tri.c.z = mesh->vertices[mesh->indices[index_counter++] * 3 + 2];

				// Checking if line has interseted with each triangle
				float hit_distance = 0.0f;
				float3 hit_point = float3::zero;

				if (segment_local_space.Intersects(tri, &hit_distance, &hit_point))
				{
					// If triangle intersects we neeed to check if it's the closest one of all of them
					if (hit_distance < min_distance)
					{
						// If it is we save this hit point unless another triangle is closer
						min_distance = hit_distance;
						selected_go = intersect_vec[i];
					}
				}
			}
		}
	}

	App->editor->SetSelected(selected_go);

	/*AABB closest_bounding_box;
	closest_bounding_box.SetNegativeInfinity();

	for (uint i = 0; i < intersect_vec.size(); i++)
	{
		if (i == 0)
		{
			closest_bounding_box = intersect_vec[i]->bounding_box;
			selected_go = intersect_vec[i];
		}
		else if (intersect_vec[i]->bounding_box.CenterPoint().Length()
				 < closest_bounding_box.CenterPoint().Length())
		{
			selected_go = intersect_vec[i];
			closest_bounding_box = intersect_vec[i]->bounding_box;
		}
	}

		App->editor->SetSelected(selected_go); */
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