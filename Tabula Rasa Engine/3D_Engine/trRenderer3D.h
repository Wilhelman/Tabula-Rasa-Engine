#ifndef __trRENDERER3D_H__
#define __trRENDERER3D_H__

#include "trModule.h"
#include "trDefs.h"

#include "Light.h"

#include "Glew\include\GL\glew.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#define MAX_LIGHTS 8

struct Mesh
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
	void SwitchWireframeMode(bool toggle); // todo this is bad
	void SwitchDepthMode(bool toggle);
	void SwitchFaceCulling(bool toggle);
	void SwitchLighting(bool toggle);
	void SwitchColorMaterial(bool toggle);
	void SwitchTexture2D(bool toggle);

	void GenerateBufferForMesh(Mesh* mesh);

	void SetTextureID(const uint texture);
	const uint GetTextureID()const;

	// Cleans all the meshes in the scene (not the primitives);
	void ClearScene();

	void Draw();

	math::float4x4 Perspective(float fovy, float aspect, float n, float f) const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float4x4 ProjectionMatrix;

	bool show_mesh_vertices = false; // todo ...
	bool show_mesh_vertices_normals = false;
	bool show_mesh_faces_normals = false;

private:

	std::vector<Mesh*> meshes;

	math::AABB* last_mesh_bounding_box = nullptr;

	uint texture_id = 0u;

};
#endif