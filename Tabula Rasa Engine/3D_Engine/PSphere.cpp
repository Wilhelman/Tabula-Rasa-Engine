#include "PSphere.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PSphere::PSphere() : trPrimitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

PSphere::PSphere(math::vec position, float radius) : trPrimitive(), radius(radius), position(position)
{
	type = PrimitiveTypes::Primitive_Sphere;

	float angle = 0.f;

	float horizontal_array[60];
	uint i = 0u;

	while (i < 30)
	{
		horizontal_array[i] = math::Cos(angle * DEGTORAD * radius);
		horizontal_array[++i] = 0;
		horizontal_array[++i] = math::Sin(angle * DEGTORAD * radius);

		i++;
		angle += 36.0f;
	}

	angle = 0.f;

	while (i < 60)
	{
		horizontal_array[i] = 0;
		horizontal_array[++i] = math::Cos(angle * DEGTORAD * radius);
		horizontal_array[++i] = math::Sin(angle * DEGTORAD * radius);

		i++;
		angle += 36.0f;
	}

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 60, horizontal_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	uint indices_array[40] = {
		0, 1, 1,2,2,3,3,4,4,5,5,6,6,7,7,8,8,9,9,0,
		10,11,11,12,12,13,13,14,14,15,15,16,16,17,17,18,18,19,19,10
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 40, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void PSphere::InnerRender() const
{
	glLineWidth(5.0f);
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);

	static float angle = 0.f;
	glRotatef(angle, 1.f, 1.f, 0.f);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_LINES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glLineWidth(1.0f);

	glDisableClientState(GL_VERTEX_ARRAY);
}