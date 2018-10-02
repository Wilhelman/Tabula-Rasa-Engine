#include "PPlane.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PPlane::PPlane() : trPrimitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;

	//test
	math::vec length(10.f, 0.f, 10.f);

	math::vec offset_size = length / 2.f;

	float vertices_arr[12] = { //TODO : DO IT WITH GLFLOATS!
		offset_size.x, offset_size.y, offset_size.z,
		offset_size.x, offset_size.y, -offset_size.z,
		-offset_size.x, offset_size.y, -offset_size.z,
		-offset_size.x, offset_size.y, offset_size.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices_arr, GL_STATIC_DRAW);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	uint indices_arr[6] = {
		3,0,1,
		1,2,3
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PPlane::PPlane(math::vec normal, float d) : trPrimitive(), normal(normal), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;

	//test
	math::vec length(1.f, 1.f, 1.f);

	math::vec offset_size = length / 2.f;

	float vertices_arr[12] = { //TODO : DO IT WITH GLFLOATS!
		offset_size.x, offset_size.y, offset_size.z,
		offset_size.x, offset_size.y, -offset_size.z,
		-offset_size.x, offset_size.y, -offset_size.z,
		-offset_size.x, offset_size.y, offset_size.z
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 12, vertices_arr, GL_STATIC_DRAW);	glBindBuffer(GL_ARRAY_BUFFER, 0);
	uint indices_arr[6] = {
		3,0,1,
		1,2,3
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 6, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

void PPlane::InnerRender() const
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