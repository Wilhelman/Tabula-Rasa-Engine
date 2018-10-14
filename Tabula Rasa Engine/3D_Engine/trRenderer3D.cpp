#include "trDefs.h"
#include "trWindow.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trMainScene.h"
#include "PArrow.h"
#include "PPoint.h"
#include "trEditor.h"
#include "trTextures.h"
#include "trInput.h"

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")


trRenderer3D::trRenderer3D() : trModule()
{
	name = "Renderer3D";
}

// Destructor
trRenderer3D::~trRenderer3D()
{}

// Called before render is available
bool trRenderer3D::Awake(JSON_Object* config)
{
	TR_LOG("Renderer3D: Creating 3D Renderer context");

	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		TR_LOG("Renderer3D: OpenGL context could not be created!SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	if (config != nullptr) {
		if (json_object_get_boolean(config, "vsync")) {
			if (SDL_GL_SetSwapInterval(1) < 0)
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
			else
				TR_LOG("Renderer3D: vSync ENABLED");
		}
		else {
			if (SDL_GL_SetSwapInterval(0) < 0)
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
			else
				TR_LOG("Renderer3D: vSync DISABLED");
		}
	}

	GLenum err = glewInit();

	TR_LOG("---------- Version info ----------");

	if (err != GLEW_OK)
	{
		TR_LOG("Glew library could not init %s", glewGetErrorString(err));
		ret = false;
	}
	else {
		TR_LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	}


	if (ret == true)
	{

		// get version info

		TR_LOG("Vendor: %s", (char*)glGetString(GL_VENDOR));
		TR_LOG("Renderer: %s", (char*)glGetString(GL_RENDERER));
		TR_LOG("OpenGL version supported %s", (char*)glGetString(GL_VERSION));
		TR_LOG("GLSL: %s", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		TR_LOG("---------- End info ----------");

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			TR_LOG("Renderer3D: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			TR_LOG("Renderer3D: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			TR_LOG("trRenderer3D: Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
		glEnable(GL_TEXTURE_2D);
	}

	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight());

	return ret;
}

// PreUpdate: clear buffer
bool trRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->pos.x, App->camera->pos.y, App->camera->pos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return true;
}

// PostUpdate present buffer to screen
bool trRenderer3D::PostUpdate(float dt)
{
	
	//RENDER GEOMETRY
	App->main_scene->Draw();

	//RENDER DEBUG
	/// not yet

	//RENDER IMPORTED MESH
	if (!meshes.empty())
		this->Draw();

	/*if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT)
	{
		int width = App->window->GetWidth();
		int height = App->window->GetWidth();
		float* data = new float[width * height];
		glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data);

		//depthSample = 2.0 * depthSample - 1.0;
		//float zLinear = 2.0 * zNear * zFar / (zFar + zNear - depthSample * (zFar - zNear));
		
		glDrawPixels(width, height, GL_LUMINANCE, GL_FLOAT, data);
		delete[] data;
	}*/

	//RENDER GUI
	App->editor->Draw();



	//SWAP BUFFERS
	SDL_GL_SwapWindow(App->window->window);
	return true;
}

// Called before quitting
bool trRenderer3D::CleanUp()
{
	TR_LOG("Renderer3D: CleanUp");

	SDL_GL_DeleteContext(context);

	this->ClearScene();

	return true;
}


void trRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ProjectionMatrix = this->Perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf((GLfloat*)ProjectionMatrix.ptr());
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool trRenderer3D::IsWireframeModeEnabled()
{
	GLint polygonMode[2];
	glGetIntegerv(GL_POLYGON_MODE, polygonMode);

	if (polygonMode[0] == GL_LINE && polygonMode[1] == GL_LINE)
		return true;
	else if (polygonMode[0] == GL_FILL && polygonMode[1] == GL_FILL)
		return false;
}

void trRenderer3D::SwitchWireframeMode(bool toggle)
{
	if (toggle)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void trRenderer3D::SwitchDepthMode(bool toggle)
{
	(toggle) ?
		glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
}

void trRenderer3D::SwitchFaceCulling(bool toggle)
{
	(toggle) ?
		glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
}

void trRenderer3D::SwitchLighting(bool toggle)
{
	(toggle) ?
		glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

}

void trRenderer3D::SwitchColorMaterial(bool toggle)
{
	(toggle) ?
		glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
}

void trRenderer3D::SwitchTexture2D(bool toggle)
{
	(toggle) ?
		glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
}

void trRenderer3D::GenerateBufferForMesh(Mesh* mesh)
{
	glGenBuffers(1, (GLuint*) &(mesh->vertex_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->vertex_size, mesh->vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(mesh->uv_buffer));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * mesh->size_uv * sizeof(GLfloat), mesh->uvs, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(mesh->index_buffer));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->index_size, mesh->indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	meshes.push_back(mesh);
	
	App->editor->SetupInspectorWith(mesh);
}

void trRenderer3D::SetTexture(Texture* texture)
{
	this->texture = texture;
	App->editor->SetupInspectorWith(texture);
}

const uint trRenderer3D::GetTextureID() const
{
	uint ret;
	(this->texture != nullptr) ? ret = texture->id : ret = 0u;
	return ret;
}

const uint trRenderer3D::GetMeshesSize() const
{
	return meshes.size();
}

void trRenderer3D::ClearScene()
{
	App->editor->CleanInspectorData();

	std::vector<Mesh*>::iterator it = meshes.begin();
	while (it != meshes.end())
	{
		delete (*it);
		(*it) = nullptr;
		it++;
	}
	meshes.clear();
	texture = nullptr;

}

void trRenderer3D::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	std::vector<Mesh*>::iterator it = meshes.begin();
	while (it != meshes.end())
	{
		Mesh* mesh = (*it);

		if(texture != nullptr)
			glBindTexture(GL_TEXTURE_2D, texture->id);

		if (texture == nullptr || !texture_2D) // If the texture is missing, we set the ambient color of the mesh
			glColor4f(mesh->ambient_color->w, mesh->ambient_color->x, mesh->ambient_color->y, mesh->ambient_color->z);
		
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//texture
		glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_buffer);
		glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
		glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glColor4f(1.f, 1.f, 1.f, 1.f);

		if (texture != nullptr)
			glBindTexture(GL_TEXTURE_2D, 0);

		it++;
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}


math::float4x4 trRenderer3D::Perspective(float fovy, float aspect, float n, float f) const
{
	math::float4x4 Perspective;

	float coty = 1.0f / tan(fovy * math::pi / 360.0f);

	for (uint i = 0; i < 4; i++) {
		for (uint j = 0; j < 4; j++)
			Perspective[i][j] = 0.0f;
	}

	Perspective[0][0] = coty / aspect;
	Perspective[1][1] = coty;
	Perspective[2][2] = (n + f) / (n - f);
	Perspective[2][3] = -1.0f;
	Perspective[3][2] = 2.0f * n * f / (n - f);

	return Perspective;
}

Mesh::~Mesh()
{
	if (indices != nullptr) { delete[] indices; indices = nullptr; }
	if (vertices != nullptr) { delete[] vertices; vertices = nullptr; }
	if (uvs != nullptr) { delete[] uvs; uvs = nullptr; }

	if (bounding_box != nullptr) { delete bounding_box; bounding_box = nullptr; }
	if (ambient_color != nullptr) { delete ambient_color; ambient_color = nullptr; }

	name.clear();

	glDeleteBuffers(1, (GLuint*)&index_buffer);
	glDeleteBuffers(1, (GLuint*)&vertex_buffer);
	glDeleteBuffers(1, (GLuint*)&uv_buffer);
}
