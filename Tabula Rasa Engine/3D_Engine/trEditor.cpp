
#include "trApp.h"
#include "trWindow.h"
#include "trEditor.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelInspector.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include "SDL\include\SDL.h"
#include "SDL\include\SDL_opengl.h"

#include <vector>

trEditor::trEditor() : trModule()
{
	this->name = "Editor";
}

// Destructor
trEditor::~trEditor()
{
}

bool trEditor::Start()
{
	// Setup Dear ImGui binding
	TR_LOG("trEditor: Init editor with IMGUI lib version %s", ImGui::GetVersion());

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui_ImplSDL2_InitForOpenGL(App->window->window, App->render->context);
	ImGui_ImplOpenGL2_Init();

	// Setup style
	//ImGui::StyleColorsDark();

	// Maya-like style

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("fonts/Roboto-Regular.ttf", 16.0f);

	ImGuiStyle& style = ImGui::GetStyle();

	style.ChildRounding = 3.f;
	style.GrabRounding = 0.f;
	style.WindowRounding = 4.f;
	style.ScrollbarRounding = 3.f;
	style.FrameRounding = 3.f;
	style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

	style.Colors[ImGuiCol_Text] = ImVec4(0.73f, 0.73f, 0.73f, 1.00f);
	style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.26f, 0.26f, 0.26f, 0.95f);
	style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_Border] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
	style.Colors[ImGuiCol_TitleBg] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.21f, 0.21f, 0.21f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PopupBg] = ImVec4(0.32f, 0.32f, 0.32f, 1.00f);
	style.Colors[ImGuiCol_CheckMark] = ImVec4(0.78f, 0.78f, 0.78f, 1.00f);
	style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.11f, 0.11f, 0.11f, 1.00f);
	style.Colors[ImGuiCol_Header] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_Column] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_ColumnHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ColumnActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.36f, 0.36f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	style.Colors[ImGuiCol_Button] = ImVec4(0.59f, 0.59f, 0.59f, 1.00f);
	style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.98f, 0.39f, 0.36f, 1.00f);
	style.Colors[ImGuiCol_PlotLines] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
	style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.32f, 0.52f, 0.65f, 1.00f);
	style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.20f, 0.20f, 0.20f, 0.50f);



	//Panels
	panels.push_back(about = new PanelAbout());
	panels.push_back(config = new PanelConfiguration());
	panels.push_back(console = new PanelConsole(init_logs));
	panels.push_back(inspector = new PanelInspector());

	return true;
}

bool trEditor::PreUpdate(float dt)
{

	ImGuiIO& io = ImGui::GetIO();
	user_using_keyboard = io.WantCaptureKeyboard;
	user_using_mouse = io.WantCaptureMouse;

	if (have_to_quit)
		return false;

	return true;
}

bool trEditor::Update(float dt)
{
	ImGui_ImplOpenGL2_NewFrame();
	//todo: do a getter for the window
	ImGui_ImplSDL2_NewFrame(App->window->window);
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
				App->Save();
			if (ImGui::MenuItem("Load"))
				App->Load();
			if (ImGui::MenuItem("Quit", "Alt+F4"))
				have_to_quit = true;

			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console", "1"))
				console->TurnActive();

			if (ImGui::MenuItem("Configuration", "2"))
				config->TurnActive();

			if (ImGui::MenuItem("Inspector", "I"))
				inspector->TurnActive();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Gui Demo"))
				show_demo_window = true;

			if (ImGui::MenuItem("Documentation"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/");

			if (ImGui::MenuItem("Download latest"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/releases");
			
			if (ImGui::MenuItem("Report a bug"))
				App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/issues");

			if (ImGui::MenuItem("About"))
				about->TurnActive();

			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}


	// Draw active panels
	std::vector<Panel*>::iterator it = panels.begin();
	while( it != panels.end())
	{
		Panel* panel = (*it);

		if (App->input->GetKey(panel->GetCurrentShortCut()) == KEY_DOWN)
			panel->TurnActive();

		if (panel->IsActive())
			panel->Draw();

		it++;
	}

	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);

	return true;
}

bool trEditor::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool trEditor::CleanUp()
{
	Log("trEditor: CleanUp");
	std::vector<Panel*>::iterator it = panels.begin();

	while (it != panels.end()) {
		delete(*it);//is this legal? todo go to each panel
		it++;
	}
	panels.clear();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(App->render->context);
	SDL_DestroyWindow(App->window->window);
	SDL_Quit();

	return true;
}

void trEditor::Draw()
{
	ImGui::Render();
	ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
}

void trEditor::SetupInspectorWith(Mesh* mesh)
{
	inspector->SetDataFrom(mesh);
}

void trEditor::SetupInspectorWith(Texture * texture)
{
	inspector->SetDataFrom(texture);
}


void trEditor::InfoFPSMS(float current_fps, float current_ms)
{
	(config != nullptr) ? config->FillChartFpsInfo(current_fps, current_ms) : Log("trEditor error: configuration == nullptr");
}

void trEditor::Log(const char * new_log)
{
	if (console != nullptr)
		console->AddLogToConsole(new_log);
	else {
		init_logs.push_back(new_log);
	}
}
