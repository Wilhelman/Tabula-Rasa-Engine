#include "trDefs.h"
#include "trApp.h"
#include "trInput.h"
#include "trWindow.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "GameObject.h"
#include "trMainScene.h"

#include "ComponentCamera.h"

#define ASPECT_RATIO 1.3f

trCamera3D::trCamera3D() : trModule()
{
	this->name = "Camera3D";

	dummy_camera = new ComponentCamera(nullptr);
}

trCamera3D::~trCamera3D()
{
	RELEASE(dummy_camera);
}

bool trCamera3D::Awake(JSON_Object* config)
{
	if (config != nullptr) {
		rotation_sensitivity = json_object_get_number(config,"rotation_sensitivity"); 
		orbit_sensitivity = json_object_get_number(config, "orbit_sensitivity");
		pan_sensitivity = json_object_get_number(config, "pan_sensitivity");
		cam_speed = json_object_get_number(config, "cam_speed");
		cam_boost_speed = json_object_get_number(config, "cam_boost_speed");
	}
	return true;
}

// -----------------------------------------------------------------
bool trCamera3D::Start()
{
	TR_LOG("trCamera3D: Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool trCamera3D::CleanUp()
{
	TR_LOG("trCamera3D: CleanUp");

	return true;
}

// -----------------------------------------------------------------
bool trCamera3D::Update(float dt)
{	
	// Drawing ray

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN)
		OnPick();

	// -----

	float3 new_pos(0.0f, 0.0f, 0.0f);

	float speed = cam_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = cam_boost_speed * dt;

	// ----- Camera zoom-in / zoom-out with mouse wheel -----

	ProcessMouseWheelInput(new_pos, speed);

	// ----- Camera focus on geometry -----

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		dummy_camera->FocusOnAABB(dummy_camera->last_aabb);
	}
	// ----- Camera FPS-like rotation with mouse -----

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		ProcessKeyboardInput(new_pos, speed);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		ProcessMouseMotion(dx, dy, rotation_sensitivity);
	}

	dummy_camera->frustum.pos += new_pos;

	// ----- Camera orbit around target with mouse and panning -----
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT
		     && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		float dx = (float)(-App->input->GetMouseXMotion()) * 0.001f;
		float dy = (float)(-App->input->GetMouseYMotion()) * 0.001f;

		float3 aim_point = float3::zero;

		GameObject* selected = App->editor->GetSelected();
		if (selected)
			aim_point = float3(selected->bounding_box.Centroid().x,
						       selected->bounding_box.Centroid().y, 
						       selected->bounding_box.Centroid().z);

		float3 dist_to_aim_point = dummy_camera->frustum.pos - aim_point;

		Quat rot_quat_y(dummy_camera->frustum.up, dx);
		Quat rot_quat_x(dummy_camera->frustum.WorldRight(), dy);

		dist_to_aim_point = rot_quat_x.Mul(dist_to_aim_point);
		dist_to_aim_point = rot_quat_y.Mul(dist_to_aim_point);

		dummy_camera->frustum.pos = dist_to_aim_point + aim_point;

		dummy_camera->LookAt(aim_point);
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		new_pos += dummy_camera->frustum.WorldRight() * dx * pan_sensitivity;
		new_pos += dummy_camera->frustum.up * dy * pan_sensitivity;

		dummy_camera->frustum.pos += new_pos;
	}
	
	return true;
}

void trCamera3D::ProcessMouseWheelInput(float3 &new_pos, float speed)
{
	if (App->input->GetMouseZ() > 0)
		new_pos += dummy_camera->frustum.front * speed;

	if (App->input->GetMouseZ() < 0)
		new_pos -= dummy_camera->frustum.front * speed;
}

void trCamera3D::ProcessKeyboardInput(float3 &new_pos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) new_pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT) new_pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos += dummy_camera->frustum.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos -= dummy_camera->frustum.front * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos -= dummy_camera->frustum.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos += dummy_camera->frustum.WorldRight() * speed;
}

void trCamera3D::ProcessMouseMotion(int dx, int dy, float sensitivity)
{
	if (dx != 0)
	{
		float delta_x = -(float)dx * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateY(delta_x);
		
		dummy_camera->frustum.up = dummy_camera->frustum.up * rotate_mat;
		dummy_camera->frustum.front = dummy_camera->frustum.front * rotate_mat;
	}

	if (dy != 0)
	{
		float delta_y = -(float)dy * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateAxisAngle(dummy_camera->frustum.WorldRight(), delta_y);

		dummy_camera->frustum.up = dummy_camera->frustum.up * rotate_mat;
		dummy_camera->frustum.front = dummy_camera->frustum.front * rotate_mat;

		if (dummy_camera->frustum.up.y < -1.0f) // todo minimal issue orbiting obj
		{
			dummy_camera->frustum.front = float3(0.0f, dummy_camera->frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			dummy_camera->frustum.up = Cross(dummy_camera->frustum.front, dummy_camera->frustum.WorldRight());
		}
	}
}

void trCamera3D::OnPick()
{
	float width = (float)App->window->GetWidth();
	float height = (float)App->window->GetHeight();

	int mouse_x = App->input->GetMouseX();
	int mouse_y = App->input->GetMouseY();

	float normalized_x = -(1.0f - (float(mouse_x) * 2.0f) / width);
	float normalized_y = 1.0f - (float(mouse_y) * 2.0f) / height;

	pick_ray = dummy_camera->frustum.UnProjectLineSegment(normalized_x, normalized_y);

	App->main_scene->TestAgainstRay(pick_ray);

	/*float distance;
	GameObject* hit = App->level->CastRay(picking, distance);

	if (hit != nullptr && hit_point != nullptr)
		*hit_point = picking.GetPoint(distance);*/
}
