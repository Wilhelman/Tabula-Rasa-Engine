#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"


class ComponentCamera : public Component
{

public:

	ComponentCamera(GameObject* embedded_game_object);
	~ComponentCamera();



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

#endif // __COMPONENT_CAMERA_H__