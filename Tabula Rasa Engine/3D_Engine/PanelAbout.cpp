#include "PanelAbout.h"

#include "imgui.h"
#include "trApp.h"

PanelAbout::PanelAbout() : Panel("About")
{
}

PanelAbout::~PanelAbout()
{}

void PanelAbout::Draw()
{
	ImGui::Begin("About Tabula Rasa Engine", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Version %s", VERSION);
	ImGui::Separator();
	ImGui::Text("By Guillermo Garcia Subirana and Victor Maso Garcia.");
	ImGui::Separator();
	ImGui::Text("Tools used:");

	ImGui::Text("Programming language: C++");

	ImGui::Text("IDE: Microsoft Visual Studio 2017");
	ImGui::SameLine();
	if (ImGui::Button("Web page"))
		App->RequestBrowser("https://visualstudio.microsoft.com/?rr=https%3A%2F%2Fwww.google.es%2F");

	ImGui::Text("Input and audio: SDL v2.0.8");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Second"))
		App->RequestBrowser("https://www.libsdl.org/index.php");

	ImGui::Text("Graphics: OpenGL2");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Third"))
		App->RequestBrowser("https://www.opengl.org/");

	ImGui::Text("Math: MathGeoLib v");
	ImGui::SameLine();
	if (ImGui::Button("GitHub"))
		App->RequestBrowser("https://github.com/juj/MathGeoLib");

	ImGui::Text("Random Number Generator: PCG (C Implementation) v0.94");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Fourth"))
		App->RequestBrowser("http://www.pcg-random.org/");

	ImGui::Text("GUI: Dear ImGui v1.65");
	ImGui::SameLine();
	if (ImGui::Button("Github##Second"))
		App->RequestBrowser("https://github.com/ocornut/imgui");

	ImGui::Text("Profiler: Brofiler");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Fifth"))
		App->RequestBrowser("http://www.brofiler.com/");

	ImGui::Text("Memomory manager: mmgr");
	ImGui::SameLine();
	if (ImGui::Button("Flipcode archive"))
		App->RequestBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");

	// TODO: add JSON parser used

	ImGui::Separator();
	ImGui::Text("Tabula Rasa Engine is licensed under MIT/Public???, see LICENSE for more information.");
	ImGui::End();
}
