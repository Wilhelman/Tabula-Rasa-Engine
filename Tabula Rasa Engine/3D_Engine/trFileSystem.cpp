#include "trFileSystem.h"
#include "PhysFS\include\physfs.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

trFileSystem::trFileSystem()
{
}

trFileSystem::~trFileSystem()
{
}

bool trFileSystem::Start()
{
	return true;
}

bool trFileSystem::CleanUp()
{
	return true;
}