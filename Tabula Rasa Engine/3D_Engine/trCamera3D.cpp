#include "Globals.h"
#include "trApp.h"
#include "trInput.h"
#include "trCamera3D.h"

trCamera3D::trCamera3D() : trModule()
{
	CalculateViewMatrix();

	X = vec(1.0f, 0.0f, 0.0f);
	Y = vec(0.0f, 1.0f, 0.0f);
	Z = vec(0.0f, 0.0f, 1.0f);

	Position = vec(0.0f, 0.0f, 5.0f);
	Reference = vec(0.0f, 0.0f, 0.0f);
}

trCamera3D::~trCamera3D()
{}

// -----------------------------------------------------------------
bool trCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool trCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
bool trCamera3D::Update(float dt)
{
	// Implement a debug camera with keys and mouse
	// Now we can make this movememnt frame rate independant!
	
	vec newPos(0.0f, 0.0f, 0.0f);
	float speed = 3.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) newPos.y -= speed;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;

	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		int dx = -App->input->GetMouseXMotion();
		int dy = -App->input->GetMouseYMotion();

		float Sensitivity = 0.25f;

		Position -= Reference;

		if (dx != 0)
		{
			float DeltaX = (float)dx * Sensitivity;
			
			X = rotate(X, DeltaX, vec(0.0f, 1.0f, 0.0f));
			Y = rotate(Y, DeltaX, vec(0.0f, 1.0f, 0.0f));
			Z = rotate(Z, DeltaX, vec(0.0f, 1.0f, 0.0f));

			math::Quat()
		}

		if (dy != 0)
		{
			float DeltaY = (float)dy * Sensitivity;

			Y = rotate(Y, DeltaY, X);
			Z = rotate(Z, DeltaY, X);

			if (Y.y < 0.0f)
			{
				Z = vec(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
				Y = math::Cross(Z, X);
			}
		}

		Position = Reference + Z * Position.Length();
	}
	
	// Recalculate matrix -------------
	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void trCamera3D::Look(const vec &Position, const vec &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;
	vec temp = Position - Reference;

	Z = temp.Normalized();
	X = Cross(vec(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void trCamera3D::LookAt(const vec &Spot)
{
	Reference = Spot;
	vec temp = Position - Reference;

	Z = temp.Normalized();
	X = Cross(vec(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void trCamera3D::Move(const vec &Movement)
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
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}