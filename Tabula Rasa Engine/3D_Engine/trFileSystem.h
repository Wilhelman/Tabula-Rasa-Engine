#ifndef __trFILESYSTEM_H__
#define __trFILESYSTEM_H__

#include "trModule.h"
#include <vector>


struct PHYSFS_File;


struct File
{
	File(char* name, int64_t last_modified) 
	{ 
		this->name = name; 
		this->last_modified = last_modified;
	}

	std::string name;
	int64_t last_modified = 0;
};

struct Directory
{
	Directory(char* name) { this->name = name; }

	~Directory() {  }

	std::string name;
	std::vector<Directory> dirs_vec;
	std::vector<File> files_vec;

};

class trFileSystem : public trModule
{
public:
	trFileSystem();
	~trFileSystem();

	bool Awake(JSON_Object* config = nullptr);
	bool Start();
	bool CleanUp();

	bool DoesFileExist(const char* file_name) const;
	bool DoesDirExist(const char* dir_name) const;

	void ClearAssetsDir();
	void RefreshDirectory(const char* dir_name);

	bool WriteInFile(const char* file_name, char* buffer, uint size) const;
	uint ReadFromFile(const char* file_name, char** buffer);

	void GetExtensionFromFile(const char* file_name, std::string& extension);
	
	bool MakeNewDir(const char* dir_name);
	bool DeleteFileDir(const char* file_dir_name);

	int64_t GetLastModifiedTime(const char* file_name) const;

	Directory* GetAssetsDirectory() const;
		
private:

	bool AddNewPath(const char* path);
	PHYSFS_File* OpenFileForWriting(const char* file_name) const;
	PHYSFS_File* OpenFileForReading(const char* file_name) const;
	void CloseFile(PHYSFS_File* file, const char* file_name) const;

public:
	Directory* assets_dir = nullptr;
	Directory* assets_dir_backup = nullptr;
	uint assets_index = 0u;
	bool assets_dir_clear = false;


};

#endif // __trFILESYSTEM_H__