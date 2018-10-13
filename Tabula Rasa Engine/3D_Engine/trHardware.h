#ifndef __trHARDWARE_H__
#define __trHARDWARE_H__

#include "trModule.h"
#include "trDefs.h"
#include "Glew/include/GL/glew.h"

// Memory conversions
#define MEM_CONV_FACTOR 1024.0f
#define CONV_MEM_UP(mem) mem / MEM_CONV_FACTOR
#define CONV_MEM_DOWN(mem) mem * MEM_CONV_FACTOR

// GPU info
#define GPU_MEMORY_INFO_DEDICATED_VIDMEM_NVX          0x9047
#define GPU_MEMORY_INFO_TOTAL_AVAILABLE_MEMORY_NVX    0x9048
#define GPU_MEMORY_INFO_CURRENT_AVAILABLE_VIDMEM_NVX  0x9049
#define GPU_MEMORY_INFO_EVICTION_COUNT_NVX            0x904A
#define GPU_MEMORY_INFO_EVICTED_MEMORY_NVX            0x904B

class trHardware : public trModule
{

public:

	struct HWInfo
	{
		unsigned int sdl_version[3];

		uint cpu_count = 0u;
		uint cache_line_size = 0u;
		float system_ram = 0.0f;

		bool has_3d_now = false;
		bool has_avx = false;
		bool has_avx2 = false;
		bool has_alti_vec = false;
		bool has_mmx = false;
		bool has_rtdcs = false;
		bool has_sse = false;
		bool has_sse2 = false;
		bool has_sse3 = false;
		bool has_sse41 = false;
		bool has_sse42 = false;
		char* gpu_vendor = nullptr;
		char* gpu_model = nullptr;

		GLint vram_budget = 0;
		GLint vram_usage = 0;
		GLint vram_available = 0;
		GLint vram_reserved = 0;
	};
	

public:

	trHardware();

	// Destructor
	~trHardware();
	bool Start();
	bool Update(float dt);
	HWInfo GetHardwareInfo() const;


private:

	HWInfo hw_info;
};

#endif // __trHARDWARE_H__