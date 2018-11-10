#ifndef __trCAMERA3D_H__
#define __trCAMERA3D_H__

#include "trDefs.h" 

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#include "trApp.h"

class ComponentCamera;

class trCamera3D : public trModule
{
public:
	trCamera3D();
	~trCamera3D();

	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool Update(float dt);

	// Load / Save
	bool Load(const JSON_Object* config = nullptr);
	bool Save(JSON_Object* config = nullptr) const;

	void ProcessMouseWheelInput(float3 &new_pos, float speed);
	void ProcessKeyboardInput(float3 &new_pos, float speed);
	void ProcessMouseMotion(int dx, int dy, float sensitivity);

	void OnPickGameObject();

	bool CleanUp();
	
public:

	ComponentCamera* dummy_camera = nullptr;
	LineSegment pick_ray;

private:

	float4x4 gl_view_matrix = float4x4::zero;
	float4x4 gl_projection_matrix = float4x4::zero;

	float rotation_sensitivity = 0.25f, orbit_sensitivity = 0.25f, 
		  pan_sensitivity = 0.01f, cam_speed = 7.f, cam_boost_speed = 14.f;



};

#endif