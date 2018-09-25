#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#define IMGUI_GREY ImVec4(0.6f,0.6f,0.6f,1.f)
#define IMGUI_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_YELLOW ImVec4(1.f,1.f,0.f,1.f)
#define IMGUI_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_WHITE ImVec4(1.f,1.f,1.f,1.f)

#define IMGUI_PRINT(field, format, ...) \
	ImGui::Text(field); \
	ImGui::SameLine(); \
	ImGui::TextColored(IMGUI_YELLOW, format, __VA_ARGS__)