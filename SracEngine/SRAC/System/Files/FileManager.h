#pragma once

#include "Core/StringBuffers.h"
#include "Core/BasicString.h"

class FileManager
{
public:
	enum Folder
	{
		None,
		Root,

		PreLoadFiles,

		Maps,

		Images,
		Image_UI,
		Image_Maps,
		Image_Weapons,
		Image_Animations,

		Font,

		Configs,
		Config_Menus,
		Config_Popups,
		Config_Objects,
		Config_MapObjects,
		Config_Enemies,
		Config_Player,
		Config_Weapons,
		Config_MeleeWeapons,
		Config_Abilities,

		Audio,
		Audio_Music,
		Audio_Sound,
		Audio_SoundGroups,

		Count
	};


public:
	static FileManager* Get();

	void init();
	void free();

	BasicString folderPath(const Folder folder) const;

	bool exists(const Folder folder, const char* name) const;
	BasicString findFile(const Folder folder, const char* name) const;
	BasicString findFile(const Folder folder, StringBuffer32 name) const { findFile(folder, name.c_str()); }
	BasicString findFileEtx(const Folder folder, const char* name) const;

	// todo: dont return these vectors, fill some in instead, same with allFilesinFolder functions
	std::vector<BasicString> fullPathsInFolder(const Folder folder) const;
	std::vector<BasicString> fullPathsInFolder(const char* directory) const;
	std::vector<StringBuffer32> fileNamesInFolder(const Folder folder) const;

	StringBuffer32 getItemName(const char* filePath) const;
	StringBuffer32 getItemName(const fs::path& filePath) const;
	StringBuffer32 getItemNameAndExt(const fs::path& filePath) const;

	std::vector<BasicString> allFilesInFolder(const Folder folder) const;
	std::vector<BasicString> allFilesInFolder(const fs::path& directory) const;

	void AllFolders(const fs::path& directoryPath, std::unordered_set<BasicString>& folderList) const;
	void AllFoldersContainingFiles(const fs::path& directoryPath, std::unordered_set<BasicString>& folderList) const;
	std::vector<BasicString> foldersInFolder(const Folder folder) const;

	Folder getFolderIndex(const char* directory);

	static bool HasExt(const char* filePath, const char* extension);

private:
	fs::path fsPath(const Folder folder) const;

	void addFilesToList(std::vector<BasicString>& fileList, const fs::path& directoryPath) const;
	void addFoldersToList(std::vector<BasicString>& folderList, const fs::path& directoryPath) const;

	bool containsFile(const fs::path& directoryPath) const;
	bool containsDirectory(const fs::path& directoryPath) const;

	void outFolderPath(BasicString& outValue, const fs::path& directoryPath, const StringBuffer32& name) const;
	void outFilePath(BasicString& outValue, const fs::path& directoryPath, const StringBuffer32& name) const;


private:
	FileManager() { };
	~FileManager() { };


private:
	std::unordered_map<Folder, BasicString> folderPaths;
};

BasicString pathToString(const fs::path& path);
