#include "trDefs.h"
#include "trApp.h"
#include "trInput.h"
#include "trWindow.h"
#include "trCamera3D.h"


trCamera3D::trCamera3D() : trModule()
{
	this->name = "Camera 3D";
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	pos = vec3(0.0f, 0.0f, 5.0f);
	ref = vec3(0.0f, 0.0f, 0.0f);
}

trCamera3D::~trCamera3D()
{}

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
	vec3 new_pos(0.0f, 0.0f, 0.0f);

	float speed = CAM_SPEED * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = CAM_BOOST_SPEED * dt;

	// ----- Camera movement with with keyboard -----

	ProcessKeyboardInput(new_pos, speed);

	// ----- Camera zoom-in / zoom-out with mouse wheel -----

	ProcessMouseWheelInput(new_pos, speed);

	pos += new_pos;
	ref += new_pos;

	// ----- Camera FPS-like rotation with mouse -----

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		ProcessMouseMotiont(dx, dy, ORBIT_SENSITIVITY);
	}

	// ----- Camera orbit around target with mouse and panning -----

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT
		     && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (b_box != nullptr)
			LookAt(vec3(b_box->Centroid().x, b_box->Centroid().y, b_box->Centroid().z));
		else
			LookAt(vec3(0.f, 0.f, 0.f));

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		pos -= ref;

		ProcessMouseMotiont(dx, dy, ROTATION_SENSITIVITY);

		pos = ref + Z * length(pos);
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		float pan_sensitivity = 0.01f;

		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		new_pos += X * dx * pan_sensitivity;
		new_pos += Y * dy * pan_sensitivity;

		pos += new_pos;
		ref += new_pos;
	}
	
	// ----- Recalculate matrix -----

	CalculateViewMatrix();

	return true;
}

void trCamera3D::ProcessMouseWheelInput(vec3 &new_pos, float speed)
{
	if (App->input->GetMouseZ() > 0)
		new_pos -= Z * speed * 2.f;

	if (App->input->GetMouseZ() < 0)
		new_pos += Z * speed * 2.f;
}

void trCamera3D::ProcessKeyboardInput(vec3 &new_pos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) new_pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) new_pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos += Z * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos += X * speed;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) CenterOnScene();
}

void trCamera3D::ProcessMouseMotiont(int dx, int dy, float sensitivity)
{
	if (dx != 0)
	{
		float DeltaX = (float)dx * sensitivity;

		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
	}

	if (dy != 0)
	{
		float DeltaY = (float)dy * sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}
}

// -----------------------------------------------------------------
void trCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->pos = Position;
	this->ref = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->ref = this->pos;
		this->pos += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void trCamera3D::LookAt(const vec3 &Spot)
{
	ref = Spot;

	Z = normalize(pos - ref);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void trCamera3D::Move(const vec3 &Movement)
{
	pos += Movement;
	ref += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* trCamera3D::GetViewMatrix()
{
	return &view_matrix;
}

// -----------------------------------------------------------------
void trCamera3D::CalculateViewMatrix()
{
	view_matrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, pos), -dot(Y, pos), -dot(Z, pos), 1.0f);
	view_inv_matrix = inverse(view_matrix);
}

void trCamera3D::CenterOnScene(AABB* bounding_box)
{
	if (bounding_box != nullptr)
		b_box = bounding_box;

	if (b_box != nullptr)
	{
		vec center_bbox(b_box->Centroid());
		vec move_dir = (vec(pos.x, pos.y, pos.z) - center_bbox).Normalized();

		float radius = b_box->MinimalEnclosingSphere().r;
		double fov = DEG_TO_RAD(60.0f);
		double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

		vec final_pos = center_bbox + move_dir * cam_distance;
		pos = vec3(final_pos.x, final_pos.y, final_pos.z);

		if (pos.y < 0.f)
			pos.y *= -1.f;

		LookAt(vec3(center_bbox.x, center_bbox.y, center_bbox.z));
	}
}