#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
class FileManager
{
private:
	FileManager() = default;
	FileManager(const FileManager&) = default;
	FileManager(FileManager&&) = default;
	~FileManager() = default;
public:
	static bool UploadFile(std::string& filename, const std::vector<uint8_t>& content);
	static std::vector<uint8_t> DownloadFile(std::string& filename);
};

