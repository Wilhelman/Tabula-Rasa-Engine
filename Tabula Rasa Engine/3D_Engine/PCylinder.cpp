#include "PCylinder.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PCylinder::PCylinder() : trPrimitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

PCylinder::PCylinder(float radius, float height) : trPrimitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void PCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);

	for (int i = 360; i >= 0; i -= (360 / n))
	{
		float a = (float)i * PI / 180.f; // degrees to radians
		glVertex3f(-height*0.5f, radius * cosf(a), radius * sinf(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for (int i = 0; i <= 360; i += (360 / n))
	{
		float a = (float)i * PI / 180.f; // degrees to radians
		glVertex3f(height * 0.5f, radius * cosf(a), radius * sinf(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for (int i = 0; i < 480; i += (360 / n))
	{
		float a = (float)i * PI / 180.f; // degrees to radians

		glVertex3f(height*0.5f, radius * cosf(a), radius * sinf(a));
		glVertex3f(-height*0.5f, radius * cosf(a), radius * sinf(a));
	}
	glEnd();
}