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
class ImageTexture;

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

	// Generates primitives (points and lines) to draw vertices and normals when needed
	void GenerateMeshDebug(Mesh* mesh);

	void SetTexture(ImageTexture* texture);

	// Cleans all the meshes in the scene (not the primitives);
	void ClearScene();

	void Draw();

	math::float4x4 Perspective(float fovy, float aspect, float n, float f) const;

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool			vsync_state = false;

	bool show_mesh_vertices = false;
	bool show_mesh_vertices_normals = false;
	bool show_mesh_faces_normals = false;

private:

	std::vector<PArrow> vertex_normals_vec;
	std::vector<PArrow> face_normals_vec;
	std::vector<PPoint> point_face_normals_vec;
	std::vector<PPoint> vertex_vec;

	std::vector<Mesh*> meshes;
	uint last_texture = 0u;

};
#endif