#include "PSphere.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PSphere::PSphere() : trPrimitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

PSphere::PSphere(float radius) : trPrimitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void PSphere::InnerRender() const
{
	int stacks = 10;
	int slices = 10;

	int i, j;
	for (j = 0; j < stacks; j++) {
		double latitude1 = (PI / stacks) * j - PI / 2;
		double latitude2 = (PI / stacks) * (j + 1) - PI / 2;
		double sinLat1 = sin(latitude1);
		double cosLat1 = cos(latitude1);
		double sinLat2 = sin(latitude2);
		double cosLat2 = cos(latitude2);
		glBegin(GL_QUAD_STRIP);
		for (i = 0; i <= slices; i++) {
			double longitude = (2 * PI / slices) * i;
			double sinLong = sin(longitude);
			double cosLong = cos(longitude);
			double x1 = cosLong * cosLat1;
			double y1 = sinLong * cosLat1;
			double z1 = sinLat1;
			double x2 = cosLong * cosLat2;
			double y2 = sinLong * cosLat2;
			double z2 = sinLat2;
			glNormal3d(x2, y2, z2);
			glVertex3d(radius*x2, radius*y2, radius*z2);
			glNormal3d(x1, y1, z1);
			glVertex3d(radius*x1, radius*y1, radius*z1);
		}
		glEnd();
	}
}