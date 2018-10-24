#ifndef __trIMPORTER_H__
#define __trIMPORTER_H__

#include "trModule.h"
#include "trDefs.h"


class trImporter
{

public:

	

public:

	trImporter();

	// Destructor
	~trImporter();
	virtual bool Import(const char* file, const char* path, std::string& output_file) = 0;
	virtual bool Import(const void* buffer, uint size, std::string& output_file) = 0;
	virtual bool Load(const char* exported_file, void* resource) = 0;

private:

};

#endif // __trIMPORTER_H__