#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trModule.h"
#include "Globals.h"
#include "Light.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#define MAX_LIGHTS 8

class trRenderer3D : public trModule
{
public:
	trRenderer3D();
	~trRenderer3D();

	bool Awake(pugi::xml_node&);
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool			vsync_state = false;
};
#endif