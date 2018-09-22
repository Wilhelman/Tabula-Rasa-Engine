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



//todo check the logs!

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
	//LOG("Wake me up. Wake me up inside!");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{
	pcg32_srandom_r(&rng, 42u, 54u);

	App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	App->camera->LookAt(vec3(0, 0, 0));

	// Setup Dear ImGui binding
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	io = ImGui::GetIO();
	(void)io;

	ImGui_ImplSDL2_InitForOpenGL(App->win->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	ImGui::StyleColorsDark();

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
	
	 d = ldexp(pcg32_random_r(&rng), -32);

	ImGui_ImplOpenGL2_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->win->window);
	ImGui::NewFrame();


	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	//just testing
	ShowTheFirstTabulaRasaWindow();


	//p.axis = true;
	

	return true;
}

bool trMainScene::PostUpdate(float dt)
{
	// Rendering

	//rendering the plane
	//p.Render();

	ImGui::Render();
	glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
	glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
	

	return true;
}

// Called before quitting
bool trMainScene::CleanUp()
{
	//LOG("Cleaning up trMainScene");
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

void trMainScene::ShowTheFirstTabulaRasaWindow()
{
	static float f = 0.0f;
	static int counter = 0;

	//
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Demo Window (Ctrl + W)", &show_demo_window);
			ImGui::EndMenu();
		}
		else if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			show_demo_window = !show_demo_window;

		ImGui::EndMainMenuBar();
	}
}

void trMainScene::ShowExampleMenuFile()
{
	if (ImGui::MenuItem("Exit", "Alt+F4"))
		quit = true;

}