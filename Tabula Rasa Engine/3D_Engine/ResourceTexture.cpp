#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(UID uid) : Resource(uid, Resource::Type::TEXTURE)
{
}

ResourceTexture::~ResourceTexture()
{
}

const char * ResourceTexture::GetFormatStr() const
{
	//TODO
	return nullptr;
}

bool ResourceTexture::LoadInMemory()
{
	return false;
}
