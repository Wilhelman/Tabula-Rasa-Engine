#include "trApp.h"
#include "trFileSystem.h"
#include "trLog.h"
#include "PhysFS/include/physfs.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

trFileSystem::trFileSystem()
{
	this->name = "FileSystem";

	// Initializing PhysFS library
	TR_LOG("trFileSystem: initializing PhysFS library...\n");
	char* base_path = SDL_GetBasePath();
	PHYSFS_init(base_path);
	SDL_free(base_path);

	TR_LOG("trFileSystem: setting search directories...\n");
	
	// Setting search directories
	AddNewPath(".");
	AddNewPath("../Game");

	// Setting game directory to write in it
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
	bool ret = false;

	TR_LOG("trFileSystem: deinitializing PHYSFS...\n");

	if (PHYSFS_deinit() != 0)
	{
		ret = true;
		TR_LOG("trFileSystem: PhysFs successfully deinitialized...\n");
	}
	else
		TR_LOG("trFileSystem: error while deinitaliazing PHYSFS: %s\n", PHYSFS_getLastError());

	return true;
}

bool trFileSystem::DoesFileExist(char * file_name) const
{
	bool ret = false;

	if (PHYSFS_exists(file_name) != 0)
		ret = true;

	return ret;
}

bool trFileSystem::AddNewPath(const char * path)
{
	bool ret = false;

	if (PHYSFS_mount(path, nullptr, 1) != 0)
	{
		ret = true;
		TR_LOG("trFileSystem: archive/dir successfully added to the search path.\n");
	}
	else
		TR_LOG("trFileSystem: could not add archive/dir to the search path: %s\n", PHYSFS_getLastErrorCode());


	return ret;
}

bool trFileSystem::OpenFileForWriting(char * file_name) const
{
	bool ret = false;

	if (PHYSFS_openWrite(file_name) != nullptr)
	{
		ret = true;
		TR_LOG("trFileSystem: file %s successfully opened for writing.\n", file_name);
	}
	else
		TR_LOG("trFileSystem: could not open file %s for writting: %s\n", file_name, PHYSFS_getLastErrorCode());

	return ret;
}