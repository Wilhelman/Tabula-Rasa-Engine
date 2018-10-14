#ifndef __trCAMERA3D_H__
#define __trCAMERA3D_H__

#include "trDefs.h" 

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#include "trApp.h"

//TODO -> DELETE: using it temporaly for camera rotation
#include "glmath.h"

class trCamera3D : public trModule
{
public:
	trCamera3D();
	~trCamera3D();

	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool Update(float dt);
	void ProcessMouseWheelInput(vec3 &new_pos, float speed);
	void ProcessKeyboardInput(vec3 &new_pos, float speed);
	void ProcessMouseMotion(int dx, int dy, float sensitivity);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void CenterOnScene(AABB* bounding_box = nullptr);

private:

	void CalculateViewMatrix();

public:

	vec3 X, Y, Z, pos, ref;

private:

	mat4x4 view_matrix, view_inv_matrix;
	AABB* b_box = nullptr;

	float rotation_sensitivity = 0.25f, orbit_sensitivity = 0.25f, 
		pan_sensitivity = 0.01f, cam_speed = 7.f, cam_boost_speed = 14.f;
};

#endif