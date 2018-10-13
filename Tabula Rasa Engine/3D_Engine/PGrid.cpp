#include "PGrid.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

#define GRID_SIZE 3.f

PGrid::PGrid() : trPrimitive()
{
	type = PrimitiveTypes::Primitive_Grid;
	gizmo = new PGizmo(vec(0.f, 0.f, 0.f));

	//todo do it nicer
	float vertices_arr[72] = { //TODO : DO IT WITH GLFLOATS!
		//X
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
		//Z
		-2.f, 0.f, -3.f,
		-2.f, 0.f, 3.f,
		-1.f, 0.f, -3.f,
		-1.f, 0.f, 3.f,
		0.f, 0.f, -3.f,
		0.f, 0.f, 3.f,
		1.f, 0.f, -3.f,
		1.f, 0.f, 3.f,
		2.f, 0.f, -3.f,
		2.f, 0.f, 3.f
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 72, vertices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	uint indices_arr[28] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0,
		12, 1, 13 , 14, 15, 16, 17 , 18, 19, 20 , 21, 22, 23
	};

	num_index = 28;

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 28, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PGrid::~PGrid()
{
	delete gizmo;
}

void PGrid::InnerRender() const
{
	if (axis)
		gizmo->Render();

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