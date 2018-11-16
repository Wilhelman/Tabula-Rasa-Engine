#include "ResourceScene.h"

ResourceScene::ResourceScene(UID uid): Resource(uid, Resource::Type::SCENE)
{
}

ResourceScene::~ResourceScene()
{
}

bool ResourceScene::LoadInMemory()
{
	return false;
}

bool ResourceScene::ReleaseMemory()
{
	return false;
}
