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

	sphere_1 = Sphere(vec(0.f, 0.f, 0.f), 2.0f);
	sphere_2 = Sphere(vec(0.f, 0.f, 0.f), 1.5f);
	
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
			if (ImGui::MenuItem("Exit", "Alt+F4"))
				quit = true;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Demo Window (Ctrl + W)", &show_demo_window);

			ImGui::Checkbox("MathGeoLib Collision Tester (Ctrl + M)", &show_mgl_test_window);

			if (ImGui::MenuItem("Random window"))
				show_random_window = true;

			ImGui::EndMenu();
		}
		
		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			show_demo_window = !show_demo_window;

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN)
			show_mgl_test_window = !show_mgl_test_window;

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
		ImGui::Text("%i", rand_min_max + min_num);
	
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
		ImGui::SliderFloat("X 1", &sphere_1.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 1", &sphere_1.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 1", &sphere_1.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 1", &sphere_1.r, -5.0f, 5.0f);

		ImGui::Text("Sphere 2:");
		ImGui::SameLine();
		ImGui::SliderFloat("X 2", &sphere_2.pos.x, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Y 2", &sphere_2.pos.y, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Z 2", &sphere_2.pos.z, -5.0f, 5.0f);
		ImGui::SameLine();
		ImGui::SliderFloat("Radius 2", &sphere_2.r, -5.0f, 5.0f);

		if (sphere_1.Contains(sphere_2))
			ImGui::Text("Contains");
		else
			ImGui::Text("Not contains");

	}


	ImGui::End();

}
