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

#include "MathGeoLib\MathGeoLib.h"
#include "Glew\include\GL\glew.h"
#include "DevIL\include\ilut.h"

#include "SDL\include\SDL_opengl.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib")


trRenderer3D::trRenderer3D() : trModule()
{
	name = "renderer3D";
}

// Destructor
trRenderer3D::~trRenderer3D()
{}

// Called before render is available
bool trRenderer3D::Awake(JSON_Object* config)
{
	TR_LOG("Renderer3D: Creating 3D Renderer context");

	bool ret = true;

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if (config != nullptr) {
		if (json_object_get_boolean(config, "vsync")) {
			flags |= SDL_RENDERER_PRESENTVSYNC;
			TR_LOG("Renderer3D: vSync ENABLED");
		}
		else {
			TR_LOG("Renderer3D: vSync DISABLED");
		}
		
	}

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		TR_LOG("Renderer3D: OpenGL context could not be created!SDL_Error: %s\n", SDL_GetError());
		ret = false;
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

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			TR_LOG("Renderer3D: Warning: Unable to set VSync!SDL Error : %s\n", SDL_GetError());

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//TR_LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			TR_LOG("Renderer3D: Error initializing OpenGL!");
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
			TR_LOG("Renderer3D: Error initializing OpenGL!");
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
			TR_LOG("Renderer3D: Error initializing OpenGL!");
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

	ilutRenderer(ILUT_OPENGL);

	// Loading FAKE texture
	/*const int checker_height = 120;
	const int checker_width = 120;
	GLubyte checkImage[checker_height][checker_width][4];
	for (int i = 0; i < checker_height; i++) {
		for (int j = 0; j < checker_width; j++) {
			int c = ((((i & 0x8) == 0) ^ (((j & 0x8)) == 0))) * 255;
			checkImage[i][j][0] = (GLubyte)c;
			checkImage[i][j][1] = (GLubyte)c;
			checkImage[i][j][2] = (GLubyte)c;
			checkImage[i][j][3] = (GLubyte)255;
		}
	}*/

	

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT);

	return ret;
}

// PreUpdate: clear buffer
bool trRenderer3D::PreUpdate(float dt)
{
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

	//RENDER IMPORTED MESH
	if (!meshes.empty())
	{
		this->Draw();

		if (show_mesh_vertices)
		{
			for (int i = 0; i < vertex_vec.size(); i++)
				vertex_vec[i].Render();
		}

		if (show_mesh_vertices_normals)
		{
			for (int i = 0; i < vertex_normals_vec.size(); i++)
				vertex_normals_vec[i].Render();
		}

		if (show_mesh_faces_normals)
		{
			for (int i = 0; i < face_normals_vec.size(); i++)
			{
				point_face_normals_vec[i].Render();
				face_normals_vec[i].Render();
			}	
		}
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
	GenerateMeshDebug(mesh);

	glGenBuffers(1, (GLuint*) &(mesh->buffer_vertex));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_vertex);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * mesh->num_vertex, mesh->vertex, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(mesh->buffer_uv));
	glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_uv);
	glBufferData(GL_ARRAY_BUFFER, 2 * mesh->num_uv * sizeof(GLfloat), mesh->uv, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, (GLuint*) &(mesh->buffer_index));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_index);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	meshes.push_back(mesh);
}

void trRenderer3D::GenerateMeshDebug(Mesh* mesh)
{
	//TODO CLEAN vertex and normals

	vertex_vec.reserve(mesh->num_vertex);
	vertex_normals_vec.reserve(mesh->num_vertex);

	// Filling vectors with vertices and vertices' normals coordinates
	for (int i = 0; i < mesh->num_vertex * 3.f; i += 3)
	{
		math::vec vertex_pos(mesh->vertex[i], mesh->vertex[i + 1], mesh->vertex[i + 2]);
		math::vec normal_pos(mesh->normals[i], mesh->normals[i + 1], mesh->normals[i + 2]);
		math::vec destination = vertex_pos + normal_pos;

		PArrow normal(vertex_pos, destination, math::float4(0.f, 1.f, 0.f, 1.f));
		PPoint vertex(vertex_pos, math::float4(0.f, 0.f, 1.f, 1.f));

		vertex_normals_vec.push_back(normal);
		vertex_vec.push_back(vertex);
	}

	face_normals_vec.reserve(mesh->num_faces);

	// Filling vector with faces' normals
	for (uint i = 0; i < mesh->num_vertex * 3.f; i += 9) // todo: check i
	{
		math::vec tri_a(mesh->vertex[i], mesh->vertex[i + 1], mesh->vertex[i + 2]);
		math::vec tri_b(mesh->vertex[i + 3], mesh->vertex[i + 4], mesh->vertex[i + 5]);
		math::vec tri_c(mesh->vertex[i + 6], mesh->vertex[i + 7], mesh->vertex[i + 8]);

		math::vec tri_center((tri_a.x + tri_b.x + tri_c.x) / 3.0f,
					         (tri_a.y + tri_b.y + tri_c.y) / 3.0f,
							 (tri_a.z + tri_b.z + tri_c.z) / 3.0f);

		math::vec u(tri_b - tri_a);
		math::vec v(tri_c - tri_a);
		math::vec result_normal = math::Cross(u, v).Normalized();
		math::vec destination = tri_center + result_normal;

		PPoint normal_point(tri_center, math::float4(0.f, 0.5f, 0.5f, 1.f));
		PArrow normal(tri_center, destination, math::float4(0.f, 0.f, 1.f, 1.f));
		point_face_normals_vec.push_back(normal_point);
		face_normals_vec.push_back(normal);
	}
}

void trRenderer3D::SetTexture(ImageTexture * texture)
{
	//this->last_texture = texture->image;

	/*glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, (GLuint*)last_texture->index);
	glBindTexture(GL_TEXTURE_2D, last_texture->index);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_BPP), (GLsizei)last_texture->width, (GLsizei)last_texture->height, 0, ilGetInteger(IL_IMAGE_FORMAT), GL_UNSIGNED_BYTE, &last_texture->image);*/

}


void trRenderer3D::ClearScene()
{
	vertex_normals_vec.clear();
	face_normals_vec.clear();
	point_face_normals_vec.clear();
	vertex_vec.clear();

	std::vector<Mesh*>::iterator it = meshes.begin();
	while (it != meshes.end())
	{
		delete (*it);
		it++;
	}

	meshes.clear();
}

void trRenderer3D::Draw()
{
	last_texture = App->texture->LoadImageFromPath("Models/Baker_house.png");

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	std::vector<Mesh*>::iterator it = meshes.begin();
	while (it != meshes.end())
	{
		Mesh* mesh = (*it);
		//glColor4f(mesh->mat_color.r, mesh->mat_color.g, mesh->mat_color.b, mesh->mat_color.a); wtf

		glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (last_texture != 0u) {
			glBindBuffer(GL_ARRAY_BUFFER, mesh->buffer_uv);
			glBindTexture(GL_TEXTURE_2D, last_texture);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->buffer_index);
		glDrawElements(GL_TRIANGLES, mesh->num_index, GL_UNSIGNED_INT, NULL);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//glColor4f(1.f, 1.f, 1.f, 1.f);
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