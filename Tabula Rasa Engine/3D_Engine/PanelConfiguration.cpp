#include "PanelConfiguration.h"
#include "trApp.h"
#include "trModule.h"
#include "trHardware.h"
#include "trWindow.h"
#include "trInput.h"
#include "trAudio.h"
#include "trRenderer3D.h"
#include "trCamera3D.h"
#include "trEditor.h"
#include "imgui_defs.h"

using namespace std;

PanelConfiguration::PanelConfiguration() : Panel("Configuration", SDL_SCANCODE_4),
chart_fps(CHART_FPS_CAP), chart_ms(CHART_FPS_CAP)
{
	width = 330;
	height = 420;
	x_pos = 960;
	y_pos = 610;
}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::BeginMenu("Options", false))
	{
		if (ImGui::MenuItem("Set Defaults"))
			TR_LOG("TODO: Set defaults");

		if (ImGui::MenuItem("Load"))
			TR_LOG("TODO: Load prefs");

		if (ImGui::MenuItem("Save"))
			TR_LOG("TODO: Save prefs");
			
		ImGui::EndMenu();
	}

	ShowApplication();

	if (SetUpCollapsingHeader(App->win))
		ShowWindow(App->win);

	if (SetUpCollapsingHeader(App->render))
		ShowRenderer(App->render);

	if (SetUpCollapsingHeader(App->audio))
		ShowAudio(App->audio);

	if (SetUpCollapsingHeader(App->input))
		ShowInput(App->input);

	if (SetUpCollapsingHeader(App->hardware))
		ShowHardware(App->hardware);

	ImGui::End();
}

bool PanelConfiguration::SetUpCollapsingHeader(trModule* module)
{
	bool ret = false;

	if (ImGui::CollapsingHeader(module->name.c_str()))
	{
		(module->active) ? ImGui::Text("Module Actived") : ImGui::Text("Module Disabled");
		ImGui::SameLine();
		bool is_active = module->active;
		if (ImGui::Checkbox("Active", &is_active)) {
			module->TurnActive();
			module->CleanUp();
		}
			

		ret = true;
	}

	return ret;
}

void PanelConfiguration::ShowApplication()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		strcpy_s(app_name, TITLE_ORG_MAX_LENGTH, App->GetTitle());
		if (ImGui::InputText("App Name", app_name, TITLE_ORG_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->win->SetTitle(app_name);

		strcpy_s(org_name, TITLE_ORG_MAX_LENGTH, App->GetOrganization());
		if (ImGui::InputText("Organization", org_name, TITLE_ORG_MAX_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrganization(org_name);

		int fps_cap = App->GetFpsCap();
		if (ImGui::SliderInt("Max FPS", &fps_cap, 0, 120))
			App->SetFpsCap(fps_cap);

		ImGui::Text("Current FPS cap:");
		ImGui::SameLine();
							//or IMGUI_YELLOW!
		ImGui::TextColored(ImVec4(0.f,1.f,1.f,1.f), "%i", App->GetFpsCap());

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", chart_fps[chart_fps.size() - 1]);
		ImGui::PlotHistogram("##framerate", &chart_fps[0], chart_fps.size(), 0, title, 0.0f, 100.0f, ImVec2(400, 90));
		sprintf_s(title, 25, "Milliseconds %0.1f", chart_ms[chart_ms.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &chart_ms[0], chart_ms.size(), 0, title, 0.0f, 40.0f, ImVec2(400, 90));
	}
}

void PanelConfiguration::ShowHardware(trHardware * module)
{
	trHardware::HWInfo info = module->GetHardwareInfo();
	IMGUI_PRINT("SDL Version:", "v%u.%u %u", info.sdl_version[0], info.sdl_version[1], info.sdl_version[2]);

	ImGui::Separator();
	IMGUI_PRINT("CPUs:", "%u (Cache: %ukb)", info.cpu_count, info.cache_line_size);
	IMGUI_PRINT("System RAM:", "%.1fGb", info.system_ram);
	IMGUI_PRINT("Caps:", "%s%s%s%s%s%s",
		info.has_rtdcs ? "RDTSC," : "",
		info.has_alti_vec ? "AltiVec," : "",
		info.has_mmx ? "MMX," : "",
		info.has_3d_now ? "3DNow," : "",
		info.has_sse ? "SSE," : "",
		info.has_sse2 ? "SSE2," : "");
	IMGUI_PRINT("", "%s%s%s%s%s",
		info.has_sse3 ? "SSE3," : "",
		info.has_sse41 ? "SSE41," : "",
		info.has_sse42 ? "SSE42," : "",
		info.has_avx ? "AVX," : "",
		info.has_avx2 ? "AVX2" : "");
}

void PanelConfiguration::ShowAudio(trAudio * module)
{
	//todo
}

void PanelConfiguration::ShowInput(trInput * module)
{
	//todo
}

void PanelConfiguration::ShowWindow(trWindow * module)
{
	float brightness = App->win->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->win->SetBrightness(brightness);

	uint w, h, min_w, min_h, max_w, max_h;
	App->win->GetWindowConstraints(min_w, min_h, max_w, max_h);
	w = App->win->width;
	h = App->win->height;

	if (ImGui::SliderInt("Width", (int*)&w, min_w, max_w))
		App->win->SetWidth(w);

	if (ImGui::SliderInt("Height", (int*)&h, min_h, max_h))
		App->win->SetHeigth(h);
	
	ImGui::Text("Refresh rate:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%u", App->win->GetMonitorRefreshRate());


	if (ImGui::Checkbox("Fullscreen", &App->win->fullscreen))
		App->win->SetFullscreen(App->win->fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &App->win->resizable))
		App->win->SetResizable(App->win->resizable);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply");

	if (ImGui::Checkbox("Borderless", &App->win->borderless))
		App->win->SetBorderless(App->win->borderless);

	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &App->win->fullscreen_desktop))
		App->win->SetFullScreenDesktop(App->win->fullscreen_desktop);
}

void PanelConfiguration::ShowRenderer(trRenderer3D * module)
{
	//todo
}

void PanelConfiguration::FillChartFpsInfo(float fps, float ms)
{
	if (chart_iterator == CHART_FPS_CAP)
	{
		for (uint i = 0; i < CHART_FPS_CAP - 1; i++)
		{
			chart_fps[i] = chart_fps[i + 1];
			chart_ms[i] = chart_ms[i + 1];
		}
	}
	else
		chart_iterator++;

	chart_fps[chart_iterator - 1] = fps;
	chart_ms[chart_iterator - 1] = ms;
}
