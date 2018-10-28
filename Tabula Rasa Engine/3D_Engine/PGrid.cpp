#include "PGrid.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

#define GRID_SIZE 3.f

PGrid::PGrid() : trPrimitive()
{
	type = PrimitiveTypes::Primitive_Grid;
	gizmo = new PGizmo(vec(0.f, 0.f, 0.f));

	float vertex_array[96];
	uint offset = 0; 
	float half_grid = 4.0f;
	uint counter = 0;

	for (int i = 0; i < 9; i++)
	{
		vertex_array[counter] = -half_grid;
		vertex_array[++counter] = 0.0f;
		vertex_array[++counter] = half_grid - offset;

		vertex_array[++counter] = half_grid;
		vertex_array[++counter] = 0.0f;
		vertex_array[++counter] = half_grid - offset;

		counter++;
		offset++;
	}

	offset = 1.0f;

	for (int i = 0; i < 7; i++)
	{
		vertex_array[counter] = -half_grid + offset;
		vertex_array[++counter] = 0.0f;
		vertex_array[++counter] = half_grid;

		vertex_array[++counter] = -half_grid + offset;
		vertex_array[++counter] = 0.0f;
		vertex_array[++counter] = -half_grid;

		counter++;
		offset++;
	}


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
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 96, vertex_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_array[32];
	uint helper = 0;

	for (uint i = 0; i < 32; i++)
	{
		indices_array[i] = i + helper;
		helper = 0;

		if (i == 18)
			indices_array[i] = indices_array[0];
		else if (i == 19)
			indices_array[i] = indices_array[16];
		else if (i == 30)
			indices_array[i] = indices_array[1];
		else if (i == 31)
			indices_array[i] = indices_array[17];

		if (i > 18)
			helper++;
	}
		

	uint indices_arr[28] = {
		0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 0,
		12, 1, 13 , 14, 15, 16, 17 , 18, 19, 20 , 21, 22, 23
	};

	num_index = 32;

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 32, indices_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

PGrid::~PGrid()
{
	delete gizmo;
	glDeleteBuffers(1, (GLuint*)&vertices_index);
	glDeleteBuffers(1, (GLuint*)&indices_index);
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