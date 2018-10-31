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

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 96, vertex_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint count_index = 0;
	uint helper_count = 0;

	while (count_index < 36)
	{
		indices_array[count_index] = helper_count;

		if (count_index == 18)
			indices_array[count_index] = indices_array[0];
		else if (count_index == 19)
		{
			indices_array[count_index] = indices_array[16];
			helper_count = 17;
		}
		else if (count_index == 34)
			indices_array[count_index] = indices_array[1];
		else if (count_index == 35)
			indices_array[count_index] = indices_array[17];
		
		helper_count++;
		count_index++;
	}

	num_index = 36;

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices_array, GL_STATIC_DRAW);
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