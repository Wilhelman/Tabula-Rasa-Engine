#include "trDefs.h"
#include "trWindow.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trEditor.h"
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

	if (err != GLEW_OK)
	{
		App->editor->Log("Glew library could not init");
		ret = false;
	}
	else {
		App->editor->Log("Using Glew ", (char*)glewGetString(GLEW_VERSION));
	}
		

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			App->editor->Log("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//TR_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			App->editor->Log("Renderer3D: Error initializing OpenGL!");
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
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
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
bool trRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

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

	ProjectionMatrix = math::float4x4::OpenGLPerspProjRH(0.125f,512.0f,(float)width, (float)height);
	glLoadMatrixf((GLfloat*)ProjectionMatrix.Transposed().ptr());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
