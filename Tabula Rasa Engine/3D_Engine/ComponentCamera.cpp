#include "ComponentCamera.h"
#include "MathGeoLib/Geometry/Frustum.h"

#include "GameObject.h"

#include "trApp.h"
#include "trEditor.h"
#include "trWindow.h"

#define ASPECT_RATIO 1.3f

ComponentCamera::ComponentCamera(GameObject * embedded_game_object):
	Component(embedded_game_object, Component::component_type::COMPONENT_CAMERA)
{
	frustum.type = FrustumType::PerspectiveFrustum;

	frustum.pos = float3(-2.f, 3.f, -10.f);
	frustum.front = float3::unitZ;
	frustum.up = float3::unitY;

	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 1000.0f;
	frustum.verticalFov = math::DegToRad(60.0f);

	SetAspectRatio(ASPECT_RATIO);

	projection_needs_update = true;
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::PreUpdate(float dt)
{
	float3 pos = float3::zero;
	float3 sca = float3::zero;
	Quat rot = Quat::identity;
	embedded_go->GetTransform()->GetLocalPosition(&pos, &sca, &rot);
	frustum.pos = pos;
	return true;
}

void ComponentCamera::LookAt(const float3 &spot)
{
	float3 frustum_dir = spot - frustum.pos;
	frustum_dir = frustum_dir.Normalized();

	float3x3 look_mat = float3x3::LookAt(frustum.front, frustum_dir, frustum.up, float3::unitY);

	frustum.front = look_mat.MulDir(frustum.front).Normalized();
	frustum.up = look_mat.MulDir(frustum.up).Normalized();
}

// -----------------------------------------------------------------
void ComponentCamera::SetAspectRatio(float new_aspect_ratio)
{
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.f) * new_aspect_ratio);
	projection_needs_update = true;
}

bool ComponentCamera::FrustumContainsAaBox(const AABB & ref_box)
{
	float3 aabb_corners[8];
	int total_in = 0;
	ref_box.GetCornerPoints(aabb_corners);

	for (int p = 0; p < 6; ++p) { // Planes of the frustum
		int corners_outside = 8;
		int point_in = 1;
		
		for (int i = 0; i < 8; ++i) { // each corner of the AABB

			if (frustum.GetPlane(p).IsOnPositiveSide(aabb_corners[i])) {
				point_in = 0;
				--corners_outside;
			}
		}

		if(corners_outside == 0) // totally in
			return false;

		total_in += point_in;
	}
	//if (iTotalIn == 6)// TODO: maybe is usefull ...
		//return true;

	return true;
}

// -----------------------------------------------------------------
float* ComponentCamera::GetViewMatrix()
{
	gl_view_matrix = frustum.ViewMatrix();

	return gl_view_matrix.Transposed().ptr();
}

float * ComponentCamera::GetProjectionMatrix()
{
	gl_projection_matrix = frustum.ProjectionMatrix().Transposed();

	return gl_projection_matrix.ptr();
}

void ComponentCamera::FocusOnSelectedGO()
{
	GameObject* selected = App->editor->GetSelected();
	if (selected) {

		selected->RecalculateBoundingBox(); // TODO: bad feelings ...

		float3 center_bbox(selected->bounding_box.Centroid());
		float3 move_dir = (frustum.pos - center_bbox).Normalized();

		float radius = selected->bounding_box.MinimalEnclosingSphere().r;
		double fov = DEG_TO_RAD(60.0f);
		double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

		float3 final_pos = center_bbox + move_dir * cam_distance;
		frustum.pos = float3(final_pos.x, final_pos.y, final_pos.z);

		if (frustum.pos.y < 0.f)
			frustum.pos.y *= -1.f;

		LookAt(float3(center_bbox.x, center_bbox.y, center_bbox.z));
	}
	else {
		frustum.pos.Set(3.f, 3.f, 3.f);
		LookAt(float3(0.f, 0.f, 0.f));
	}
}