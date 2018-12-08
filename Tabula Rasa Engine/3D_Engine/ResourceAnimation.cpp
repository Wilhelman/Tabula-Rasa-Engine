#include "ResourceAnimation.h"

ResourceAnimation::ResourceAnimation(UID uid) : Resource(uid, Resource::Type::ANIMATION)
{
}

ResourceAnimation::~ResourceAnimation()
{
}

bool ResourceAnimation::LoadInMemory()
{
	return true;
}

bool ResourceAnimation::ReleaseMemory()
{
	return true;
}
