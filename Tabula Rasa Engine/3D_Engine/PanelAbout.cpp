#include "PanelAbout.h"

#include "ImGui/imgui.h"
#include "trApp.h"
#include "trHardware.h"
#include "trEditor.h"
#include "Glew/include/GL/glew.h"
#include "Assimp/include/version.h"
#include "DevIL/include/il.h"
#include "ImGui/imgui_defs.h"
#include "PhysFS/include/physfs.h"

PanelAbout::PanelAbout() : Panel("About")
{
}

PanelAbout::~PanelAbout()
{}

void PanelAbout::Draw()
{
	ImGui::Begin("About Tabula Rasa Engine", &active, ImGuiWindowFlags_AlwaysAutoResize);
	
	ImGui::Text("Version %s", App->GetVersion()); 
	ImGui::SameLine();
	if (ImGui::Button("Github##TABULARASA"))
		App->RequestBrowser("https://github.com/Wilhelman/Tabula-Rasa-Engine/");
	ImGui::Separator();
	ImGui::Text("By Guillermo Garcia Subirana and Victor Maso Garcia.");
	
	if (ImGui::CollapsingHeader("Tools and libraries"))
	{
		ImGui::Text("Programming language: C++");

		ImGui::Text("IDE: Microsoft Visual Studio 2017");
		ImGui::SameLine();
		if (ImGui::Button("Web page"))
			App->RequestBrowser("https://visualstudio.microsoft.com/?rr=https%3A%2F%2Fwww.google.es%2F");

		ImGui::Text("Input and audio: SDL v%i.%i.%i", App->hardware->GetHardwareInfo().sdl_version[0], App->hardware->GetHardwareInfo().sdl_version[1], App->hardware->GetHardwareInfo().sdl_version[2]);
		ImGui::SameLine();
		if (ImGui::Button("Web page##Second"))
			App->RequestBrowser("https://www.libsdl.org/index.php");

		ImGui::Text("Graphics: OpenGL version supported: %s", glGetString(GL_VERSION));
		ImGui::SameLine();
		if (ImGui::Button("Web page##Third"))
			App->RequestBrowser("https://www.opengl.org/");

		ImGui::Text("Extension loading library: Glew v%s", (char*)glewGetString(GLEW_VERSION));
		ImGui::SameLine();
		if (ImGui::Button("Web page##Fourth"))
			App->RequestBrowser("http://glew.sourceforge.net/");

		ImGui::Text("Math: MathGeoLib v1.5");
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

		ImGui::Text("JSON parser: Parson v2017");
		ImGui::SameLine();
		if (ImGui::Button("Github##Third"))
			App->RequestBrowser("https://github.com/kgabis/parson");

		ImGui::Text("Profiler: Brofiler v1.1.2");
		ImGui::SameLine();
		if (ImGui::Button("Web page##Eigth"))
			App->RequestBrowser("http://www.brofiler.com/");

		ImGui::Text("File system: PHYSFS", PHYSFS_VER_MAJOR, PHYSFS_VER_MINOR, PHYSFS_VER_PATCH);
		ImGui::SameLine();
		if (ImGui::Button("Web page##Nineth"))
			App->RequestBrowser("http://www.brofiler.com/");

		ImGui::Text("Memomory manager: mmgr");
		ImGui::SameLine();
		if (ImGui::Button("Flipcode archive"))
			App->RequestBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");
	}

	if (ImGui::CollapsingHeader("License"))
	{
		ImGui::Text("MIT License");
		ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy");
		ImGui::Text("of this software and associated documentation files(the ""Software""), to deal");
		ImGui::Text("in the Software without restriction, including without limitation the rights");
		ImGui::Text("to use, copy, modify, merge, publish, distribute, sublicense, and/or sell");
		ImGui::Text("copies of the Software, and to permit persons to whom the Software is");
		ImGui::Text("furnished to do so, subject to the following conditions :\n\n");

		ImGui::TextColored(IMGUI_RED, "The above copyright notice and this permission notice shall be included in all");
		ImGui::TextColored(IMGUI_RED, "copies or substantial portions of the Software.\n\n");

		ImGui::Text("THE SOFTWARE IS PROVIDED ""AS IS"", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR");
		ImGui::Text("IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, ");
		ImGui::Text("FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE");
		ImGui::Text("AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER");
		ImGui::Text("LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,");
		ImGui::Text("OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE");
		ImGui::Text("SOFTWARE.");
	}

	ImGui::End();
}
