#include "trApp.h"
#include "trFileSystem.h"
#include "trLog.h"
#include "PhysFS/include/physfs.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

trFileSystem::trFileSystem()
{
	this->name = "FileSystem";

	// Initializing PhysFS library
	TR_LOG("trFileSystem: initializing PhysFS library...");
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	// Setting game directory to write in it
	TR_LOG("trFileSystem: Setting game directory to write in it...");
	PHYSFS_setWriteDir(".");

	// TODO: here we should create all the needed directories here
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