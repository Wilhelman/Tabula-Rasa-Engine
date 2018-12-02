#include "trApp.h"
#include "trFileSystem.h"
#include "trLog.h"
#include "trTimeManager.h"
#include "PhysFS/include/physfs.h"
#include "trResources.h"

#include <fstream>  

#pragma comment( lib, "PhysFS/libx86/physfs.lib" )

trFileSystem::trFileSystem()
{
	this->name = "FileSystem";
}

trFileSystem::~trFileSystem() { RELEASE(assets_dir); }

bool trFileSystem::Awake(JSON_Object * config)
{
	// Initializing PhysFS library
	TR_LOG("trFileSystem: initializing PhysFS library...\n");

	bool ret = true;

	if (PHYSFS_init(nullptr) != 0)
		TR_LOG("trFileSystem: success on initializing PhysFS \n");
	else
	{
		ret = false;
		TR_LOG("trFileSystem: error initializing PhysFS: %s \n", PHYSFS_getLastError());
	}	

	TR_LOG("trFileSystem: setting search directories...\n");


	// Setting search directories
	AddNewPath(".");
	AddNewPath("./Assets/", "Assets");
	AddNewPath("./Assets/Models/", "Models");

	AddNewPath("./Library/", "Library");
	AddNewPath("./Settings/", "Settings");

	// Setting game directory to write in it
	PHYSFS_setWriteDir(".");

	assets_dir = new Directory(ASSETS_DIR);

	RefreshDirectory(ASSETS_DIR);

	return true;
}

bool trFileSystem::Start()
{
	refresh_clock = REFRESH_TIME;
	return true;
}

bool trFileSystem::Update(float dt)
{
	if (refresh_clock >= REFRESH_TIME)
	{
		// Storing assets before refresh
		assets_dir_backup = *assets_dir;

		// Refreshing assets
		ClearAssetsDir();
		RefreshDirectory(ASSETS_DIR);

		// Getting current refreshed assets files
		std::vector<File> assets_files;
		GetDirectoryFiles(assets_dir, assets_files);

		// Getting assets files before refresh
		std::vector<File> assets_backup_files;
		GetDirectoryFiles(&assets_dir_backup, assets_backup_files);
	
		bool file_found = false;

		// Checking assets before refresh against current refreshed assets
		for (uint i = 0u; i < assets_backup_files.size(); i++)
		{
			for (uint j = 0u; j < assets_files.size(); j++)
			{
				if (assets_backup_files[i].name.compare(assets_files[j].name) == 0)
				{
					// Case 1: asset has been modified
					if (assets_backup_files[i].last_modified != assets_files[j].last_modified) 
					{
						//App->resources->CheckForChangesInAssets(assets_dir);
						// TODO: send event that file 'assets_files[j]' has been modified
					}

					file_found = true;
					break;
				}

				// TODO: maybe implement the following cases
					// Case 5: asset has been renamed
					// Case 5: asset has been moved 		
			}

			// Case 2: asset has been removed
			if (!file_found)
			{
				int a = 0;
				// TODO: send event that file 'assets_backup_files[i]' has been removed
			}

			file_found = false;
		}
		
		// Checking current refreshed assets against assets before refresh
		for (uint i = 0u; i < assets_files.size(); i++)
		{
			for (uint j = 0u; j < assets_backup_files.size(); j++)
			{
				if (assets_files[i].name.compare(assets_backup_files[j].name) == 0)
				{
					file_found = true;
					break;
				}
			}

			// Case 3: asset has been added / renamed
			if (!file_found)
			{
				App->resources->CheckForChangesInAssets(assets_dir);
				// TODO: send event that file 'assets_files[i]' has been added / renamed
			}

			file_found = false;
		}

		refresh_clock = 0.0f;
	}

	refresh_clock += App->time_manager->GetRealTimeDt();

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

bool trFileSystem::DoesDirExist(const char * dir_name) const
{
	bool ret = true;

	if (PHYSFS_isDirectory(dir_name) != 0) {
		//TR_LOG("trFileSystem: directory %s found\n", dir_name);
	}
	else
	{
		ret = false;
		//TR_LOG("trFileSystem: directory %s not found\n", dir_name);
	}

	return ret;
}

void trFileSystem::ClearAssetsDir()
{
	assets_dir->files_vec.clear();
	assets_dir->dirs_vec.clear();
	assets_index = 0u;
}

void trFileSystem::RefreshDirectory(const char* dir_name)
{
	std::string assets_name(dir_name);
	assets_name.append("/");
	char **rc = PHYSFS_enumerateFiles(assets_name.c_str());

	if (rc == nullptr)
	{
		//TR_LOG("Directory %s not found", dir_name);
		return;
	}
	else
	{
		std::string directory = assets_name.c_str();

		for (char** i = rc; *i != nullptr; i++)
		{
			if (DoesDirExist((directory + *i).c_str()))
			{
				std::string tmp_dir = assets_name;
				tmp_dir.append(*i);

				Directory new_dir(*i);
				assets_dir->dirs_vec.push_back(new_dir);

				RefreshDirectory(tmp_dir.c_str());
			
				assets_index++;
			}
			else
			{
				File new_file(*i, PHYSFS_getLastModTime((directory + *i).c_str()));
				new_file.path = directory;

				if (directory.compare("Assets/") == 0)
					assets_dir->files_vec.push_back(new_file);
				else
					assets_dir->dirs_vec[assets_index].files_vec.push_back(new_file);
			}
			
		}
	} 
	PHYSFS_freeList(rc);
}


bool trFileSystem::AddNewPath(const char * path, char* mount)
{
	bool ret = false;

	if (PHYSFS_mount(path, mount, 1) != 0)
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

void trFileSystem::GetDirectoryFiles(Directory * dir_to_compare, std::vector<File>& compare_files_vec)
{
	for (uint i = 0u; i < dir_to_compare->files_vec.size(); i++)
		compare_files_vec.push_back(dir_to_compare->files_vec[i]);

	for (uint j = 0u; j < dir_to_compare->dirs_vec.size(); j++)
		GetDirectoryFiles(&dir_to_compare->dirs_vec[j], compare_files_vec);
}

bool trFileSystem::CopyFileFrom(const char* src_file_path)
{
	bool ret = false;

	// Opening file for reading
	std::ifstream output_file(src_file_path, std::ifstream::binary);

	if (output_file)
	{
		ret = true;

		output_file.seekg(0, output_file.end);
		int length = output_file.tellg();
		output_file.seekg(0, output_file.beg);

		// Reading file
		char* buffer = new char[length];
		output_file.read(buffer, length);

		if (buffer != nullptr)
		{
			output_file.close();

			// Getting file name
			std::string file_name(src_file_path);
			const size_t last_slash = file_name.find_last_of("\\/");

			if (std::string::npos != last_slash)
				file_name.erase(0, last_slash + 1);

			// Getting file extension
			std::string extension;
			GetExtensionFromFile(file_name.c_str(), extension);

			// Checking if file already exists in assets
			std::vector<File> files_vec;
			GetDirectoryFiles(assets_dir, files_vec);

			bool existing_file = false;
			bool overwrite_file = false;

			for (uint i = 0; i < files_vec.size(); i++)
			{
				if (files_vec[i].name.compare(file_name) == 0)
				{
					existing_file = true;
					break;
				}
			}

			// If file exists asking users if they want to overwrite it
			if (existing_file)
			{
				// TODO: pop-up warning of overwritting file
				// change overwrite_file var depending on user answer

				// just for now we overwrite it by default
				overwrite_file = true;
			}
			
			// If file does not exist or if it does but users want to overwrite it we generate the file
			if (!existing_file || overwrite_file)
			{
				// Checking file type for copying in correct folder
				bool accepted_file = false;
				std::string new_path;

				if (extension.compare(".fbx") == 0 || extension.compare(".FBX") == 0)
				{
					accepted_file = true;
					new_path = A_MODELS_DIR;
				}
				else if (extension.compare(".png") == 0 || extension.compare(".PNG") == 0 ||
					extension.compare(".jpg") == 0 || extension.compare(".JPG") == 0 ||
					extension.compare(".jpeg") == 0 || extension.compare(".JPEG") == 0 ||
					extension.compare(".dds") == 0 || extension.compare(".DDS") == 0 ||
					extension.compare(".tga") == 0 || extension.compare(".TGA") == 0)
				{
					accepted_file = true;
					new_path = A_TEXTURES_DIR;
				}

				// If file format is accepted we copy the file into assets
				if (accepted_file)
				{
					new_path.append("/");
					new_path.append(file_name.c_str());
					WriteInFile(new_path.c_str(), buffer, length);
					// TODO: call App->resources->CheckForChangesInAssets(assets_dir);
				}
				else
				{
					// TODO: pop-up of format error in file
				}
			}
		}
		else
		{
			output_file.close();
			ret = false;
		}
			
		delete[] buffer;
	}

	return ret;
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

uint trFileSystem::ReadFromFile(const char* file_name, char** buffer)
{
	uint size = 0u;
	PHYSFS_File* file = OpenFileForReading(file_name);

	if (file != nullptr)
	{
		size = PHYSFS_fileLength(file);

		*buffer = new char[size];

		if (PHYSFS_readBytes(file, *buffer, size) == -1)
		{
			size = 0u;
			TR_LOG("trFileSystem: could not read from file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
		}
		else
			TR_LOG("trFileSystem: success on reading from file %s: %s\n", file_name, PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
	}

	CloseFile(file, file_name);

	return size;
}

void trFileSystem::GetFileFileNameFromPath(const char* file_path, std::string& file_name)
{
	std::string tmp = file_path;

	const size_t last_slash = tmp.find_last_of("\\/");

	if (std::string::npos != last_slash)
		tmp.erase(0, last_slash + 1);

	const size_t extension = tmp.rfind('.');

	if (std::string::npos != extension)
		tmp.erase(extension);

	file_name = tmp;
}

void trFileSystem::GetExtensionFromFile(const char* file_name, std::string & extension)
{
	extension = file_name;

	uint point = extension.find_last_of(".");
	if (point != std::string::npos)
		extension = extension.substr(point);
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

File trFileSystem::GetFileByName(const char* file_name)
{
	std::vector<File> files_vec;
	GetDirectoryFiles(assets_dir, files_vec);

	for (uint i = 0; i < files_vec.size(); i++)
	{
		if (files_vec[i].name.compare(file_name) == 0)
			return files_vec[i];
	}

	File error_file("error", -1);
	return error_file;
}

int trFileSystem::GetLastModifiedTime(const char* file_name) const
{
	return PHYSFS_getLastModTime(file_name);
}

Directory* trFileSystem::GetAssetsDirectory() const
{
	return assets_dir;
}