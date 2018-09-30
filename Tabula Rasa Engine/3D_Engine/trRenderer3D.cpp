#include "trDefs.h"
#include "trWindow.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "trMainScene.h"

#include "Glew\include\GL\glew.h"

#include "SDL\include\SDL_opengl.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

trRenderer3D::trRenderer3D() : trModule()
{
	name = "Renderer 3D";
}

// Destructor
trRenderer3D::~trRenderer3D()
{}

// Called before render is available
bool trRenderer3D::Awake(pugi::xml_node& config)
{
	App->editor->Log("Renderer3D: Creating 3D Renderer context");

	bool ret = true;

	vsync_state = false;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		vsync_state = true;
		App->editor->Log("Renderer3D: Using vsync");
	}

	//Create context
	context = SDL_GL_CreateContext(App->win->window);
	if (context == NULL)
	{
		App->editor->Log("Renderer3D: OpenGL context could not be created!SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum err = glewInit();

	App->editor->Log("---------- Version info ----------");

	if (err != GLEW_OK)
	{
		App->editor->Log("Glew library could not init", (char*)glewGetErrorString(err));
		ret = false;
	}
	else {
		App->editor->Log("Using Glew ", (char*)glewGetString(GLEW_VERSION));
	}
		

	if (ret == true)
	{

		// get version info
		
		App->editor->Log("Vendor: ", (char*)glGetString(GL_VENDOR));
		App->editor->Log("Renderer: ", (char*)glGetString(GL_RENDERER));
		App->editor->Log("OpenGL version supported ", (char*)glGetString(GL_VERSION));
		App->editor->Log("GLSL: ", (char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
		App->editor->Log("---------- End info ----------");

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->editor->Log("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//TR_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->Log("Renderer3D: Error initializing OpenGL!");
			ret = false;
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//TR_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->Log("Renderer3D: Error initializing OpenGL!");
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
			//TR_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->Log("Renderer3D: Error initializing OpenGL!");
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
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
bool trRenderer3D::PreUpdate(float dt)
{
	//Color c = cam->background;
	//glClearColor(c.r, c.g, c.b, c.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

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

	//RENDER GUI
	App->editor->Draw();

	//SWAP BUFFERS
	SDL_GL_SwapWindow(App->win->window);
	return true;
}

// Called before quitting
bool trRenderer3D::CleanUp()
{
	App->editor->Log("Renderer3D: CleanUp");

	SDL_GL_DeleteContext(context);

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