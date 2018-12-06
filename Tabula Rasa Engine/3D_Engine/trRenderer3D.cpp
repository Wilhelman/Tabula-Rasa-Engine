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
#include "ComponentCamera.h"

#include "ResourceMesh.h"
#include "ResourceTexture.h"

#include "trOpenGL.h"

#define N_PLANE 0.125f
#define F_PLANE 1024.0f
#define FOV 60.0f

#define DEFAULT_AMBIENT_COLOR {0.f,120.f,120.f,255.f}


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
		vsync_toogle = json_object_get_boolean(config, "vsync");
		if (vsync_toogle) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
				vsync_toogle = false;
			}
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
	else {
		wireframe = R_WIREFRAME;
		depth_test = R_DEPTH_TEST;
		cull_face = R_CULL_FACE;
		lighting = R_LIGHTING;
		z_buffer = R_ZBUFFER;
		color_material = R_COLOR_MATERIAL;
		texture_2D = R_TEXTURE_2D;
		vsync_toogle = R_VSYNC;
		if (vsync_toogle) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
				vsync_toogle = false;
			}
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
	ComponentCamera* camera_co = nullptr;

	if (App->IsRunTime()) 
		camera_co = (ComponentCamera*)App->main_scene->main_camera->FindComponentByType(Component::component_type::COMPONENT_CAMERA);
	else 
	{
		App->camera->dummy_camera->projection_needs_update = true;
		camera_co = App->camera->dummy_camera;
	}

	if (camera_co->projection_needs_update)
	{
		UpdateCameraProjection();
		camera_co->projection_needs_update = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera_co->GetViewMatrix().ptr());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->dummy_camera->frustum.pos.x, App->camera->dummy_camera->frustum.pos.y, App->camera->dummy_camera->frustum.pos.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();
	
	return true;
}

// PostUpdate present buffer to screen
bool trRenderer3D::PostUpdate(float dt)
{
	// Filter the drawable gos:
	ComponentCamera* camera_co = nullptr;

	if (App->IsRunTime())
		camera_co = (ComponentCamera*)App->main_scene->main_camera->FindComponentByType(Component::component_type::COMPONENT_CAMERA);
	else
	{
		App->camera->dummy_camera->projection_needs_update = true;
		camera_co = App->camera->dummy_camera;
	}

	meshable_go.clear();
	drawable_go.clear();

	App->main_scene->CollectDinamicGOs(meshable_go);

	// Camera culling
	ComponentCamera* main_camera_co = (ComponentCamera*)App->main_scene->main_camera->FindComponentByType(Component::component_type::COMPONENT_CAMERA);
	// Quadtree update

	App->main_scene->quadtree.CollectsGOs(main_camera_co->frustum, meshable_go);

	if (main_camera_co->frustum_culling) {

		for (uint i = 0; i < meshable_go.size(); i++)
		{
			if (main_camera_co->FrustumContainsAaBox(meshable_go.at(i)->bounding_box))
				meshable_go.at(i)->in_camera = true;
			else
				meshable_go.at(i)->in_camera = false;
		}

		CollectActiveInCameraGameObjects();
	}
	else {
		CollectActiveGameObjects();
	}

	//RENDER GEOMETRY
	if (App->main_scene != nullptr)
		App->main_scene->Draw();

	if (debug_draw_on)
	{
		App->main_scene->DrawDebug();
	}

	//RENDER IMPORTED MESH
	if (!drawable_go.empty()) 
	{
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

	SDL_GL_DeleteContext(context); // TODO: crash here whem importing scene multiple times
	return true;
}

bool trRenderer3D::Load(const JSON_Object * config)
{
	if (config != nullptr) {
		wireframe = json_object_get_boolean(config, "wireframe");
		depth_test = json_object_get_boolean(config, "depth_test");
		cull_face = json_object_get_boolean(config, "cull_face");
		lighting = json_object_get_boolean(config, "lighting");
		z_buffer = json_object_get_boolean(config, "z_buffer");
		color_material = json_object_get_boolean(config, "color_material");
		texture_2D = json_object_get_boolean(config, "texture_2D");
		vsync_toogle = json_object_get_boolean(config, "vsync");
		if (vsync_toogle) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
				vsync_toogle = false;
			}
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
	else {
		wireframe = R_WIREFRAME;
		depth_test = R_DEPTH_TEST;
		cull_face = R_CULL_FACE;
		lighting = R_LIGHTING;
		z_buffer = R_ZBUFFER;
		color_material = R_COLOR_MATERIAL;
		texture_2D = R_TEXTURE_2D;
		vsync_toogle = R_VSYNC;
		if (vsync_toogle) {
			if (SDL_GL_SetSwapInterval(1) < 0) {
				TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());
				vsync_toogle = false;
			}
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

	SwitchWireframeMode(wireframe);
	SwitchDepthMode(depth_test);
	SwitchFaceCulling(cull_face);
	SwitchLighting(lighting);
	SwitchColorMaterial(color_material);
	SwitchTexture2D(texture_2D);

	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight());
	return true;
}

bool trRenderer3D::Save(JSON_Object * config) const
{
	json_object_set_boolean(config, "vsync", vsync_toogle);
	json_object_set_boolean(config, "wireframe", wireframe);
	json_object_set_boolean(config, "depth_test", depth_test);
	json_object_set_boolean(config, "z_buffer", z_buffer);
	json_object_set_boolean(config, "cull_face", cull_face);
	json_object_set_boolean(config, "lighting", lighting);
	json_object_set_boolean(config, "color_material", color_material);
	json_object_set_boolean(config, "texture_2D", texture_2D);
	return true;
}

void trRenderer3D::OnResize(int width, int height)
{
	ComponentCamera* camera_co = nullptr;
	if (App->IsRunTime())
		camera_co = (ComponentCamera*)App->main_scene->main_camera->FindComponentByType(Component::component_type::COMPONENT_CAMERA);
	else
		camera_co = App->camera->dummy_camera;

	camera_co->SetAspectRatio((float)width / (float)height);

	glViewport(0, 0, width, height);

	UpdateCameraProjection();
}

void trRenderer3D::UpdateCameraProjection()
{
	ComponentCamera* camera_co = nullptr;
	if (App->IsRunTime())
		camera_co = (ComponentCamera*)App->main_scene->main_camera->FindComponentByType(Component::component_type::COMPONENT_CAMERA);
	else
		camera_co = App->camera->dummy_camera;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glLoadMatrixf((GLfloat*)camera_co->GetProjectionMatrix().ptr());

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
	
	wireframe = toggle;
}

void trRenderer3D::SwitchDepthMode(bool toggle)
{
	(toggle) ?
		glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);

	depth_test = toggle;
}

void trRenderer3D::SwitchFaceCulling(bool toggle)
{
	(toggle) ?
		glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);

	cull_face = toggle;
}

void trRenderer3D::SwitchLighting(bool toggle)
{
	(toggle) ?
		glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);

	lighting = toggle;
}

void trRenderer3D::SwitchColorMaterial(bool toggle)
{
	(toggle) ?
		glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);

	color_material = toggle;
}

void trRenderer3D::SwitchTexture2D(bool toggle)
{
	(toggle) ?
		glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

	texture_2D = toggle;
}

void trRenderer3D::SwitchVsync(bool toggle)
{
	SDL_GL_SetSwapInterval(toggle);

	vsync_toogle = toggle;
}

void trRenderer3D::SwitchDebugDraw(bool toggle)
{
	debug_draw_on = toggle;
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
		glMultMatrixf((GLfloat*)(*it)->GetTransform()->GetMatrix().Transposed().ptr());

		ComponentMesh* mesh_co = (ComponentMesh*)(*it)->FindComponentByType(Component::component_type::COMPONENT_MESH);
		ResourceMesh* mesh = (ResourceMesh*)mesh_co->GetResource();

		ComponentMaterial* material_co = (ComponentMaterial*)(*it)->FindComponentByType(Component::component_type::COMPONENT_MATERIAL);
		ResourceTexture* texture = nullptr;

		float4 ambient_color = DEFAULT_AMBIENT_COLOR;
		if (material_co != nullptr) 
		{
			texture = (ResourceTexture*)material_co->GetResource();
			//ambient_color = material_co->GetAmbientColor();
		}

		if (texture != nullptr) {
			if (mesh->uv_buffer != 0 && material_co)
			{
				glEnable(GL_ALPHA_TEST);
				glAlphaFunc(GL_GREATER, material_co->alpha_test);
				glBindTexture(GL_TEXTURE_2D, texture->gpu_id);
			}
		}
			
		if (material_co && texture == nullptr) {
			glColor4f(ambient_color.w, ambient_color.x, ambient_color.y, ambient_color.z);
		}
		if (texture == nullptr || !texture_2D) // If the texture is missing, we set the ambient color of the mesh
			glColor4f(ambient_color.w, ambient_color.x, ambient_color.y, ambient_color.z);
		
		if (mesh) {
			glBindBuffer(GL_ARRAY_BUFFER, mesh->vertex_buffer);
			glVertexPointer(3, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			//texture
			if (mesh->uvs != nullptr && mesh->uv_buffer != 0)
			{
				glBindBuffer(GL_ARRAY_BUFFER, mesh->uv_buffer);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->index_buffer);
			glDrawElements(GL_TRIANGLES, mesh->index_size, GL_UNSIGNED_INT, NULL);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}

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

void trRenderer3D::CollectActiveGameObjects()
{
	for (uint i = 0u; i < meshable_go.size(); i++)
	{
		if (meshable_go.at(i)->is_active && !meshable_go.at(i)->to_destroy) {
			ComponentMesh* mesh_co = (ComponentMesh*)meshable_go.at(i)->FindComponentByType(Component::component_type::COMPONENT_MESH);
			if (mesh_co && mesh_co->GetResource())
				drawable_go.push_back(meshable_go.at(i));
		}
	}
}

void trRenderer3D::CollectActiveInCameraGameObjects()
{
	for (uint i = 0u; i < meshable_go.size(); i++)
	{
		if (meshable_go.at(i)->is_active && meshable_go.at(i)->in_camera && !meshable_go.at(i)->to_destroy) {
			ComponentMesh* mesh_co = (ComponentMesh*)meshable_go.at(i)->FindComponentByType(Component::component_type::COMPONENT_MESH);
			if (mesh_co) {
				ResourceMesh* mesh = (ResourceMesh*)mesh_co->GetResource();
				if (mesh_co && mesh)
					drawable_go.push_back(meshable_go.at(i));
			}
		}
	}
}
