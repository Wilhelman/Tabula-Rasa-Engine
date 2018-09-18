#include "trDefs.h"
#include "trLog.h"
#include "trApp.h"
#include "trMainScene.h"

#include "imgui.h"

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
	LOG("Wake me up. Wake me up inside!");
	bool ret = true;
	
	return ret;
}

// Called before the first frame
bool trMainScene::Start()
{/*
	LOG("Main Scene start");

	ImGui::Begin("My First Tool", true, ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
			if (ImGui::MenuItem("Save", "Ctrl+S")) { }
			if (ImGui::MenuItem("Close", "Ctrl+W")) { my_tool_active = false; }
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// Edit a color (stored as ~4 floats)
	ImGui::ColorEdit4("Color", my_color);

	// Plot some values
	const float my_values[] = { 0.2f, 0.1f, 1.0f, 0.5f, 0.9f, 2.2f };
	ImGui::PlotLines("Frame Times", my_values, IM_ARRAYSIZE(my_values));

	// Display contents in a scrolling region
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
	ImGui::BeginChild("Scrolling");
	for (int n = 0; n < 50; n++)
		ImGui::Text("%04d: Some text", n);
	ImGui::EndChild();
	ImGui::End();
	*/
	return true;
}

// Called each loop iteration
bool trMainScene::PreUpdate(float dt)
{
	return true;
}

bool trMainScene::Update(float dt)
{

	return true;
}

bool trMainScene::PostUpdate(float dt)
{
	return true;
}

// Called before quitting
bool trMainScene::CleanUp()
{
	LOG("Cleaning up trMainScene");
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