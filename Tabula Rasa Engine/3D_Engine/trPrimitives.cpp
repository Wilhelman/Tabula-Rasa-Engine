#include "trDefs.h"
#include "trPrimitives.h"
#include "Glew\include\GL\glew.h"

// ------------------------------------------------------------
trPrimitive::trPrimitive() : transform(math::float4x4::identity), color(White), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes trPrimitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void trPrimitive::Render() const
{
	InnerRender();
}

// ------------------------------------------------------------
void trPrimitive::SetPos(float x, float y, float z)
{
	transform = float4x4::Translate(x, y, z).ToFloat4x4() * transform;
}

// ------------------------------------------------------------
void trPrimitive::SetRotation(float angle, const math::vec &u)
{
	transform = float4x4::RotateAxisAngle(u, angle) * transform;
}

// ------------------------------------------------------------
void trPrimitive::Scale(float x, float y, float z)
{
	transform = float4x4::Scale(x, y, z).ToFloat4x4() * transform;
}