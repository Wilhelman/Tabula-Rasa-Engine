#include "PCube.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PCube::PCube() : trPrimitive(), size(1.f,1.f,1.f)
{
	math::vec offset_size = size / 2.f;

	type = PrimitiveTypes::Primitive_Cube;

	float vertices_arr[24] = {
	-offset_size.x, -offset_size.y, offset_size.z,
	offset_size.x, -offset_size.y, offset_size.z,
	offset_size.x, offset_size.y, offset_size.z,
	-offset_size.x, offset_size.y, offset_size.z,
	offset_size.x, -offset_size.y, -offset_size.z,
	offset_size.x, offset_size.y, -offset_size.z,
	-offset_size.x, -offset_size.y, -offset_size.z,
	-offset_size.x, offset_size.y, -offset_size.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices_arr, GL_STATIC_DRAW);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	uint indices_arr[36] = {
	0,1,2,
	3,0,2,
	2,1,4,
	5,2,4,
	3,2,7,
	2,5,7,
	5,4,6,
	6,7,5,
	3,6,0,
	7,6,3,
	1,0,6,
	1,6,4
	};

	indices_index  = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint)* 36, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PCube::PCube(math::vec position, math::vec size) :size(size), position(position)
{
	math::vec offset_size = size / 2.f;

	type = PrimitiveTypes::Primitive_Cube;

	float vertices_arr[24] = {
		-offset_size.x, -offset_size.y, offset_size.z,
		offset_size.x, -offset_size.y, offset_size.z,
		offset_size.x, offset_size.y, offset_size.z,
		-offset_size.x, offset_size.y, offset_size.z,
		offset_size.x, -offset_size.y, -offset_size.z,
		offset_size.x, offset_size.y, -offset_size.z,
		-offset_size.x, -offset_size.y, -offset_size.z,
		-offset_size.x, offset_size.y, -offset_size.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices_arr, GL_STATIC_DRAW);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	uint indices_arr[36] = {
		0,1,2,
		3,0,2,
		2,1,4,
		5,2,4,
		3,2,7,
		2,5,7,
		5,4,6,
		6,7,5,
		3,6,0,
		7,6,3,
		1,0,6,
		1,6,4
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PCube::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_TRIANGLES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}