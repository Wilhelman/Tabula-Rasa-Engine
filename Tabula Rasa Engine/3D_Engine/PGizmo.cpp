#include "PGizmo.h"

PGizmo::PGizmo(math::vec origin) : trPrimitive()
{
	type = PrimitiveTypes::Primitive_Gizmo;

	this->origin = origin;
	x_arrow = PArrow(origin, origin + vec(1.f, 0.f, 0.f), float4(1.f, 0.f, 0.f, 1.f));
	y_arrow = PArrow(origin, origin + vec(0.f, 1.f, 0.f), float4(0.f, 1.f, 0.f, 1.f));
	z_arrow = PArrow(origin, origin + vec(0.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f));
	x_arrow = PArrow(origin, origin + vec(1.f, 0.f, 0.f), float4(1.f, 0.f, 0.f, 1.f));
}

void PGizmo::InnerRender() const
{
	// Render each gizmo axis
	x_arrow.Render();
	y_arrow.Render();
	z_arrow.Render();
}