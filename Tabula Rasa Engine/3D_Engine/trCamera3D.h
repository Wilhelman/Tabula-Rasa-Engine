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
	void ProcessMouseWheelInput(float3 &new_pos, float speed);
	void ProcessKeyboardInput(float3 &new_pos, float speed);
	void ProcessMouseMotion(int dx, int dy, float sensitivity);
	bool CleanUp();

	void LookAt(const float3 &Spot);

	void SetAspectRatio(float new_aspect_ratio);

	float* GetViewMatrix();
	float* GetProjectionMatrix();

	void FocusOnSelectedGO();

	
public:

	Frustum frustum;
	bool projection_needs_update = false;

private:

	float4x4 gl_view_matrix = float4x4::zero;
	float4x4 gl_projection_matrix = float4x4::zero;

	float rotation_sensitivity = 0.25f, orbit_sensitivity = 0.25f, 
		  pan_sensitivity = 0.01f, cam_speed = 7.f, cam_boost_speed = 14.f;
};

#endif