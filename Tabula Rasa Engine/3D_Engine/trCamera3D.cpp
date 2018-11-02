#include "trDefs.h"
#include "trApp.h"
#include "trInput.h"
#include "trWindow.h"
#include "trCamera3D.h"


trCamera3D::trCamera3D() : trModule()
{
	this->name = "Camera3D";
	CalculateViewMatrix();

	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);

	pos = vec(0.0f, 0.0f, 5.0f);
	ref = vec(0.0f, 0.0f, 0.0f);
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
	vec new_pos(0.0f, 0.0f, 0.0f);

	float speed = cam_speed * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = cam_boost_speed * dt;

	// ----- Camera zoom-in / zoom-out with mouse wheel -----

	ProcessMouseWheelInput(new_pos, speed);

	// ----- Camera focus on geometry -----

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) CenterOnScene();

	// ----- Camera FPS-like rotation with mouse -----

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		ProcessKeyboardInput(new_pos, speed);

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		ProcessMouseMotion(dx, dy, rotation_sensitivity);
	}

	pos += new_pos;
	ref += new_pos;

	// ----- Camera orbit around target with mouse and panning -----
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT
		     && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (b_box != nullptr)
			LookAt(vec(b_box->Centroid().x, b_box->Centroid().y, b_box->Centroid().z));
		else
			LookAt(vec(0.f, 0.f, 0.f));

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		pos -= ref;

		ProcessMouseMotion(dx, dy, orbit_sensitivity);

		pos = ref + Z * pos.Length();
	}
	else if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		new_pos += X * dx * pan_sensitivity;
		new_pos += Y * dy * pan_sensitivity;

		pos += new_pos;
		ref += new_pos;
	}
	
	// ----- Recalculate view matrix -----

	CalculateViewMatrix();

	return true;
}

void trCamera3D::ProcessMouseWheelInput(vec &new_pos, float speed)
{
	if (App->input->GetMouseZ() > 0)
		new_pos -= Z * speed;

	if (App->input->GetMouseZ() < 0)
		new_pos += Z * speed;
}

void trCamera3D::ProcessKeyboardInput(vec &new_pos, float speed)
{
	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) new_pos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) != KEY_REPEAT) new_pos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) new_pos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) new_pos += Z * speed;

	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) new_pos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) new_pos += X * speed;
}

void trCamera3D::ProcessMouseMotion(int dx, int dy, float sensitivity)
{
	if (dx != 0)
	{
		float delta_x = -(float)dx * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateY(delta_x);
		
		X = X * rotate_mat;
		Y = Y * rotate_mat;
		Z = Z * rotate_mat;
	}

	if (dy != 0)
	{
		float delta_y = -(float)dy * sensitivity;

		float3x3 rotate_mat;
		rotate_mat = rotate_mat.RotateAxisAngle(X, delta_y);

		Y = Y * rotate_mat;
		Z = Z * rotate_mat;

		if (Y.y < -1.0f) // todo minimal issue orbiting obj
		{
			Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = Cross(Z, X);
		}
	}
}

// -----------------------------------------------------------------
void trCamera3D::Look(const vec &Position, const vec &Reference, bool RotateAroundReference)
{
	this->pos = Position;
	this->ref = Reference;

	Z = (Position - Reference).Normalized();
	X = (Cross(vec(0.0f, 1.0f, 0.0f), Z)).Normalized();;
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->ref = this->pos;
		this->pos += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void trCamera3D::LookAt(const vec &Spot)
{
	ref = Spot;

	Z = (pos - ref).Normalized();
	X = (Cross(vec(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void trCamera3D::Move(const vec &Movement)
{
	pos += Movement;
	ref += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* trCamera3D::GetViewMatrix()
{
	return view_matrix.ptr();
}

// -----------------------------------------------------------------
void trCamera3D::CalculateViewMatrix()
{
	view_matrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -Dot(X, pos), -Dot(Y, pos), -Dot(Z, pos), 1.0f);
	view_inv_matrix = view_matrix.Inverted();
}

void trCamera3D::CenterOnScene(AABB* bounding_box)
{
	if (bounding_box != nullptr)
		b_box = bounding_box;

	if (bounding_box == nullptr && b_box == nullptr) {
		pos.Set(3.f, 3.f, 3.f);
		LookAt(vec(0.f, 0.f, 0.f));
	}else if (b_box != nullptr)
	{
		vec center_bbox(b_box->Centroid());
		vec move_dir = (vec(pos.x, pos.y, pos.z) - center_bbox).Normalized();

		float radius = b_box->MinimalEnclosingSphere().r;
		double fov = DEG_TO_RAD(60.0f);
		double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

		vec final_pos = center_bbox + move_dir * cam_distance;
		pos = vec(final_pos.x, final_pos.y, final_pos.z);

		if (pos.y < 0.f)
			pos.y *= -1.f;

		LookAt(vec(center_bbox.x, center_bbox.y, center_bbox.z));
	}
}

void trCamera3D::ClearLastBoundingBox()
{
	if (b_box != nullptr)
		b_box = nullptr;
}
