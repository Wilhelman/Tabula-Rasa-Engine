#ifndef __trFILESYSTEM_H__
#define __trFILESYSTEM_H__

#include "trModule.h"
#include <vector>


struct PHYSFS_File;

struct Directory
{
	Directory(char* name) { this->name = name; }

	~Directory() {  }

	void Clear()
	{
		name = nullptr;
		dirs_vec.clear();
		dirs_vec.clear();
		files_vec.clear();
	}

	std::string name;
	std::vector<Directory> dirs_vec;
	std::vector<std::string> files_vec;
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

	void RefreshAssets(const char* dir_name) const;

	bool WriteInFile(const char* file_name, char* buffer, uint size) const;
	uint ReadFromFile(const char* file_name, char** buffer);

	void GetExtensionFromFile(const char* file_name, std::string& extension);
	
	bool MakeNewDir(const char* dir_name);
	bool DeleteFileDir(const char* file_dir_name);

	Directory* GetAssetsDirectory() const;
		
private:

	bool AddNewPath(const char* path);
	PHYSFS_File* OpenFileForWriting(const char* file_name) const;
	PHYSFS_File* OpenFileForReading(const char* file_name) const;
	void CloseFile(PHYSFS_File* file, const char* file_name) const;

private:
	Directory* assets_dir = nullptr;


};

#endif // __trFILESYSTEM_H__