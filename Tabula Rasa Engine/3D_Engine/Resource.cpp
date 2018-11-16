#include "Resource.h"

Resource::Resource(UID uid, Resource::Type type)
{
	this->uid = uid;
	this->type = type;
}

Resource::Type Resource::GetType() const
{
	return type;
}

UID Resource::GetUID() const
{
	return uid;
}

const char * Resource::GetFileName() const
{
	return file_name.c_str();
}

const char * Resource::GetImportedFile() const
{
	return imported_path.c_str();
}

const char * Resource::GetExportedFile() const
{
	return exported_path.c_str();
}

void Resource::SetFileName(const char * file_name)
{
	this->file_name = file_name;
}

void Resource::SetImportedPath(const char * imported_path)
{
	this->imported_path = imported_path;
}

void Resource::SetExportedPath(const char * exported_path)
{
	this->exported_path = exported_path;
}

bool Resource::IsLoadedToMemory() const
{
	return (references > 0u);
}

uint Resource::LoadToMemory()
{
	if (references <= 0) {
		if (LoadInMemory())
			references++;
	}
	else
		references++;

	return references;
}

uint Resource::CountReferences() const
{
	return references;
}

void Resource::UnReference()
{
	references--;
	if (references <= 0) {
		references = 0;

	}
}
