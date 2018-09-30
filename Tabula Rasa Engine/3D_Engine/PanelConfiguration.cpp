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
#include "mmgr/mmgr.h"
#include "trTimer.h"

using namespace std;

PanelConfiguration::PanelConfiguration() : Panel("Configuration", SDL_SCANCODE_4),
chart_fps(CHART_FPS_CAP), chart_ms(CHART_FPS_CAP)
{
	width = 330;
	height = 420;
	x_pos = 960;
	y_pos = 610;

	mem_timer.Start();
}

PanelConfiguration::~PanelConfiguration()
{}

void PanelConfiguration::Draw()
{
	ImGui::Begin("Configuration", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing);

	if (ImGui::BeginMenu("OpenGL Options"))
	{
		ImGui::Text("Switch wireframe");
		ImGui::SameLine();
		bool is_active = true;
		if (ImGui::Checkbox("", &is_active)) {
			//todo
		}
		
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

		ImGui::Separator();

		sMStats mem_stats = m_getMemoryStatistics();

		if (mem_timer.Read() > 0.05f)
		{
			mem_timer.Start();
			if (mem_list.size() == CHART_MEM)
			{
				for (uint i = 0; i < CHART_MEM - 1; i++)
					mem_list[i] = mem_list[i + 1];

				mem_list[CHART_MEM - 1] = (float)mem_stats.totalReportedMemory;
			}
			else
				mem_list.push_back((float)mem_stats.totalReportedMemory);
			
		}

	

		ImGui::PlotHistogram("##memory", &mem_list[0], mem_list.size(), 0, "Memory Consumption", 0.0f, (float)mem_stats.peakReportedMemory * 1.2f, ImVec2(310, 100));


		ImGui::Text("Total Reported Mem: %u", mem_stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", mem_stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", mem_stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", mem_stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", mem_stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", mem_stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", mem_stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", mem_stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", mem_stats.peakAllocUnitCount);
	}
}

void PanelConfiguration::ShowHardware(trHardware * module)
{
	trHardware::HWInfo info = module->GetHardwareInfo();
	std::string info_str;
	info_str = "v%u.%u.%u";
	ImGui::Text("SDL Version:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.sdl_version[0], info.sdl_version[1], info.sdl_version[2]);

	ImGui::Separator();
	info_str = "%u (Cache: %ukb)";
	ImGui::Text("CPUs:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.cpu_count, info.cache_line_size);

	info_str = "%.1fGb";
	ImGui::Text("System RAM:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.system_ram);

	info_str = "%s%s%s%s%s%s";
	ImGui::Text("Caps:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.has_rtdcs ? "RDTSC," : "",
		info.has_alti_vec ? "AltiVec," : "",
		info.has_mmx ? "MMX," : "",
		info.has_3d_now ? "3DNow," : "",
		info.has_sse ? "SSE," : "",
		info.has_sse2 ? "SSE2," : "");

	info_str = "%s%s%s%s%s";
	ImGui::Text("");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, info_str.c_str(), info.has_sse3 ? "SSE3," : "",
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
	ImGui::Text("Mouse Position:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", module->GetMouseX(), module->GetMouseY());
	ImGui::Text("Mouse Motion:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", module->GetMouseXMotion(), module->GetMouseYMotion());
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


	//TODO : this is VERY UNSTABLE
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
	static uint chart_iterator = 0u;

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
