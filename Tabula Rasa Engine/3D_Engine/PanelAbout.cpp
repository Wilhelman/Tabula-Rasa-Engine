#include "PanelAbout.h"

#include "imgui.h"
#include "trApp.h"
#include "trHardware.h"
#include "trEditor.h"
#include "Glew\include\GL\glew.h"
#include "Assimp\include\version.h"
#include "DevIL\include\il.h"

PanelAbout::PanelAbout() : Panel("About")
{
}

PanelAbout::~PanelAbout()
{}

void PanelAbout::Draw()
{
	ImGui::Begin("About Tabula Rasa Engine", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);

	ImGui::Text("Version %s", App->GetVersion());
	ImGui::Separator();
	ImGui::Text("By Guillermo Garcia Subirana and Victor Maso Garcia.");
	ImGui::Separator();
	ImGui::Text("Tools used:");

	ImGui::Text("Programming language: C++");

	ImGui::Text("IDE: Microsoft Visual Studio 2017");
	ImGui::SameLine();
	if (ImGui::Button("Web page"))
		App->RequestBrowser("https://visualstudio.microsoft.com/?rr=https%3A%2F%2Fwww.google.es%2F");

	ImGui::Text("Input and audio: SDL v%i.%i.%i", App->hardware->GetHardwareInfo().sdl_version[0], App->hardware->GetHardwareInfo().sdl_version[1], App->hardware->GetHardwareInfo().sdl_version[2]);
	ImGui::SameLine();
	if (ImGui::Button("Web page##Second"))
		App->RequestBrowser("https://www.libsdl.org/index.php");

	ImGui::Text("Graphics: OpenGL v%s", (char*)glGetString(GL_VERSION));
	ImGui::SameLine();
	if (ImGui::Button("Web page##Third"))
		App->RequestBrowser("https://www.opengl.org/");

	ImGui::Text("Extension loading library: Glew v%s", (char*)glewGetString(GLEW_VERSION));
	ImGui::SameLine();
	if (ImGui::Button("Web page##Fourth"))
		App->RequestBrowser("http://glew.sourceforge.net/");

	ImGui::Text("Math: MathGeoLib v");
	ImGui::SameLine();
	if (ImGui::Button("GitHub"))
		App->RequestBrowser("https://github.com/juj/MathGeoLib");

	ImGui::Text("Random Number Generator: PCG (C Implementation) v0.94");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Fifth"))
		App->RequestBrowser("http://www.pcg-random.org/");

	ImGui::Text("GUI: Dear ImGui v%s", ImGui::GetVersion());
	ImGui::SameLine();
	if (ImGui::Button("Github##Second"))
		App->RequestBrowser("https://github.com/ocornut/imgui");

	ImGui::Text("3D Model importer: Assimp v%i.%i.%i", aiGetVersionMinor(), aiGetVersionMajor(), aiGetVersionRevision());
	ImGui::SameLine();
	if (ImGui::Button("Web page##Sixth"))
		App->RequestBrowser("http://www.assimp.org/");

	ImGui::Text("Image loader: Developer's Image Library (DevIL) v%i.%i.%i", 1, 8, 0);
	ImGui::SameLine();
	if (ImGui::Button("Web page##Seventh"))
		App->RequestBrowser("http://openil.sourceforge.net/");

	ImGui::Text("JSON parser: Parson v%");
	ImGui::SameLine();
	if (ImGui::Button("Github##Third"))
		App->RequestBrowser("https://github.com/ocornut/imgui");

	ImGui::Text("Profiler: Brofiler");
	ImGui::SameLine();
	if (ImGui::Button("Web page##Eigth"))
		App->RequestBrowser("http://www.brofiler.com/");

	ImGui::Text("Memomory manager: mmgr");
	ImGui::SameLine();
	if (ImGui::Button("Flipcode archive"))
		App->RequestBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");

	ImGui::Separator();
	ImGui::Text("Tabula Rasa Engine is licensed under MIT/Public???, see LICENSE for more information.");
	ImGui::End();
}
