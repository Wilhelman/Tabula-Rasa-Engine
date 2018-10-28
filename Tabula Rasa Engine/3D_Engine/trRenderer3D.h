#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trModule.h"
#include "trDefs.h"

#include "Light.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#define MAX_LIGHTS 8

struct Mesh //todo take this to component
{
	math::vec position;
	math::vec scale;
	math::vec rotation;
	
	std::string name;
	std::string path;

	uint index_buffer = 0u;
	uint index_size = 0u;
	uint* indices = nullptr;

	uint face_size = 0u;

	uint vertex_buffer = 0u;
	uint vertex_size = 0u;
	float* vertices = nullptr;

	uint uv_buffer = 0u;
	uint size_uv = 0u;
	float* uvs = nullptr;

	math::float4* ambient_color = nullptr;

	math::AABB* bounding_box = nullptr;

	~Mesh();
};

struct Texture { // just to send info to inspector easily: to remove when gameobject enter
	uint id = 0u;
	uint width = 0u;
	uint height = 0u;
	std::string path;
};

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
	void SetTexture(Texture* texture);

	const uint GetTextureID() const;
	const uint GetMeshesSize() const;

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

	std::vector<Mesh*> meshes;
	Texture* texture = nullptr;

	math::AABB* last_mesh_bounding_box = nullptr;

};
#endif