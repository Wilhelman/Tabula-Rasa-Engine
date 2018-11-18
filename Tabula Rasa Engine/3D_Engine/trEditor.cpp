
#include "trApp.h"
#include "trWindow.h"
#include "trEditor.h"
#include "trCamera3D.h"
#include "trRenderer3D.h"
#include "trInput.h"

#include "Panel.h"
#include "PanelAbout.h"
#include "PanelConfiguration.h"
#include "PanelConsole.h"
#include "PanelInspector.h"
#include "PanelHierarchy.h"
#include "PanelResources.h"
#include "PanelControl.h"

#include "GameObject.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"
#include "ImGuizmo/ImGuizmo.h"
#include "SDL/include/SDL.h"
#include "SDL/include/SDL_opengl.h"

#include "Event.h"

#include "ComponentCamera.h"

#include "ImGuizmo/ImGuizmo.h"
#include "ImGui/imgui.h"

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
	ImGui_ImplOpenGL3_Init();

	// ImGui style
	{
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
	}


	//Panels
	about = new PanelAbout();
	config = new PanelConfiguration();
	console = new PanelConsole(init_logs);
	inspector = new PanelInspector();
	hierarchy = new PanelHierarchy();
	resources = new PanelResources();
	control = new PanelControl();
	panels.push_back(about);
	panels.push_back(config);
	panels.push_back(console);
	panels.push_back(inspector);
	panels.push_back(hierarchy);
	panels.push_back(resources);
	panels.push_back(control);

	return true;
}

bool trEditor::PreUpdate(float dt)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame(App->window->GetWindow());
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	if (have_to_quit)
	{
		ImGui::EndFrame();
		return false;
	}

	return true;
}

bool trEditor::Update(float dt)
{
	// Show/hide imgui
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_G) == KEY_DOWN)
		show_imgui = !show_imgui;

	if (!ImGuizmo::IsUsing())
	{
		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::TRANSLATE;
		else if (App->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::ROTATE;
		else if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			guizmo_operation = ImGuizmo::OPERATION::SCALE;
	}

	if (!show_imgui)
		return true;
	
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

			if (ImGui::MenuItem("Game Objects Hierarchy", "3"))
				hierarchy->TurnActive();

			if (ImGui::MenuItem("Assets", "4"))
				resources->TurnActive();

			if (ImGui::MenuItem("Inspector", "I"))
				inspector->TurnActive();

			if (ImGui::MenuItem("Show/Hide UI", "Alt+G"))
				show_imgui = !show_imgui;

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
		delete(*it);
		it++;
	}
	panels.clear();

	init_logs.clear();

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	this->active = false;
	return true;
}

void trEditor::OnEventReceived(const Event & event)
{
	if (event.GetType() == Event::event_type::GAMEOBJECT_DESTROYED) {
		this->selected = nullptr; // for now
	}
}

void trEditor::Draw()
{
	if (selected)
		DisplayGuizmos();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void trEditor::InfoFPSMS(float current_fps, float current_ms, int frames)
{
	(config != nullptr) ? config->FillChartFpsInfo(current_fps, current_ms, frames) : TR_LOG("trEditor error: configuration == nullptr");
}

void trEditor::Log(const char * new_log)
{
	if (console != nullptr)
		console->AddLogToConsole(new_log);
	else 
		init_logs.push_back(new_log);
}

GameObject * trEditor::GetSelected() const
{
	return selected;
}

void trEditor::SetSelected(GameObject * selected)
{
	if (!ImGui::IsMouseHoveringAnyWindow() && !ImGuizmo::IsOver())
		this->selected = selected;
}

void trEditor::DisplayGuizmos()
{
	ImGuizmo::Enable(true);

	ImGuizmo::OPERATION current_guizmo_operation = guizmo_operation;
	ImGuizmo::MODE current_guizmo_mode = guizmo_mode;

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);

	float4x4 view_matrix(App->camera->dummy_camera->GetViewMatrix());
	float4x4 proj_matrix(App->camera->dummy_camera->GetProjectionMatrix());
	float4x4 transform_matrix(selected->GetTransform()->GetMatrix());
	transform_matrix = transform_matrix.Transposed();

	ImGuizmo::Manipulate(view_matrix.ptr(),
						 proj_matrix.ptr(),
						 current_guizmo_operation,
						 current_guizmo_mode,
					 	 transform_matrix.ptr()); 

	if (ImGuizmo::IsUsing() && App->input->GetKey(SDL_SCANCODE_LALT) == KEY_IDLE)
	{
		/*float3 new_pos;
		Quat new_rot;
		float3 new_scale;

		transform_matrix.Transposed().Decompose(new_pos, new_rot, new_scale);*/

		transform_matrix = transform_matrix.Transposed();
		selected->GetTransform()->SetupFromGlobalMatrix(transform_matrix);
		selected->RecalculateBoundingBox();
	}
}