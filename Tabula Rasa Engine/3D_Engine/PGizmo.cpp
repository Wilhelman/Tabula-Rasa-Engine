#include "PGizmo.h"
#include "PArrow.h"

PGizmo::PGizmo(math::vec origin) : trPrimitive()
{
	type = PrimitiveTypes::Primitive_Gizmo;

	this->origin = origin;
	w_arrow = new PArrow(origin, origin + vec(1.f, 0.f, 0.f), float4(1.f, 0.f, 0.f, 1.f));
	y_arrow = new PArrow(origin, origin + vec(0.f, 1.f, 0.f), float4(0.f, 1.f, 0.f, 1.f));
	z_arrow = new PArrow(origin, origin + vec(0.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f));
	x_arrow = new PArrow(origin, origin + vec(1.f, 0.f, 0.f), float4(1.f, 0.f, 0.f, 1.f));
}

PGizmo::~PGizmo()
{
	delete x_arrow;
	delete y_arrow;
	delete z_arrow;
	delete w_arrow;
}

void PGizmo::InnerRender() const
{
	// Render each gizmo axis
	x_arrow->Render();
	y_arrow->Render();
	z_arrow->Render();
}