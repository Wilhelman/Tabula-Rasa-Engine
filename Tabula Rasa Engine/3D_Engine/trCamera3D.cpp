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

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);
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
	vec3 newPos(0.0f, 0.0f, 0.0f);
	float speed = 5.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_T) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_G) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) CenterOnScene();

	if (App->input->GetMouseZ() > 0)
		newPos -= Z * speed * 2.f;

	if (App->input->GetMouseZ() < 0)
		newPos += Z * speed * 2.f;

	Position += newPos;
	Reference += newPos;

	// TODO: this should orbit the obj. Check LookAt to see why the camera always points there once called
	//if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN) 
		//LookAt(vec3(0.f, 0.f, 0.f));

	// Mouse motion ----------------

	//if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_DOWN)
		//LookAt(vec3(0.f, 0.f, 0.f));

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		float orbit_sensitivity = 0.25f;

		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		if (dx != 0)
		{
			float DeltaX = (float)dx * orbit_sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * orbit_sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			// Uncomment teh code below to cap camera rotation on Z / X axis by +/-90 degrees
			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

	}
	
	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT
		&& App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
	{
		if (b_box != nullptr)
			LookAt(vec3(b_box->Centroid().x, b_box->Centroid().y, b_box->Centroid().z));
		else
			LookAt(vec3(0.f, 0.f, 0.f));


		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;

			X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = cross(Z, X);
			}
		}

		Position = Reference + Z * length(Position);


	}

	
	
	if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE))
	{
		float pan_sensitivity = 0.01f;

		int dx = -App->input->GetMouseXMotion();
		int dy = App->input->GetMouseYMotion();

		newPos += X * dx * pan_sensitivity;
		newPos += Y * dy * pan_sensitivity;

		Position += newPos;
		Reference += newPos;
	}
	
	
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return true;
}

// -----------------------------------------------------------------
void trCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void trCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void trCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* trCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

// -----------------------------------------------------------------
void trCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void trCamera3D::CenterOnScene(AABB* bounding_box)
{
	if (bounding_box != nullptr)
		b_box = bounding_box;

	if (b_box != nullptr)
	{
		vec center_bbox(b_box->Centroid());
		vec move_dir = (vec(Position.x, Position.y, Position.z) - center_bbox).Normalized();

		float radius = b_box->MinimalEnclosingSphere().r;
		double fov = DEG_TO_RAD(60.0f);
		double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

		vec final_pos = center_bbox + move_dir * cam_distance;
		Position = vec3(final_pos.x, final_pos.y, final_pos.z);

		if (Position.y < 0.f)
			Position.y *= -1.f;

		LookAt(vec3(center_bbox.x, center_bbox.y, center_bbox.z));
	}
}