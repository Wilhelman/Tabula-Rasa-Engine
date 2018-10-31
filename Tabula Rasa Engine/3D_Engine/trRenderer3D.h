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


	bool IsWireframeModeEnabled();
	void SwitchWireframeMode(bool toggle);
	void SwitchDepthMode(bool toggle);
	void SwitchFaceCulling(bool toggle);
	void SwitchLighting(bool toggle);
	void SwitchColorMaterial(bool toggle);
	void SwitchTexture2D(bool toggle);
	void SwitchVsync(bool toggle);

	void GenerateBufferForMesh(Mesh* mesh);

	const uint GetMeshesSize() const;

	void CollectGameObjectWithMesh(GameObject* game_object);

	// Cleans all the meshes in the scene (not the primitives);
	void ClearScene();

	void Draw();

	void DrawZBuffer();

	math::float4x4 Perspective(float fovy, float aspect, float n, float f) const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float4x4 ProjectionMatrix;

	// Config settings
	bool wireframe = false;
	bool depth_test = true;
	bool cull_face = true;
	bool lighting = true;
	bool color_material = true;
	bool texture_2D = true;
	bool z_buffer = false;
	bool vsync_toogle = false;

private:

	std::vector<GameObject*> drawable_go;

	math::AABB* last_mesh_bounding_box = nullptr;

};
#endif