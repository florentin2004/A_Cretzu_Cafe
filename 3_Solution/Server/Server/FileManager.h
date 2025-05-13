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
	static void UploadFile(std::string& filename, std::vector<char>& content);
	static std::string* DownloadFile(std::string& filename);
};

