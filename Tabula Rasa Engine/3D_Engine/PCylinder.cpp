#include "PCylinder.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PCylinder::PCylinder() : trPrimitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

PCylinder::PCylinder(float radius, float height) : trPrimitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;

	float angle = 0.0f;
	int i = 0;

	float array_top_circle[30];
	float array_bottom_circle[30];
	float final_array[66];

	math::vec center_top(0.0f, height, 0.f);
	math::vec center_bottom(0.0f, 0.0f, 0.f);

	while (i < 30)
	{
		final_array[i] = math::Cos(angle * DEGTORAD);
		final_array[++i] = height;
		final_array[++i] = math::Sin(angle * DEGTORAD);

		i++;
		angle += 36.0f;
	}

	i = 29;
	angle = 0.0f;

	while (i < 60)
	{
		final_array[i] = math::Cos(angle * DEGTORAD);
		final_array[++i] = 0.0f;
		final_array[++i] = math::Sin(angle * DEGTORAD);

		i++;
		angle += 36.0f;
	}

	final_array[60] = center_top.x;
	final_array[61] = center_top.y;
	final_array[62] = center_top.z;
	final_array[63] = center_bottom.x;
	final_array[64] = center_bottom.y;
	final_array[65] = center_bottom.z;


	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 66, final_array, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_arr[63] = {
		// side
		0, 11, 9
	
		
		/*
		0, 11, 1,
		1, 11, 12,
		1, 12, 2
		
		2, 13, 14,
		2, 14, 3,
		3, 14, 15,
		3, 15, 4,
		4, 15, 16,
		4, 16, 5,
		5, 16, 17,
		5, 17, 6,
		6, 17, 16,
		6, 18, 7,
		7, 18, 17,
		7, 19, 8,
		8, 19, 18,
		8, 20, 9,
		9, 20, 11,
		9, 11, 0,

		// top
		/*0, 21, 9 */

	
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 63, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

void PCylinder::InnerRender() const
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