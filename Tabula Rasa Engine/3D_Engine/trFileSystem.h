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

	bool DoesFileExist(char* file_name) const;
	bool AddNewPath(const char* path);

	bool OpenFileForWriting(char* file_name) const;

private:

};

#endif // __trFILESYSTEM_H__