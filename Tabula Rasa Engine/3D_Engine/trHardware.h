#ifndef __trHARDWARE_H__
#define __trHARDWARE_H__

#include "trModule.h"

#define MB_2_GB 1024.0f

class trHardware : public trModule
{

public:

	struct HWInfo
	{
		unsigned int sdl_version[3];
		unsigned int cpu_count = 0u;
		unsigned int cache_line_size = 0u;
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
	};
	

public:

	trHardware();

	// Destructor
	~trHardware();

	HWInfo GetHardwareInfo();


private:

	HWInfo hw_info;
	bool log_displayed = false;
};

#endif // __trHARDWARE_H__