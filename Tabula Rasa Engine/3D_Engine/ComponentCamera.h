#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"


class ComponentCamera : public Component
{

public:

	ComponentCamera(GameObject* embedded_game_object);
	~ComponentCamera();

	float GetNearPlaneDist() const;
	float GetFarPlaneDist() const;
	float GetFOV() const;
	float GetAspectRatio() const;

	void SetNearPlaneDist(float dist);
	void SetFarPlaneDist(float dist);
	void SetFOV(float fov);
	void SetAspectRatio(float aspect_ratio);

	void Look(const float3& position);

	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();

public:

	Frustum frustum;
	float4 background_color = float4::zero;

	const GameObject* looking_at = nullptr;

	bool frustum_culling = false;

	bool projection_changed = false;

};

#endif // __COMPONENT_CAMERA_H__