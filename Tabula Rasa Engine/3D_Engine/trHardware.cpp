#include "trHardware.h"
#include "trEditor.h"
#include "trApp.h"
#include "SDL/include/SDL.h"


trHardware::trHardware()
{
	this->name = "Hardware";
}

trHardware::~trHardware()
{
}

bool trHardware::Start()
{
	SDL_version sdl_version;
	SDL_GetVersion(&sdl_version);
	hw_info.sdl_version[0] = sdl_version.major;
	hw_info.sdl_version[1] = sdl_version.minor;
	hw_info.sdl_version[2] = sdl_version.patch;

	hw_info.cpu_count = SDL_GetCPUCount();
	hw_info.cache_line_size = SDL_GetCPUCacheLineSize();

	hw_info.system_ram = (float)SDL_GetSystemRAM() / MB_2_GB;
	hw_info.has_3d_now = SDL_Has3DNow();
	hw_info.has_avx = SDL_HasAVX();
	hw_info.has_avx2 = SDL_HasAVX2();
	hw_info.has_alti_vec = SDL_HasAltiVec();
	hw_info.has_mmx = SDL_HasMMX();
	hw_info.has_rtdcs = SDL_HasRDTSC();
	hw_info.has_sse = SDL_HasSSE();
	hw_info.has_sse2 = SDL_HasSSE2();
	hw_info.has_sse3 = SDL_HasSSE3();
	hw_info.has_sse41 = SDL_HasSSE41();
	hw_info.has_sse42 = SDL_HasSSE42();

	App->editor->Log("trHardware: Obtained hardware info");
	return true;
}

trHardware::HWInfo trHardware::GetHardwareInfo() const
{
	return hw_info;
}





