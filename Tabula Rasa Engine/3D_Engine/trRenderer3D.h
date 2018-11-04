#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trApp.h"
#include "trModule.h"
#include "trDefs.h"

#include "Light.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#define MAX_LIGHTS 8

class GameObject;
class Mesh;
class ComponentCamera;

class trRenderer3D : public trModule
{
public:
	trRenderer3D();
	~trRenderer3D();

	bool Awake(JSON_Object* config = nullptr);
	bool PreUpdate(float dt);
	bool PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void UpdateCameraProjection();

	bool IsWireframeModeEnabled();
	void SwitchWireframeMode(bool toggle);
	void SwitchDepthMode(bool toggle);
	void SwitchFaceCulling(bool toggle);
	void SwitchLighting(bool toggle);
	void SwitchColorMaterial(bool toggle);
	void SwitchTexture2D(bool toggle);
	void SwitchVsync(bool toggle);
	void SwitchDebugDraw(bool toggle);

	const uint GetMeshesSize() const;

	void CollectGameObjectWithMesh(GameObject* game_object);

	void Draw();
	void DebugDrawAABB(AABB bounding_box);
	void DrawZBuffer();

	math::float4x4 Perspective(float fovy, float aspect, float n, float f) const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;

	ComponentCamera* active_camera = nullptr;

	// Config settings
	bool wireframe = false;
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2D = true;
	bool z_buffer = false;
	bool vsync_toogle = false;
	bool debug_draw_on = false;

private:

	std::vector<GameObject*> drawable_go;

};
#endif