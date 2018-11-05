#ifndef __trFILESYSTEM_H__
#define __trFILESYSTEM_H__

#include "trModule.h"

class trFileSystem : public trModule
{
public:
	trFileSystem();
	~trFileSystem();

	bool Start();
	bool CleanUp();

private:

};

#endif // __trFILESYSTEM_H__