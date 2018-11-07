#include "DebugDraw.h"
#include "trApp.h"
#include "trRenderer3D.h"

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

void DebugDraw(const AABB & aabb, Color color, const float4x4 & transform)
{
	static float3 corners[8];
	aabb.GetCornerPoints(corners);

	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());
	
	glColor4f(color.r, color.g, color.b,1.f);

	glDisable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[3]);

	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[2]);
	glVertex3fv((GLfloat*)&corners[6]);

	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[7]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[1]);
	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[3]);
	glVertex3fv((GLfloat*)&corners[7]);
	glVertex3fv((GLfloat*)&corners[6]);
	glVertex3fv((GLfloat*)&corners[2]);

	glVertex3fv((GLfloat*)&corners[0]);
	glVertex3fv((GLfloat*)&corners[4]);
	glVertex3fv((GLfloat*)&corners[5]);
	glVertex3fv((GLfloat*)&corners[1]);

	glEnd();

	if (!App->render->wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (App->render->cull_face)
		glEnable(GL_CULL_FACE);

	glColor4f(1.f, 1.f, 1.f,1.f);

	glPopMatrix();
}

void DebugDraw(const Frustum & frustum, Color color, const float4x4& transform)
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor4f(color.r, color.g, color.b, 1.f);
	glLineWidth(5.f);

	glBegin(GL_LINES);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(frustum.Edge(i).a.x, frustum.Edge(i).a.y, frustum.Edge(i).a.z);
		glVertex3f(frustum.Edge(i).b.x, frustum.Edge(i).b.y, frustum.Edge(i).b.z);
	}

	glEnd();

	glLineWidth(1.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);

	glPopMatrix();
}

void DebugDraw(const LineSegment & line_segment, Color color, const float4x4 & transform)
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	glColor4f(color.r, color.g, color.b, 1.f);
	glLineWidth(5.f);

	glBegin(GL_LINES);

	glVertex3f((GLfloat)line_segment.a.x, (GLfloat)line_segment.a.y, (GLfloat)line_segment.a.z);
	glVertex3f((GLfloat)line_segment.b.x, (GLfloat)line_segment.b.y, (GLfloat)line_segment.b.z);

	glEnd();

	glPopMatrix();
}