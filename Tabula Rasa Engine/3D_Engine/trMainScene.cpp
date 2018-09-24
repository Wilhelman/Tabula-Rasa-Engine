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

	if (show_about_window)
		ShowAboutWindow(&show_about_window);

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
				ShellExecute(GetActiveWindow(), "open", "https://github.com/Wilhelman/Tabula-Rasa-Engine/issues", NULL, NULL, SW_SHOWNORMAL);

			if (ImGui::MenuItem("About"))
				show_about_window = true;

			ImGui::EndMenu();
		}

		if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
			TR_LOG("Todo open console");

		if (App->input->GetKey(SDL_SCANCODE_4) == KEY_DOWN)
			TR_LOG("Todo open configuration");

		ImGui::EndMainMenuBar();
	}
}

void trMainScene::ShowAboutWindow(bool * p_open)
{
	ImGui::Begin("About Tabula Rasa Engine", p_open, ImGuiWindowFlags_AlwaysAutoResize);

	ImGui::Text("TODO: Version");

	ImGui::Separator();

	//todo check link
	ImGui::Text("By Guillermo Garcia Subirana & Victor Maso Garcia");

	ImGui::Text("TODO: Licensed under MIT");

	ImGui::End();
}