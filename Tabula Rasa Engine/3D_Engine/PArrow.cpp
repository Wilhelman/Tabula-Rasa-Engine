#include "PArrow.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PArrow::PArrow() : trPrimitive(), origin(0, 0, 0), destination(1, 1, 1), color(0.f, 0.f, 0.f, 1.f)
{
	type = PrimitiveTypes::Primitive_Arrow;

	float vertices_array[15] = {
		origin.x, origin.y, origin.z,
		destination.x, destination.y, destination.z,
		destination.x + 0.5f, destination.y, destination.z,
		destination.x, destination.y + 1.0f, destination.z,
		destination.x - 0.5f, destination.y, destination.z,
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 15, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[5] = {
		0, 1, 2, 3, 4
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 5, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PArrow::PArrow(math::vec destination, math::float4 color) : trPrimitive(), origin(0.f, 0.f, 0.f), destination(destination.x, destination.y, destination.z), color(color.x, color.y, color.z, color.w)
{
	type = PrimitiveTypes::Primitive_Arrow;

	float vertices_array[18] = {
		origin.x, origin.y, origin.z,
		destination.x, destination.y, destination.z,
		destination.x + 0.1f, destination.y - 0.2f, destination.z,
		destination.x, destination.y, destination.z,
		destination.x, destination.y, destination.z,
		destination.x - 0.1f, destination.y - 0.2f, destination.z,
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, vertices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[6] = {
		0, 1, 2, 3, 4, 5
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PArrow::InnerRender() const
{
	glLineWidth(5.0f);
	//glColor4f(color.x, color.y, color.z, color.w);
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_LINES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}