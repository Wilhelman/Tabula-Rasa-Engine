#include "ResourceBone.h"

ResourceBone::ResourceBone(UID uid) : Resource(uid, Resource::Type::BONE)
{
}

ResourceBone::~ResourceBone()
{
}

bool ResourceBone::LoadInMemory()
{
	return true;
}

bool ResourceBone::ReleaseMemory()
{
	return true;
}
