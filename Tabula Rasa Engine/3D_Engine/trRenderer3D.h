#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trModule.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#define MAX_LIGHTS 8

class trRenderer3D : public trModule
{
public:
	trRenderer3D();
	~trRenderer3D();

	bool Init();
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;
};
#endif