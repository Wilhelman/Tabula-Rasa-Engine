#include "ResourceAnimation.h"

ResourceAnimation::ResourceAnimation(UID uid) : Resource(uid, Resource::Type::ANIMATION)
{
}

ResourceAnimation::~ResourceAnimation()
{
}

bool ResourceAnimation::LoadInMemory()
{
	// Done in Set resource ( Component )
	return true;
}

bool ResourceAnimation::ReleaseMemory()
{
	return true;
}
