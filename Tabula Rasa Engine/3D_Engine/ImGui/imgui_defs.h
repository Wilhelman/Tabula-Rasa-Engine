#pragma once

#include "imgui.h"
#include "imgui_internal.h"

// Memory chart
#define CHART_FPS_CAP 100
#define CHART_MEM 100

// Window attributes
#define MIN_WIDTH 300
#define MAX_WIDTH 2000
#define MIN_HEIGHT 300
#define MAX_HEIGHT 2000

// Colors
#define IMGUI_RED ImVec4(1.f,0.f,0.f,1.f)
#define IMGUI_GREEN ImVec4(0.f,1.f,0.f,1.f)
#define IMGUI_BLUE ImVec4(0.2f,0.2f,1.f,1.f)
#define IMGUI_YELLOW ImVec4(1.f,1.f,0.f,1.f)
#define IMGUI_WHITE ImVec4(1.f,1.f,1.f,1.f)

// Names
#define TITLE_MAX_LENGTH 77