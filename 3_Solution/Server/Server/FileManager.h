#pragma once
#include <sstream>
#include <vector>
#include <windows.h>
#include "Logger.h"
class FileManager
{
private:
	FileManager() = default;
	FileManager(const FileManager&) = default;
	FileManager(FileManager&&) = default;
	~FileManager() = default;
	static std::string logMessage;
public:
	static bool UploadFile(std::string& filename, const std::vector<uint8_t>& content, int& IDUser);
	static std::vector<uint8_t> DownloadFile(std::string& filename, int& IDUser);
	static bool removeFile(std::string& filename);
	static bool changeDirectoryFile(int& IDUserSource, int& IDUserDestination, std::string& filename);
};

