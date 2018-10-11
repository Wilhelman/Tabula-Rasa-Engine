#include "PGrid.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

#define GRID_SIZE 3.f

PGrid::PGrid() : trPrimitive()
{

	type = PrimitiveTypes::Primitive_Grid;

	float vertices_arr[42] = { //TODO : DO IT WITH GLFLOATS!
		-3.f, 0.f, -3.f,
		3.f, 0.f, -3.f,

		-3.f, 0.f, -2.f,
		3.f, 0.f, -2.f,

		-3.f, 0.f, -1.f,
		3.f, 0.f, -1.f,

		-3.f, 0.f, 0.f,
		3.f, 0.f, 0.f,

		-3.f, 0.f, 1.f,
		3.f, 0.f, 1.f,

		-3.f, 0.f, 2.f,
		3.f, 0.f, 2.f,

		-3.f, 0.f, 3.f,
		3.f, 0.f, 3.f,
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 42, vertices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	uint indices_arr[14] = {
		0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39
	};

	num_index = 14;

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 14, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PGrid::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glColor3f(1.f, 1.f, 1.f);
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_LINES, num_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}