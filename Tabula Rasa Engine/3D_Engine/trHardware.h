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

	int version_major, version_minor, version_patch;

};

#endif // __trHARDWARE_H__