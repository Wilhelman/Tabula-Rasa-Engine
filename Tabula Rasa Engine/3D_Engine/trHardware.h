#ifndef __trHARDWARE_H__
#define __trHARDWARE_H__

#include "trModule.h"

class trHardware : public trModule
{
public:

	trHardware();

	// Destructor
	~trHardware();


private:

	char* sdl_version_string = nullptr;

};

#endif // __trHARDWARE_H__