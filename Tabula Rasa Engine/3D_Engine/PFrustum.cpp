#include "PFrustum.h"

#include "Glew\include\GL\glew.h"
#include "trDefs.h"

PFrustum::PFrustum() : trPrimitive()
{
	position = math::vec(-3.f, 3.f,-3.f);	
	type = PrimitiveTypes::Primitive_Frustum;

	float vertices_arr[24] = { //TODO : DO IT WITH GLFLOATS!
		0.0f,-0.5f, 0.5f,
		0.f,0.5f,0.5f,
		2.0f,-1.5f,1.5f,
		2.0f,1.5f,1.5f,
		0.f,-0.5f,-0.5f,
		0.f,0.5f,-0.5f,
		2.0f,-1.5f,-1.5f,
		2.0f, 1.5f,-1.5f,
	};

	vertices_index = 0;
	glGenBuffers(1, (GLuint*) &(vertices_index));
	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24, vertices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	uint indices_arr[36] = {
		0,2,3,
		3,1,0,
		0,1,4,
		1,5,4,
		4,5,6,
		5,7,6,
		3,2,6,
		6,7,3,
		1,3,7,
		5,1,7,
		2,0,4,
		4,6,2
	};

	indices_index = 0;
	glGenBuffers(1, (GLuint*) &(indices_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * 36, indices_arr, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void PFrustum::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);

	glTranslatef(position.x, position.y, position.z);

	static float angle = 0.f;
	glRotatef(angle, 1.f, 1.f, 0.f);

	angle++;
	if (angle >= 360.0f)
		angle = 0.0f;

	glBindBuffer(GL_ARRAY_BUFFER, vertices_index);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indices_index);
	glDrawElements(GL_TRIANGLES, vertices_index, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
}