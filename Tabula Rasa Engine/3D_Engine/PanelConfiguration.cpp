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

// ---------------------------------------------------------
PanelConfiguration::PanelConfiguration() : Panel("Configuration", SDL_SCANCODE_4),
fps_log(FPS_LOG_SIZE), ms_log(FPS_LOG_SIZE)
{
	width = 330;
	height = 420;
	posx = 960;
	posy = 610;
}

// ---------------------------------------------------------
PanelConfiguration::~PanelConfiguration()
{}

// ---------------------------------------------------------
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

	DrawApplication();

	if (InitModuleDraw(App->win))
		DrawModuleWindow(App->win);

	if (InitModuleDraw(App->render))
		DrawModuleRenderer(App->render);

	if (InitModuleDraw(App->camera))
		DrawModuleCamera(App->camera);

	if (InitModuleDraw(App->audio))
		DrawModuleAudio(App->audio);

	if (InitModuleDraw(App->input))
		DrawModuleInput(App->input);

	if (InitModuleDraw(App->hardware))
		DrawModuleHardware(App->hardware);

	ImGui::End();
}

bool PanelConfiguration::InitModuleDraw(trModule* module)
{
	bool ret = false;

	if (ImGui::CollapsingHeader(module->name.c_str()))
	{
		bool active = module->active;//todo:better with funciton call
		if (ImGui::Checkbox("Active", &active))
			module->active = active;
		ret = true;
	}

	return ret;
}

void PanelConfiguration::DrawApplication()
{
	if (ImGui::CollapsingHeader("Application"))
	{
		static char app_name[120];
		strcpy_s(app_name, 120, App->GetTitle());
		if (ImGui::InputText("App Name", app_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->win->SetTitle(app_name);

		static char org_name[120];
		strcpy_s(org_name, 120, App->GetOrganization());
		if (ImGui::InputText("Organization", org_name, 120, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
			App->SetOrganization(org_name);

		int max_fps = App->GetFramerateLimit();
		if (ImGui::SliderInt("Max FPS", &max_fps, 0, 120))
			App->SetFramerateLimit(max_fps);

		ImGui::Text("Limit Framerate:");
		ImGui::SameLine();
		ImGui::TextColored(ImVec4(0.f,1.f,1.f,1.f), "%i", App->GetFramerateLimit());

		char title[25];
		sprintf_s(title, 25, "Framerate %.1f", fps_log[fps_log.size() - 1]);
		ImGui::PlotHistogram("##framerate", &fps_log[0], fps_log.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
		sprintf_s(title, 25, "Milliseconds %0.1f", ms_log[ms_log.size() - 1]);
		ImGui::PlotHistogram("##milliseconds", &ms_log[0], ms_log.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		
		/*// Memory --------------------
		sMStats stats = m_getMemoryStatistics();
		static int speed = 0;
		static vector<float> memory(100);
		if (++speed > 20)
		{
			speed = 0;
			if (memory.size() == 100)
			{
				for (uint i = 0; i < 100 - 1; ++i)
					memory[i] = memory[i + 1];

				memory[100 - 1] = (float)stats.totalReportedMemory;
			}
			else
				memory.push_back((float)stats.totalReportedMemory);
		}

		ImGui::PlotHistogram("##memory", &memory[0], memory.size(), 0, "Memory Consumption", 0.0f, (float)stats.peakReportedMemory * 1.2f, ImVec2(310, 100));

		ImGui::Text("Total Reported Mem: %u", stats.totalReportedMemory);
		ImGui::Text("Total Actual Mem: %u", stats.totalActualMemory);
		ImGui::Text("Peak Reported Mem: %u", stats.peakReportedMemory);
		ImGui::Text("Peak Actual Mem: %u", stats.peakActualMemory);
		ImGui::Text("Accumulated Reported Mem: %u", stats.accumulatedReportedMemory);
		ImGui::Text("Accumulated Actual Mem: %u", stats.accumulatedActualMemory);
		ImGui::Text("Accumulated Alloc Unit Count: %u", stats.accumulatedAllocUnitCount);
		ImGui::Text("Total Alloc Unit Count: %u", stats.totalAllocUnitCount);
		ImGui::Text("Peak Alloc Unit Count: %u", stats.peakAllocUnitCount);
		*/
	}
}

void PanelConfiguration::DrawModuleHardware(trHardware * module)
{
	trHardware::HWInfo info = module->GetHardwareInfo();
	IMGUI_PRINT("SDL Version:", info.sdl_version);

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


	/*ImGui::Separator();
	IMGUI_PRINT("GPU:", "vendor %u device %u", info.gpu_vendor, info.gpu_device);
	IMGUI_PRINT("Brand:", info.gpu_brand);
	IMGUI_PRINT("VRAM Budget:", "%.1f Mb", info.vram_mb_budget);
	IMGUI_PRINT("VRAM Usage:", "%.1f Mb", info.vram_mb_usage);
	IMGUI_PRINT("VRAM Available:", "%.1f Mb", info.vram_mb_available);
	IMGUI_PRINT("VRAM Reserved:", "%.1f Mb", info.vram_mb_reserved);*/
}

void PanelConfiguration::DrawModuleAudio(trAudio * module)
{
	// General Volume
	/*float volume = module->GetVolume();
	if (ImGui::SliderFloat("General Volume", (float*)&volume, 0.0f, 1.0f))
		module->SetVolume(volume);

	// Music Volume
	float music_volume = module->GetMusicVolume();
	if (ImGui::SliderFloat("Music Volume", (float*)&music_volume, 0.0f, 1.0f))
		module->SetMusicVolume(music_volume);

	// FX Volume
	float fx_volume = module->GetFXVolume();
	if (ImGui::SliderFloat("FX Volume", (float*)&fx_volume, 0.0f, 1.0f))
		module->SetFXVolume(fx_volume);
		*/
}

void PanelConfiguration::DrawModuleInput(trInput * module)
{
	/*int mouse_x, mouse_y;
	module->GetMousePosition(mouse_x, mouse_y);
	ImGui::Text("Mouse Position:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", mouse_x, mouse_y);

	module->GetMouseMotion(mouse_x, mouse_y);
	ImGui::Text("Mouse Motion:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i,%i", mouse_x, mouse_y);

	int wheel = module->GetMouseWheel();
	ImGui::Text("Mouse Wheel:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%i", wheel);

	ImGui::Separator();

	ImGui::BeginChild("Input Log");
	ImGui::TextUnformatted(input_buf.begin());
	if (need_scroll)
		ImGui::SetScrollHere(1.0f);
	need_scroll = false;
	ImGui::EndChild();*/
}

void PanelConfiguration::DrawModuleWindow(trWindow * module)
{
	/*static bool waiting_to_load_icon = false;

	if (waiting_to_load_icon == true && App->editor->FileDialog("bmp"))
	{
		const char* file = App->editor->CloseFileDialog();
		if (file != nullptr)
			App->window->SetIcon(file);
		waiting_to_load_icon = false;
	}

	ImGui::Text("Icon: ");
	ImGui::SameLine();
	if (ImGui::Selectable(App->window->GetIcon()))
		waiting_to_load_icon = true;

	float brightness = App->window->GetBrightness();
	if (ImGui::SliderFloat("Brightness", &brightness, 0.0f, 1.0f))
		App->window->SetBrightness(brightness);

	uint w, h, min_w, min_h, max_w, max_h;
	App->window->GetMaxMinSize(min_w, min_h, max_w, max_h);
	w = App->window->GetWidth();
	h = App->window->GetHeight();

	if (ImGui::SliderInt("Width", (int*)&w, min_w, max_w))
		App->window->SetWidth(w);

	if (ImGui::SliderInt("Height", (int*)&h, min_h, max_h))
		App->window->SetHeigth(h);

	ImGui::Text("Refresh rate:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, "%u", App->window->GetRefreshRate());

	bool fullscreen = App->window->IsFullscreen();
	bool resizable = App->window->IsResizable();
	bool borderless = App->window->IsBorderless();
	bool full_desktop = App->window->IsFullscreenDesktop();

	if (ImGui::Checkbox("Fullscreen", &fullscreen))
		App->window->SetFullscreen(fullscreen);

	ImGui::SameLine();
	if (ImGui::Checkbox("Resizable", &resizable))
		App->window->SetResizable(resizable);
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("Restart to apply");

	if (ImGui::Checkbox("Borderless", &borderless))
		App->window->SetBorderless(borderless);

	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &full_desktop))
		App->window->SetFullScreenDesktop(full_desktop);*/
}

void PanelConfiguration::DrawModuleRenderer(trRenderer3D * module)
{
	/*ImGui::Text("Driver:");
	ImGui::SameLine();
	ImGui::TextColored(IMGUI_YELLOW, App->renderer3D->GetDriver());

	bool vsync = App->renderer3D->GetVSync();
	if (ImGui::Checkbox("Vertical Sync", &vsync))
		App->renderer3D->SetVSync(vsync);*/
}

void PanelConfiguration::DrawModuleCamera(trCamera3D * module)
{
	/*ImGui::DragFloat3("Front", &App->camera->GetDummy()->frustum.front.x, 0.1f);
	ImGui::DragFloat3("Up", &App->camera->GetDummy()->frustum.up.x, 0.1f);
	ImGui::DragFloat3("Position", &App->camera->GetDummy()->frustum.pos.x, 0.1f);
	ImGui::DragFloat("Mov Speed", &App->camera->mov_speed, 0.1f, 0.1f);
	ImGui::DragFloat("Rot Speed", &App->camera->rot_speed, 0.05f, 0.01f);
	ImGui::DragFloat("Zoom Speed", &App->camera->zoom_speed, 0.1f, 0.1f);
	App->editor->props->DrawCameraComponent(module->GetDummy());*/
}

float rnd(float min, float max)
{
	return max * ((float)rand() / (float)RAND_MAX);
}


void PanelConfiguration::AddInput(const char * entry)
{
	input_buf.appendf(entry); // df or the other?
	need_scroll = true;
}

void PanelConfiguration::AddFPS(float fps, float ms)
{
	static uint count = 0;

	if (count == FPS_LOG_SIZE)
	{
		for (uint i = 0; i < FPS_LOG_SIZE - 1; ++i)
		{
			fps_log[i] = fps_log[i + 1];
			ms_log[i] = ms_log[i + 1];
		}
	}
	else
		++count;

	fps_log[count - 1] = fps;
	ms_log[count - 1] = ms;
}
