#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
class FileManager
{
private:
	FileManager() = default;
	FileManager(const FileManager&) = default;
	FileManager(FileManager&&) = default;
	~FileManager() = default;
public:
	static void UploadFile(std::string& filename, std::string& content);
	static std::string* DownloadFile(std::string& filename);
};

