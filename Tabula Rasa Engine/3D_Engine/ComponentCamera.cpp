#include "ComponentCamera.h"
#include "MathGeoLib/Geometry/Frustum.h"

#define ASPECT_RATIO 1.3f

ComponentCamera::ComponentCamera(GameObject * embedded_game_object):
	Component(embedded_game_object, Component::component_type::COMPONENT_CAMERA)
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.SetPos(float3::zero);
	frustum.SetFront(float3::unitZ);
	frustum.SetUp(float3::unitY);

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.SetVerticalFovAndAspectRatio(DEGTORAD * 60.0f, ASPECT_RATIO);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * ASPECT_RATIO);


	background_color = float4(0.f,0.f,0.f,255.f);

	projection_changed = true;
}

ComponentCamera::~ComponentCamera()
{
}
