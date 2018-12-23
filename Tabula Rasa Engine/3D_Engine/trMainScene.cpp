#include "trDefs.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trFileLoader.h"
#include "PGrid.h"
#include "ComponentCamera.h"
#include "ComponentMesh.h"
#include "ComponentBone.h"
#include "trEditor.h" //TODO: check this

#include "ResourceMesh.h"

#include "GameObject.h"
#include "DebugDraw.h"
#include <iostream> 
#include <stdio.h>
#include "trFileSystem.h"
#include "trInput.h"

#include "trAnimation.h"

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

	root = new GameObject("root", nullptr);
	root->CreateComponent(Component::component_type::COMPONENT_TRANSFORM);

	main_camera = new GameObject("Main Camera", root);
	main_camera->CreateComponent(Component::component_type::COMPONENT_TRANSFORM);

	App->render->active_camera = (ComponentCamera*)main_camera->CreateComponent(Component::component_type::COMPONENT_CAMERA);

	quadtree.Create(AABB(AABB(float3(-500, -100, -500), float3(500, 100, 500))));

	//scene_name = "TR Unnamed Scene";

	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	scene_bb = App->camera->dummy_camera->default_aabb;

	App->camera->dummy_camera->LookAt(float3(0.f, 0.f, 0.f));

	grid = new PGrid();
	grid->axis = true;

	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate(float dt)
{
	root->DestroyGameObjectsIfNeeded();

	// PreUpdate GOS with game dt if runtime
	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++) 
		(*it)->PreUpdate(dt);

	return true;
}

bool trMainScene::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN){
		if (App->animation->animations.size() > 1)
			App->animation->SetCurrentAnimation(1);
	}
	else if (App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN) {
		if (App->animation->animations.size() > 2) // TODO 3
			App->animation->SetCurrentAnimation(2);
	}

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
	RecursiveDebugDrawGameObjects(root);

	if (main_camera != nullptr) {
		ComponentCamera* camera_co = (ComponentCamera*)main_camera->FindComponentByType(Component::Component::COMPONENT_CAMERA);
		DebugDraw(camera_co->frustum);
		DebugDraw(App->camera->pick_ray, Blue);
	}
}

void trMainScene::RecursiveDebugDrawGameObjects(GameObject * go)
{
	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
	{
		ComponentBone* bone_comp = (ComponentBone*)(*it)->FindComponentByType(Component::COMPONENT_BONE);

		// Drawing bones
		float3 pos = float3::zero;
		(*it)->GetTransform()->GetLocalPosition(&pos, &float3(), &Quat()); 
		if (bone_comp) {
			DebugDraw(pos, Green, (*it)->GetTransform()->GetMatrix());
			math::LineSegment segment; 
			segment.a = (*it)->GetTransform()->GetMatrix().TranslatePart();

			for (std::list<GameObject*>::iterator it_childs = (*it)->childs.begin(); it_childs != (*it)->childs.end(); it_childs++) {
				segment.b = (*it_childs)->GetTransform()->GetMatrix().TranslatePart();
				DebugDraw(segment, Blue);
			}
		}
	}

	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
		RecursiveDebugDrawGameObjects((*it));
}

void trMainScene::RecursiveDeleteGos(GameObject * go, bool and_camera)
{
	if (and_camera) {
		go->to_destroy = true;
		go->is_active = false; // doing this, renderer will ignore it till is destroyed
	}
	else {
		if (go != main_camera) {
			go->to_destroy = true;
			go->is_active = false; // doing this, renderer will ignore it till is destroyed
		}
	}
	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++) {
		RecursiveDeleteGos((*it), and_camera);
	}
}

void trMainScene::RecursiveSetupGo(GameObject * go, bool only_animation)
{
	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++) {
		(*it)->is_static = true;
	}

	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++) {
		if ((*it)->is_static && (*it)->to_destroy == false && !(*it)->FindComponentByType(ComponentMesh::COMPONENT_BONE)){
			App->main_scene->InsertGoInQuadtree((*it));
		}
	}

	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++) {
		if ((*it)->to_destroy == false && (*it)->FindComponentByType(ComponentMesh::COMPONENT_MESH)) {
			(*it)->FindComponentByType(ComponentMesh::COMPONENT_MESH)->Start();
		}
	}

	for (std::list<GameObject*>::iterator it = go->childs.begin(); it != go->childs.end(); it++)
	{
		if((*it)->to_destroy == false)
			RecursiveSetupGo((*it));
	}
		
}

// Called before quitting
bool trMainScene::CleanUp()
{
	RELEASE(grid);
	RELEASE(root);
	return true;
}

void trMainScene::ClearScene(bool delete_camera)
{
	//todo rec

	for (std::list<GameObject*>::iterator it = root->childs.begin(); it != root->childs.end(); it++) {
		RecursiveDeleteGos((*it), delete_camera);
	}

	if(delete_camera)
		this->main_camera = nullptr;

	static_go.clear();
	dinamic_go.clear();

	ReDoQuadtree();
}

void trMainScene::Draw()
{
	grid->Render();
}

// Load Game State
bool trMainScene::Load(const JSON_Object* config)
{
	return true;
}

// Save Game State
bool trMainScene::Save(JSON_Object* config)const 
{

	return true;
}

bool trMainScene::SerializeScene(std::string& output_file, const char* force_name)
{
	//if (force_name)
	std::string file_name;
	App->file_system->GetFileFileNameFromPath(force_name, file_name);
	this->scene_name = file_name;

	JSON_Value* root_value = nullptr;
	JSON_Object* root_obj = nullptr;
	JSON_Array* array = nullptr;

	root_value = json_value_init_object();

	// Scene stuff
	root_obj = json_value_get_object(root_value);
	json_object_set_string(root_obj, "Name", scene_name.c_str());

	// Go's stuff
	JSON_Value* go_value = json_value_init_array();
	array = json_value_get_array(go_value);
	json_object_set_value(root_obj, "GameObjects", go_value);

	/// Iterating between all gos
	for (std::list<GameObject*>::const_iterator it = root->childs.begin(); it != root->childs.end(); it++) {
		if(!(*it)->to_destroy)
			(*it)->Save(array);
	}

	char *serialized_string = NULL;

	serialized_string = json_serialize_to_string_pretty(root_value);
	puts(serialized_string);

	std::string final_path = A_SCENES_DIR;
	final_path.append("/");
	final_path.append(scene_name.c_str());
	final_path.append(".trScene");
	json_serialize_to_file(root_value, final_path.c_str());
	output_file = final_path;
	json_free_serialized_string(serialized_string);
	json_value_free(root_value);

	return true;
}

bool trMainScene::DeSerializeScene(const char * string, bool only_animation)
{
	JSON_Value* root_value = json_parse_string(string);
	JSON_Object* root_obj = json_value_get_object(root_value); 
	
	//JSON_Object* description = json_object_get_object(root, "");
	JSON_Value* value = json_object_get_value(root_obj, "Name");
	scene_name = json_value_get_string(value);

	/// todo load camera state?
	std::map<GameObject*, UID> uuid_relations;

	// Get GameObjects Array
	JSON_Array* array = json_object_get_array(root_obj, "GameObjects");

	if (array != nullptr) {
		uint go_size = json_array_get_count(array);

		
		for (uint i = 0u; i < go_size; ++i)
		{
			GameObject* go = this->CreateGameObject("unnamed for now");
			
			JSON_Object* go_obj = json_array_get_object(array, i);

			go->Load(go_obj, uuid_relations);
			if (only_animation && go->FindComponentByType(Component::component_type::COMPONENT_CAMERA) == nullptr)
				go->to_destroy = true;
		}
	}

	for (std::map<GameObject*, UID>::iterator it = uuid_relations.begin(); it != uuid_relations.end(); ++it)
	{
		uint parent_id = it->second;
		GameObject* go = it->first;

		if (parent_id > 0)
		{
			GameObject* parent_go = FindGoByUUID(parent_id, this->GetRoot());
			if (parent_go != nullptr)
				go->SetParent(parent_go);
		}
	}

	App->main_scene->GetRoot()->RecalculateBoundingBox();
	scene_bb.SetNegativeInfinity();
	bool mesh_find = false;

	for (std::list<GameObject*>::iterator it = App->main_scene->GetRoot()->childs.begin(); it != App->main_scene->GetRoot()->childs.end(); it++)
	{
		if (!(*it)->to_destroy && (*it)->FindComponentByType(ComponentMesh::COMPONENT_MESH) != nullptr)
		{
			AABB current_bb = (*it)->bounding_box;
			scene_bb.Enclose(current_bb);

			if (!mesh_find) mesh_find = true;
		}
	}

	if (!mesh_find)
		scene_bb = App->camera->dummy_camera->default_aabb;

	App->main_scene->scene_bb = scene_bb;
	App->camera->dummy_camera->FocusOnAABB(scene_bb);

	RecursiveSetupGo(GetRoot(), only_animation);

	return true;
}

GameObject * trMainScene::FindGoByUUID(UID uid, GameObject* go)
{
	if (uid == go->GetUUID() && go->to_destroy == false)
		return go;

	GameObject* ret = nullptr;

	for (std::list<GameObject*>::const_iterator it = go->childs.begin(); it != go->childs.end() && ret == nullptr; ++it)
		ret = FindGoByUUID(uid, *it);

	return ret;
}

GameObject * trMainScene::GetRoot() const
{
	return root;
}

void trMainScene::InsertGoInQuadtree(GameObject * go) // This GO is now static
{
	if (go->FindComponentByType(ComponentMesh::COMPONENT_BONE))
		return;

	if (go != main_camera) {
		if (!go->to_destroy) {
			static_go.push_back(go);
			quadtree.Insert(go);
		}
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
	if (go->FindComponentByType(ComponentMesh::COMPONENT_BONE))
		return;

	if (go != main_camera) {
		if(!go->to_destroy)
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
	std::map<float, GameObject*> intersect_map;
	GameObject* selected_go = nullptr;
	float min_distance = App->camera->dummy_camera->frustum.farPlaneDistance;

	// Collecting all STATIC gameobjects whose AABBs have intersected with the line segment
	quadtree.CollectIntersectingGOs(line_segment, intersect_map);

	// Collecting all DYNAMIC gameobjects (as they are not in the quadtree, it only accepts STATIC objects inside)
	std::vector<GameObject*> intersect_dynamic_vec;
	CollectDinamicGOs(intersect_dynamic_vec);

	/* Checking if these dynamic gameobjects are inside the fustrum. If so, checking if they intersect with the line
	   segment; in that case, we put them in our map along with their hit distance. As we use a map (with hit distance 
	   value as key) it is already ordered in ascending order by default. This means the gameobjects are already sorted 
	   by their AABBs distance to the camera, so we will check first the closer gameobjects to speed up the process. */
	for (uint i = 0; i < intersect_dynamic_vec.size(); i++)
	{
		AABB current_bounding_box = intersect_dynamic_vec[i]->bounding_box;
		if (App->camera->dummy_camera->FrustumContainsAaBox(current_bounding_box))
		{
			float hit_distance;
			float out_distance;
			if (line_segment.Intersects(current_bounding_box, hit_distance, out_distance))
				intersect_map.insert(std::pair<float, GameObject*>(hit_distance, intersect_dynamic_vec[i]));
		}
	}
	
	// Now we can go through the map checking each gameobject's triangle against the line segment
	for (std::map<float, GameObject*>::iterator it_map = intersect_map.begin(); it_map != intersect_map.end(); it_map++)
	{
		const ComponentMesh* mesh_comp = (ComponentMesh*)it_map->second->FindComponentByType(Component::COMPONENT_MESH);
		
		// Making sure the intersecting gameobject has a mesh component
		if (mesh_comp != nullptr && mesh_comp->GetResource() != nullptr)
		{
			ResourceMesh* mesh = (ResourceMesh*)mesh_comp->GetResource();
			/* Checking if the current minimum distance to the camera is greater than the AABB's hit distance of 
			   the current intersecting gameobject in the loop. If it's not, we can safely discard this gameobject 
			   as it's further away from the previous one. This way, we prevent the program from calculating all 
			   the mesh triangles so the mouse picking process will be faster. */
			if (min_distance >= (*it_map).first)
			{
				// Transforming line segment into intersecting gameobject's local space
				LineSegment segment_local_space(line_segment);
				segment_local_space.Transform(it_map->second->GetTransform()->GetMatrix().Inverted());

				Triangle tri;
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
							// If it is we save its hit distance (unless another triangle is closer in next lap)
							min_distance = hit_distance;
							selected_go = it_map->second;
						}
					}
				}
			}
		}
	}

	// Finally, we set the resulting selected gameobject
	App->editor->SetSelected(selected_go);
}											   
											   

GameObject * trMainScene::CreateGameObject(GameObject * parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject("unnamed", parent);
}

GameObject * trMainScene::CreateGameObject(const char* name, GameObject* parent)
{
	if (parent == nullptr)
		parent = root;

	return new GameObject(name, parent);
}