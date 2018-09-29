#ifndef __tr_PRIMITIVE_H__
#define __tr_PRIMITIVE_H__

#include "Color.h"

#include "MathGeoLib\MathGeoLib.h"


enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class trPrimitive
{
public:

	trPrimitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const math::vec &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:

	Color color;
	math::float4x4 transform;
	bool axis, wire;

protected:
	PrimitiveTypes type;
};

// ============================================
class PCube : public trPrimitive
{
public:
	PCube();
	PCube(float sizeX, float sizeY, float sizeZ);
	void InnerRender() const;
public:
	math::vec size;
};

// ============================================
class PSphere : public trPrimitive
{
public:
	PSphere();
	PSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class PCylinder : public trPrimitive
{
public:
	PCylinder();
	PCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class PLine : public trPrimitive
{
public:
	PLine();
	PLine(float x, float y, float z);
	void InnerRender() const;
public:
	math::vec origin;
	math::vec destination;
};

// ============================================
class PPlane : public trPrimitive
{
public:
	PPlane();
	PPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::vec normal;
	float constant;
};
#endif // __PRIMITIVE_H__