#pragma once
#include <sstream>
#include <vector>
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
	static bool UploadFile(std::string& filename, const std::vector<uint8_t>& content);
	static std::vector<uint8_t> DownloadFile(std::string& filename);
	static bool removeFile(std::string& filename);
};

