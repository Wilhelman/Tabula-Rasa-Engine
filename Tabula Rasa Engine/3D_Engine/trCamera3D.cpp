#include "trDefs.h"
#include "trApp.h"
#include "trInput.h"
#include "trWindow.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "GameObject.h"

#define ASPECT_RATIO 1.3f

trCamera3D::trCamera3D() : trModule()
{
	this->name = "Camera3D";

	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3(-2.f,3.f,-10.f);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = math::DegToRad(60.0f);

	SetAspectRatio(ASPECT_RATIO);

	projection_needs_update = true;
}

trCamera3D::~trCamera3D()
{}

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
	float3 new_pos(0.0f, 0.0f, 0.0f);

	float speed = cam_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = cam_boost_speed * dt;

	// ----- Camera zoom-in / zoom-out with mouse wheel -----

	ProcessMouseWheelInput(new_pos, speed);

	// ----- Camera focus on geometry -----

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) FocusOnSelectedGO();

	// ----- Camera FPS-like rotation with mouse -----

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		ProcessKeyboardInput(new_pos, speed);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		ProcessMouseMotion(dx, dy, rotation_sensitivity);
	}

	// WHAT IS THIS
	frustum.pos += new_pos;
	//WHAT IS THIS
	looking_at += new_pos;

	// ----- Camera orbit around target with mouse and panning -----
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT
		     && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		GameObject* selected = App->editor->GetSelected();
		if(selected)
			LookAt(float3(selected->bounding_box.Centroid().x, selected->bounding_box.Centroid().y, selected->bounding_box.Centroid().z));
		else
			LookAt(float3(0.f, 0.f, 0.f));

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		//WHAT IS THIS
		frustum.pos -= looking_at;

		ProcessMouseMotion(dx, dy, orbit_sensitivity);
		//WHAT IS THIS
		frustum.pos = looking_at + frustum.front * frustum.pos.Length();
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		new_pos += frustum.WorldRight() * dx * pan_sensitivity;
		new_pos += frustum.up * dy * pan_sensitivity;

		frustum.pos += new_pos;
		looking_at += new_pos;
	}
	
	return true;
}

void trCamera3D::ProcessMouseWheelInput(float3 &new_pos, float speed)
{
	if (App->input->GetMouseZ() > 0)
		new_pos += frustum.front * speed;

	if (App->input->GetMouseZ() < 0)
		new_pos -= frustum.front * speed;
}

void trCamera3D::ProcessKeyboardInput(float3 &new_pos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) new_pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT) new_pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos += frustum.front * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos -= frustum.front * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos -= frustum.WorldRight() * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos += frustum.WorldRight() * speed;
}

void trCamera3D::ProcessMouseMotion(int dx, int dy, float sensitivity)
{
	if (dx != 0)
	{
		float delta_x = -(float)dx * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateY(delta_x);
		
		//X = X * rotate_mat;
		frustum.up = frustum.up * rotate_mat;
		frustum.front = frustum.front * rotate_mat;
	}

	if (dy != 0)
	{
		float delta_y = -(float)dy * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateAxisAngle(frustum.WorldRight(), delta_y);

		frustum.up = frustum.up * rotate_mat;
		frustum.front = frustum.front * rotate_mat;

		if (frustum.up.y < -1.0f) // todo minimal issue orbiting obj
		{
			frustum.front = float3(0.0f, frustum.front.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			frustum.up = Cross(frustum.front, frustum.WorldRight());
		}
	}
}

// -----------------------------------------------------------------
void trCamera3D::LookAt(const float3 &Spot)
{
	//TODO CHECK THIS
	float3 dir = Spot - frustum.pos;

	float3x3 m = float3x3::LookAt(frustum.front, dir.Normalized(), frustum.up, float3::unitY);

	frustum.front = m.MulDir(frustum.front).Normalized();
	frustum.up = m.MulDir(frustum.up).Normalized();


	/*
	looking_at = Spot;

	frustum.front = (frustum.pos - looking_at).Normalized();
	//X = (Cross(vec(0.0f, 1.0f, 0.0f), Z)).Normalized();
	frustum.up = Cross(frustum.front, frustum.WorldRight());*/
}


// -----------------------------------------------------------------
void trCamera3D::Move(const float3 &Movement)
{
	/*frustum.pos += Movement;
	looking_at += Movement;*/
}

void trCamera3D::SetAspectRatio(float new_aspect_ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.f) * new_aspect_ratio);
	projection_needs_update = true;
}

// -----------------------------------------------------------------
float* trCamera3D::GetViewMatrix()
{
	gl_view_matrix = frustum.ViewMatrix();

	return gl_view_matrix.Transposed().ptr();
}

float * trCamera3D::GetProjectionMatrix()
{
	gl_projection_matrix = frustum.ProjectionMatrix().Transposed();

	return gl_projection_matrix.ptr();
}

void trCamera3D::FocusOnSelectedGO()
{
	GameObject* selected = App->editor->GetSelected();
	if (selected) {

		selected->RecalculateBoundingBox(); // TODO: bad feelings ...

		float3 center_bbox(selected->bounding_box.Centroid());
		float3 move_dir = (frustum.pos - center_bbox).Normalized();

		float radius = selected->bounding_box.MinimalEnclosingSphere().r;
		double fov = DEG_TO_RAD(60.0f);
		double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

		float3 final_pos = center_bbox + move_dir * cam_distance;
		frustum.pos = float3(final_pos.x, final_pos.y, final_pos.z);

		if (frustum.pos.y < 0.f)
			frustum.pos.y *= -1.f;

		LookAt(float3(center_bbox.x, center_bbox.y, center_bbox.z));
	}else{
		frustum.pos.Set(3.f, 3.f, 3.f);
		LookAt(float3(0.f, 0.f, 0.f));
	}
}