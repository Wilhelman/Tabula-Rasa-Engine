#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trWindow.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "trPrimitives.h"

#include "PCube.h"
#include "PPlane.h"
#include "PArrow.h"
#include "PFrustum.h"
#include "PCylinder.h"
#include "PSphere.h"



#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

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

	App->camera->Move(vec3(1.f, 1.f, 1.f));
	App->camera->LookAt(vec3(0.f, 0.f, 0.f));


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

void trMainScene::Draw()
{
	//todo check this
	/*cube_test = new PCube();
	cube_test->axis = true;
	cube_test->color = Color(0.2f, 0.2f, 1.f, 1.f);
	cube_test->Render();*/

	//todo check this
	plane_test = new PPlane();
	plane_test->Render();

	//for some reason import only draws if we render with primitives too
	/*plane_test->axis = true;
	plane_test->Render();*/

	/*
	PArrow p_arrow(math::vec(0.f, 2.5f, 0.f), math::float4(0.f, 1.0f, 0.f, 1.f));
	p_arrow.axis = true;
	p_arrow.Render();
	*/
	/*PFrustum frustum;
	frustum.Render();*/
	/*
	PSphere sphere(math::vec(2.f, 3.f, -2.f), 1);
	sphere.Render();

	PSphere sphere2(math::vec(2.f, 6.f, -2.f), 3);
	sphere2.Render();*/
	
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