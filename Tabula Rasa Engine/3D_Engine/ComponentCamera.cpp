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

	default_aabb = AABB(float3(-1.f,-1.f,-1.f),float3(1.f,1.f,1.f));

	projection_needs_update = true;
}

ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Save(JSON_Object* component_obj) const
{
	JSON_Value* component_value = json_value_init_array();
	JSON_Array* translation_array = json_value_get_array(component_value);
	json_object_set_value(component_obj, "F_Position", component_value);
	json_array_append_number(translation_array, frustum.pos.x);
	json_array_append_number(translation_array, frustum.pos.y);
	json_array_append_number(translation_array, frustum.pos.z);

	json_object_set_boolean(component_obj, "F_Culling", frustum_culling);
	json_object_set_number(component_obj, "F_Near", frustum.nearPlaneDistance);
	json_object_set_number(component_obj, "F_Far", frustum.farPlaneDistance);
	json_object_set_number(component_obj, "F_HorizontalFov", frustum.horizontalFov);
	json_object_set_number(component_obj, "F_VerticalFov", frustum.verticalFov);

	return true;
}

bool ComponentCamera::Load(const JSON_Object * component_obj)
{
	// Translation
	JSON_Array* array = json_object_get_array(component_obj, "F_Position");
	JSON_Value* value = json_array_get_value(array, 0);
	frustum.pos.x = json_value_get_number(value);
	value = json_array_get_value(array, 1);
	frustum.pos.y = json_value_get_number(value);
	value = json_array_get_value(array, 2);
	frustum.pos.z = json_value_get_number(value);

	value = json_object_get_value(component_obj, "F_Culling");
	frustum_culling = json_value_get_boolean(value);
	value = json_object_get_value(component_obj, "F_Near");
	frustum.nearPlaneDistance = json_value_get_number(value);
	value = json_object_get_value(component_obj, "F_Far");
	frustum.farPlaneDistance = json_value_get_number(value);
	value = json_object_get_value(component_obj, "F_HorizontalFov");
	frustum.horizontalFov = json_value_get_number(value);
	value = json_object_get_value(component_obj, "F_VerticalFov");
	frustum.verticalFov = json_value_get_number(value);

	return true;
}

bool ComponentCamera::PreUpdate(float dt)
{
	float3 pos = float3::zero;
	float3 sca = float3::zero;
	Quat rot = Quat::identity;
	embedded_go->GetTransform()->GetLocalPosition(&pos, &sca, &rot);
	frustum.pos = pos;
	frustum.front = embedded_go->GetTransform()->GetMatrix().WorldZ();
	frustum.up = embedded_go->GetTransform()->GetMatrix().WorldY();
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

void ComponentCamera::SetVerticalFov(float new_v_fov)
{
	float ratio = math::DegToRad(new_v_fov) / frustum.verticalFov;
	frustum.verticalFov = math::DegToRad(new_v_fov);

	frustum.horizontalFov *= ratio;
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
float4x4 ComponentCamera::GetViewMatrix()
{
	gl_view_matrix = frustum.ViewMatrix();

	return gl_view_matrix.Transposed();
}

float4x4 ComponentCamera::GetProjectionMatrix()
{
	gl_projection_matrix = frustum.ProjectionMatrix();

	return gl_projection_matrix.Transposed();
}

void ComponentCamera::FocusOnAABB(AABB& aabb)
{
	GameObject* selected = App->editor->GetSelected();
	if (selected) {
		selected->RecalculateBoundingBox(); // TODO: bad feelings ...
		aabb = selected->bounding_box;
	}

	float3 center_bbox(aabb.Centroid());
	float3 move_dir = (frustum.pos - center_bbox).Normalized();

	float radius = aabb.MinimalEnclosingSphere().r;
	double fov = DEG_TO_RAD(60.0f);
	double cam_distance = Abs(App->window->GetWidth() / App->window->GetHeight() * radius / Sin(fov / 2.f));

	float3 final_pos = center_bbox + move_dir * cam_distance;
	frustum.pos = float3(final_pos.x, final_pos.y, final_pos.z);

	if (frustum.pos.y < 0.f)
		frustum.pos.y *= -1.f;

	LookAt(float3(center_bbox.x, center_bbox.y, center_bbox.z));
}