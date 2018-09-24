#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"
#include "trCamera3D.h"
#include "trWindow.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"


#define VSYNC true

trMainScene::trMainScene() : trModule()
{
	name = "main_scene";
}

// Destructor
trMainScene::~trMainScene()
{}

// Called before render is available
bool trMainScene::Awake(pugi::xml_node& config)
{
	TR_LOG("Wake me up. Wake me up inside!");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	App->camera->Move(vec(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec(0.0f, 0.0f, 0.0f));

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = ImGui::GetIO();
	(void)io;

	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

	//testing zone
	sphere_1 = Sphere(vec(0.f, 0.f, 0.f), 2.0f);
	sphere_2 = Sphere(vec(0.f, 0.f, 0.f), 1.5f);

	capsule_1 = Capsule(vec(3.f, 0.f, 0.f), vec(3.f, 2.f, 0.f), 1.f);
	capsule_2 = Capsule(vec(0.f, 0.f, 0.f), vec(0.f, 2.f, 0.f), 1.f);

	aabb_1 = sphere_1.MinimalEnclosingAABB();
	aabb_2 = sphere_2.MinimalEnclosingAABB();

	obb_1 = OBB(vec(0.0f, 0.0f, 0.0f), vec(1.0f, 1.0f, 1.0f), vec(1.0f, 0.0f, 0.0f), vec(0.0f, 1.0f, 0.0f), vec(0.0f, 0.0f, 1.0f));
	obb_2 = OBB(vec(2.5f, 0.0f, 0.0f), vec(1.0f, 1.0f, 1.0f), vec(1.0f, 0.0f, 0.0f), vec(0.0f, 1.0f, 0.0f), vec(0.0f, 0.0f, 1.0f));

	tri_1 = Triangle(vec(0.0f, 0.0f, 0.0f), vec(1.0f, 0.0f, 0.0f), vec(0.5f, 1.0f, 0.0f));
	tri_2 = Triangle(vec(0.0f, 0.0f, 0.0f), vec(0.0f, 0.0f, 1.0f), vec(0.0f, 1.0f, 0.5f));

	ray_1 = Ray(vec(10.0f, 0.0f, 0.0f), vec(1.0f, 1.0f, 1.0f));

	plane_1 = Plane(vec(0.f, 0.f, 1.f), 0.0f);
	plane_2 = Plane(vec(0.f, 0.f, 1.f), 2.0f);
	
	//end testing zone

	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate(float dt)
{

	if (quit)
		return false;

	return true;

}

bool trMainScene::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->win->window);
	ImGui::NewFrame();


	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	if(show_mgl_test_window)
		ShowMathGeoLibTestWindow(&show_mgl_test_window);

	if (show_random_window)
		ShowRandomWindow(&show_random_window);

	ShowMenuBar();

	return true;
}

bool trMainScene::PostUpdate(float dt)
{

	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	
	
	glLineWidth(1.0f);

	glBegin(GL_LINES);


	//testing grid
	/*float d = 100.0f;

	for (float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}*/

	glEnd();

	return true;
}

// Called before quitting
bool trMainScene::CleanUp()
{
	TR_LOG("Cleaning up trMainScene");
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->render->context);
	SDL_DestroyWindow(App->win->window);
	SDL_Quit();

	return true;
}

// Load Game State
bool trMainScene::Load(pugi::xml_node& data)
{
	return true;
}

// Save Game State
bool trMainScene::Save(pugi::xml_node& data) const
{

	return true;
}

void trMainScene::ShowMenuBar()
{
	static float f = 0.0f;
	static int counter = 0;

	//
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				quit = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
				TR_LOG("Todo open console");

			if (ImGui::MenuItem("Configuration", "4"))
				TR_LOG("Todo open configuration");

			ImGui::Checkbox("Demo Window (Ctrl + W)", &show_demo_window);

			ImGui::Checkbox("MathGeoLib Collision Tester (Ctrl + M)", &show_mgl_test_window);

			if (ImGui::MenuItem("Random window"))
				show_random_window = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
				TR_LOG("Todo open console");

			if (ImGui::MenuItem("Configuration", "4"))
				TR_LOG("Todo open configuration");

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
				show_demo_window = true;

			if (ImGui::MenuItem("Documentation"))
				TR_LOG("Todo open Documentation");

			if (ImGui::MenuItem("Download latest"))
				TR_LOG("Todo open Download latest link");

			if (ImGui::MenuItem("Report a bug"))
				TR_LOG("Todo open Download latest link");

			ImGui::Checkbox("MathGeoLib Collision Tester (Ctrl + M)", &show_mgl_test_window);

			if (ImGui::MenuItem("Random window"))
				show_random_window = true;

			ImGui::EndMenu();
		}
		
		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			TR_LOG("Todo open console");

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			TR_LOG("Todo open configuration");

		ImGui::EndMainMenuBar();
	}
}

void trMainScene::ShowRandomWindow(bool * p_open)
{
		ImGui::Begin("Random Generator", p_open, ImGuiWindowFlags_AlwaysAutoResize);
		ImGui::Separator();

		// Random between 0 and 1

		ImGui::Text("Random between 0 and 1:");

		if (ImGui::Button("Generate 1"))
			rand_num_range = ldexp(pcg32_random_r(&rng), -32);

		ImGui::SameLine();
		ImGui::Text("%f", rand_num_range);

		// Random between specific range

		ImGui::Text("Random between min and max (both included):");

		ImGui::InputInt("Min", &temp_min_num);
		ImGui::InputInt("Max", &max_num);

		if (ImGui::Button("Generate 2") && max_num >= temp_min_num)
		{
			min_num = temp_min_num;
			bound = max_num - min_num + 1;
			rand_min_max = pcg32_boundedrand_r(&rng, bound);
		}

		ImGui::SameLine();
		ImGui::Text("%d", rand_min_max + min_num);
	
		ImGui::End();
}

void trMainScene::ShowMathGeoLibTestWindow(bool* p_open)
{

	ImGui::Begin("MathGeoLib Collision Tester", p_open, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Separator();

	// Random between 0 and 1

	//ImGui::Text("Random between 0 and 1:");
	
	if (ImGui::CollapsingHeader("Spheres"))
	{
		ImGui::Text("Sphere 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1", &sphere_1.pos.x, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &sphere_1.pos.y, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &sphere_1.pos.z, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 1", &sphere_1.r, 0.0f, 10.0f);

		ImGui::Text("Sphere 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &sphere_2.pos.x, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &sphere_2.pos.y, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &sphere_2.pos.z, -5.0f, 10.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 2", &sphere_2.r, 0.0f, 10.0f);
		
		ImGui::Text("Sphere intersection");

		if (sphere_1.Intersects(sphere_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");

		ImGui::Separator();
		ImGui::Text("Sphere intersection w/ AABB");

		aabb_1 = sphere_1.MinimalEnclosingAABB();
		aabb_2 = sphere_2.MinimalEnclosingAABB();

		if (aabb_1.Intersects(aabb_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");
	}

	if (ImGui::CollapsingHeader("Capsules"))
	{
		ImGui::Text("Capsule 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 1", &capsule_1.r, 0.0f, 5.0f);
		
		ImGui::Text("Capsule 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 2", &capsule_2.r, 0.0f, 5.0f);

		if (capsule_1.Intersects(capsule_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");
	}

	if (ImGui::CollapsingHeader("OBB"))
	{
		ImGui::Text("OBB 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1", &obb_1.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &obb_1.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &obb_1.pos.z, -5.0f, 5.0f);

		ImGui::Text("OBB 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &obb_2.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &obb_2.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &obb_2.pos.z, -5.0f, 5.0f);
		
		if (obb_1.Intersects(obb_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");
	}

	if (ImGui::CollapsingHeader("Rays"))
	{
		ImGui::Text("Ray 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X dir", &ray_1.dir.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y dir", &ray_1.dir.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z dir", &ray_1.dir.z, -5.0f, 5.0f);

		if (ray_1.Intersects(sphere_1))
			ImGui::Text("INTERSECTS WITH SPHERE 1");
		else
			ImGui::Text("DOES NOT INTERSECT WITH SPHERE 1");
	}

	if (ImGui::CollapsingHeader("Triangles"))
	{

		ImGui::Text("Triangle 1:");

		ImGui::Text("Triangle 1 vertex A");
		ImGui::SliderFloat("X A vertex", &tri_1.a.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y A vertex", &tri_1.a.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z A vertex", &tri_1.a.z, -5.0f, 5.0f);

		ImGui::Text("Triangle 1 vertex B");
		ImGui::SliderFloat("X B vertex", &tri_1.b.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y B vertex", &tri_1.b.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z B vertex", &tri_1.b.z, -5.0f, 5.0f);

		ImGui::Text("Triangle 1 vertex C");
		ImGui::SliderFloat("X C vertex", &tri_1.c.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y C vertex", &tri_1.c.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z C vertex", &tri_1.c.z, -5.0f, 5.0f);

		ImGui::Text("Triangle 2:");

		ImGui::Text("Triangle 2 vertex A");
		ImGui::SliderFloat("X A 2 vertex", &tri_2.a.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y A 2 vertex", &tri_2.a.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z A 2 vertex", &tri_2.a.z, -5.0f, 5.0f);

		ImGui::Text("Triangle 2 vertex B");
		ImGui::SliderFloat("X B 2 vertex", &tri_2.b.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y B 2 vertex", &tri_2.b.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z B 2 vertex", &tri_2.b.z, -5.0f, 5.0f);

		ImGui::Text("Triangle 2 vertex C");
		ImGui::SliderFloat("X C 2 vertex", &tri_2.c.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y C 2 vertex", &tri_2.c.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z C 2 vertex", &tri_2.c.z, -5.0f, 5.0f);


		if (tri_1.Intersects(tri_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");
	}

	if (ImGui::CollapsingHeader("Planes"))
	{
		//todo transform + translate
		ImGui::Text("Plane 1:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 1", &plane_1.normal.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &plane_1.normal.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &plane_1.normal.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Signed Distance 1", &plane_1.d, 0.0f, 5.0f);

		ImGui::Text("Plane 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &plane_2.normal.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &plane_2.normal.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &plane_2.normal.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Signed Distance 2", &plane_2.d, 0.0f, 5.0f);

		if (plane_1.Intersects(plane_2))
			ImGui::Text("INTERSECT");
		else
			ImGui::Text("DON'T INTERSECT");
	}

	ImGui::End();

}
