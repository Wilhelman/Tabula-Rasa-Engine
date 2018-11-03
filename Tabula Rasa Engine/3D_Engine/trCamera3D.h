#ifndef __trCAMERA3D_H__
#define __trCAMERA3D_H__

#include "trDefs.h" 

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#include "trApp.h"

class trCamera3D : public trModule
{
public:
	trCamera3D();
	~trCamera3D();

	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool Update(float dt);
	void ProcessMouseWheelInput(vec &new_pos, float speed);
	void ProcessKeyboardInput(vec &new_pos, float speed);
	void ProcessMouseMotion(int dx, int dy, float sensitivity);
	bool CleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	float* GetViewMatrix();

	void FocusOnSelectedGO();

private:

	void CalculateViewMatrix();
	
public:

	vec X, Y, Z, pos, ref;

private:

	float4x4 view_matrix, view_inv_matrix;

	float rotation_sensitivity = 0.25f, orbit_sensitivity = 0.25f, 
		  pan_sensitivity = 0.01f, cam_speed = 7.f, cam_boost_speed = 14.f;
};

#endif