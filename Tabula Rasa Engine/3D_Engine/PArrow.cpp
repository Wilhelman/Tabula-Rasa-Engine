#include "PArrow.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PArrow::PArrow()
{
	origin = vec(0.f, 0.f, 0.f);
	destination = vec(0.f, 0.f, 0.f);
	color = float4(0.f, 0.f, 0.f, 0.f);
}

PArrow::PArrow(math::vec origin, math::vec direction, math::float4 color) : trPrimitive(), origin(origin), destination(direction), color(color)
{
	type = PrimitiveTypes::Primitive_Arrow;

	float vertices_array[6] = {
		origin.x, origin.y, origin.z,
		direction.x, direction.y, direction.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[2] = {
		0, 1
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 2, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PArrow::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glLineWidth(3.5f);
	glColor4f(color.x, color.y, color.z, color.w);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_LINES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glLineWidth(1.0f);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glDisableClientState(GL_VERTEX_ARRAY);
}