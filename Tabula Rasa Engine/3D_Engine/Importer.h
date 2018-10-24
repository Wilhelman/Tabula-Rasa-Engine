#ifndef __IMPORTER_H__
#define __IMPORTER_H__

#include "trModule.h"
#include "trDefs.h"


class Importer
{

public:
	Importer() { }
	~Importer() { }

	virtual bool Import(const char* file, const char* path, std::string& output_file) { return true; }
	virtual bool Import(const void* buffer, uint size, std::string& output_file) { return true; }
	virtual bool Load(const char* exported_file, void* resource) { return true; }
};

#endif // __trIMPORTER_H__