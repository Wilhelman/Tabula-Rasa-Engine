#ifndef __COMPONENT_CAMERA_H__
#define __COMPONENT_CAMERA_H__

#include "Component.h"
#include "MathGeoLib/MathGeoLib.h"


class ComponentCamera : public Component
{

public:

	ComponentCamera(GameObject* embedded_game_object);
	~ComponentCamera(); 



private:

	Frustum frustum;
	float4 background_color = float4::zero;
	bool frustum_culling = false;
	bool projection_changed = false;
	const GameObject* looking_at = nullptr;
	uint looking_at_uid = 0;

};

#endif // __COMPONENT_CAMERA_H__