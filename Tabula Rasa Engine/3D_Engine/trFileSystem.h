#ifndef __trFILESYSTEM_H__
#define __trFILESYSTEM_H__

#define REFRESH_TIME 2.0f

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
	std::string path;
	int64_t last_modified = 0;
};

struct Directory
{
	Directory() { }
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
	bool Update(float dt);
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

	File GetFileByName(const char* file_name);

	int GetLastModifiedTime(const char* file_name) const;
	Directory* GetAssetsDirectory() const;

	bool CopyFileFrom(const char* src_file_path);
		
private:

	bool AddNewPath(const char* path, char* mount = nullptr);

	PHYSFS_File* OpenFileForWriting(const char* file_name) const;
	PHYSFS_File* OpenFileForReading(const char* file_name) const;

	void CloseFile(PHYSFS_File* file, const char* file_name) const;

	void GetDirectoryFiles(Directory* dir_to_compare, std::vector<File>& compare_files_vec);


private:
	float refresh_clock = 0;
	uint assets_index = 0u;
	Directory* assets_dir = nullptr;
	Directory assets_dir_backup;
};

#endif // __trFILESYSTEM_H__