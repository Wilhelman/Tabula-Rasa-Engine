#include "trDefs.h"
#include "trWindow.h"
#include "trApp.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trMainScene.h"
#include "trEditor.h"

#include "GameObject.h"
#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

#include "Glew\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")

#define N_PLANE 0.125f
#define F_PLANE 1024.0f
#define FOV 60.0f

#define DEFAULT_AMBIENT_COLOR {0.f,0.f,255.f,255.f}


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
		wireframe = json_object_get_boolean(config, "wireframe");
		depth_test = json_object_get_boolean(config, "depth_test");
		cull_face = json_object_get_boolean(config, "cull_face");
		lighting = json_object_get_boolean(config, "lighting");
		z_buffer = json_object_get_boolean(config, "z_buffer");
		color_material = json_object_get_boolean(config, "color_material");
		texture_2D = json_object_get_boolean(config, "texture_2D");

		if (json_object_get_boolean(config, "vsync")) {
			if (SDL_GL_SetSwapInterval(1) < 0)
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
			else
			{
				vsync_toogle = true;
				TR_LOG("Renderer3D: vSync ENABLED");
			}
				
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

		lights[0].Active(true);
		
		SwitchWireframeMode(wireframe);
		SwitchDepthMode(depth_test);
		SwitchFaceCulling(cull_face);
		SwitchLighting(lighting);
		SwitchColorMaterial(color_material);
		SwitchTexture2D(texture_2D);
		
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
	if (App->main_scene != nullptr) {
		App->main_scene->Draw();
	}

	//RENDER DEBUG
	/// not yet

	//RENDER IMPORTED MESH
	drawable_go.clear();
	CollectGameObjectWithMesh(App->main_scene->GetRoot());

	if (!drawable_go.empty()) {
		this->Draw();
		if(z_buffer)
			this->DrawZBuffer();
	}

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
	return true;
}

void trRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	ProjectionMatrix = this->Perspective(FOV, (float)width / (float)height, N_PLANE, F_PLANE);
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

void trRenderer3D::SwitchVsync(bool toggle)
{
	SDL_GL_SetSwapInterval(toggle);
}

const uint trRenderer3D::GetMeshesSize() const
{
	return drawable_go.size();
}

void trRenderer3D::Draw()
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	std::vector<GameObject*>::iterator it = drawable_go.begin();
	while (it != drawable_go.end())
	{
		glPushMatrix();
		glMultMatrixf((GLfloat*)(*it)->GetTransform()->GetMatrix());

		ComponentMesh* mesh_co = (ComponentMesh*)(*it)->FindComponentWithType(Component::component_type::COMPONENT_MESH);
		const Mesh* mesh = mesh_co->GetMesh();

		ComponentMaterial* material_co = (ComponentMaterial*)(*it)->FindComponentWithType(Component::component_type::COMPONENT_MATERIAL);
		const Texture* texture = nullptr;
		float4 ambient_color = DEFAULT_AMBIENT_COLOR;
		if (material_co != nullptr) {
			texture = material_co->GetTexture();
			ambient_color = material_co->GetAmbientColor();
		}

		if(texture != nullptr)
			glBindTexture(GL_TEXTURE_2D, texture->id);

		if (texture == nullptr || !texture_2D) // If the texture is missing, we set the ambient color of the mesh
			glColor4f(ambient_color.w, ambient_color.x, ambient_color.y, ambient_color.z);
		
		glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//texture
		if (mesh->uvs != nullptr) {
			glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_buffer);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
		glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glColor4f(1.f, 1.f, 1.f, 1.f);

		if (texture != nullptr)
			glBindTexture(GL_TEXTURE_2D, 0);


		glPopMatrix();

		it++;
	}
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void trRenderer3D::DrawZBuffer()
{
	int width = App->window->GetWidth();
	int height = App->window->GetHeight();
	float* data = new float[width * height];

	// First, we read the Z-Buffer and store its values in data
	glReadPixels(0, 0, width, height, GL_DEPTH_COMPONENT, GL_FLOAT, data);

	// Once we have the values we need to make the data non-linear. We want a lot of precission 
	// for small z values (closer to the camera) and much less precision for high z values (further
	// from the camera) as we care more about what it's in our near vision than what's far away.
	float* first_data = data;

	for (uint i = 0; i < width * height; i++)
	{
		(*data) = (*data) * 2.f - 1.f;
		(*data) = (2.0 * (N_PLANE / 16) * F_PLANE) / (F_PLANE + (N_PLANE / 16) - (*data) * (F_PLANE - (N_PLANE / 16)));
		data++;
	}

	glDrawPixels(width, height, GL_LUMINANCE, GL_FLOAT, first_data);
	delete[] first_data;
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

void trRenderer3D::CollectGameObjectWithMesh(GameObject* game_object)
{
	if (game_object->FindComponentWithType(Component::component_type::COMPONENT_MESH))
		drawable_go.push_back(game_object);

	for (std::list<GameObject*>::const_iterator it = game_object->childs.begin(), end = game_object->childs.end(); it != end; it++)
		CollectGameObjectWithMesh(*it);
}