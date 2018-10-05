#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trModule.h"
#include "trDefs.h"

#include "trFileLoader.h"

#include "Light.h"
#include "trPrimitives.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#define MAX_LIGHTS 8

class PArrow;
class PPoint;

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

	bool IsWireframeModeEnabled();
	void SwitchWireframeMode(bool toggle);
	void SwitchDepthMode(bool toggle);
	void SwitchFaceCulling(bool toggle);
	void SwitchLighting(bool toggle);
	void SwitchColorMaterial(bool toggle);
	void SwitchTexture2D(bool toggle);

	void GenerateBufferForMesh(Mesh* mesh);

	void Draw();

	math::float4x4 Perspective(float fovy, float aspect, float n, float f) const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool			vsync_state = false;

private:
	uint mesh_buffer_vertex = 0u;
	uint mesh_buffer_index = 0u;

	std::vector<PArrow> normals_vec;
	std::vector<PPoint> vertex_vec;

};
#endif