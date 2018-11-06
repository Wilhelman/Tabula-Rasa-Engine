#include "trApp.h"
#include "trFileSystem.h"
#include "trLog.h"
#include "PhysFS/include/physfs.h"

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

trFileSystem::trFileSystem()
{
	this->name = "FileSystem";
}

trFileSystem::~trFileSystem() { }

bool trFileSystem::Awake(JSON_Object * config)
{
	// Initializing PhysFS library
	TR_LOG("trFileSystem: initializing PhysFS library...\n");
	PHYSFS_init(nullptr);

	TR_LOG("trFileSystem: setting search directories...\n");

	// Setting search directories
	AddNewPath(".");
	AddNewPath("../Game");

	// Setting game directory to write in it
	PHYSFS_setWriteDir(".");

	// TODO: here we should create all the needed directories here

	return true;
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

bool trFileSystem::DoesFileExist(const char * file_name) const
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
		TR_LOG("trFileSystem: could not add archive/dir to the search path: %s\n", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return ret;
}

PHYSFS_File* trFileSystem::OpenFileForWriting(const char * file_name) const
{
	PHYSFS_File* file = PHYSFS_openWrite(file_name);

	if (file != nullptr)
		TR_LOG("trFileSystem: file %s successfully opened for writing.\n", file_name);
	else
		TR_LOG("trFileSystem: could not open file %s for writting: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return file;
}

PHYSFS_File* trFileSystem::OpenFileForReading(const char* file_name) const
{
	PHYSFS_File* file = PHYSFS_openRead(file_name);

	if (file != nullptr)
		TR_LOG("trFileSystem: file %s successfully opened for reading.\n", file_name);
	else
		TR_LOG("trFileSystem: could not open file %s for reading: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));

	return file;
}

void trFileSystem::CloseFile(PHYSFS_File* file, const char* file_name) const
{
	if (PHYSFS_close(file) != 0)
		TR_LOG("trFileSystem: success on closing file %s\n", file_name);
	else
		TR_LOG("trFileSystem: could not close file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
}

bool trFileSystem::WriteInFile(const char* file_name, char* buffer, uint size) const
{
	bool ret = true;

	PHYSFS_File* file = OpenFileForWriting(file_name);

	if (file == nullptr || PHYSFS_writeBytes(file, (const void*)buffer, size) < size)
	{
		ret = false;
		TR_LOG("trFileSystem: could not write to file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}
	else
		TR_LOG("trFileSystem: success on writting to file %s\n", file_name);

	CloseFile(file, file_name);

	return ret;
}

bool trFileSystem::ReadFromFile(const char* file_name, char* buffer)
{
	bool ret = true;
	PHYSFS_File* file = OpenFileForReading(file_name);

	if (file != nullptr)
	{
		uint size = PHYSFS_fileLength(file);

		if (PHYSFS_readBytes(file, buffer, size) == -1)
		{
			ret = false;
			TR_LOG("trFileSystem: could not read from file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
		else
			TR_LOG("trFileSystem: success on reading from file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	CloseFile(file, file_name);

	return ret;
}

bool trFileSystem::MakeNewDir(const char * dir_name)
{
	bool ret = true;

	if (PHYSFS_mkdir(dir_name) != 0)
		TR_LOG("trFilesystem: success on creating directory %s\n", dir_name);
	else
	{
		ret = false;
		TR_LOG("trFilesystem: could not create directory %s: %s\n", dir_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return ret;
}

bool trFileSystem::DeleteFileDir(const char* file_dir_name)
{
	bool ret = true;

	if (PHYSFS_delete(file_dir_name) != 0)
		TR_LOG("trFilesystem: success on deleting %s\n", file_dir_name);
	else
	{
		ret = false;
		TR_LOG("trFilesystem: could not delete %s: %s\n", file_dir_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	return ret;
}