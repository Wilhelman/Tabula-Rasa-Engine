#include "PPoint.h"

PPoint::PPoint(math::vec pos, math::float4 color) : trPrimitive(), pos(pos), color(color)
{
	type = PrimitiveTypes::Primitive_Point;


	float vertices_array[3] = {
		pos.x, pos.y, pos.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[1] = { 0 };

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint), indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PPoint::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glPointSize(7.0f);
	glColor4f(color.x, color.y, color.z, color.w);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_POINTS, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glPointSize(1.0f);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glDisableClientState(GL_VERTEX_ARRAY);
}
