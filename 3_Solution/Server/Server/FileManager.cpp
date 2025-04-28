#include "FileManager.h"

void FileManager::UploadFile(std::string& filename, std::string& content)
{
	std::ofstream out(filename + ".txt", std::ios::binary);
	if (!out.is_open()) {
		std::cerr << "Eroare: nu pot deschide fisierul pentru scriere!\n";
	}
	out.write(content.data(), content.size());
	out.close();
}

std::string* FileManager::DownloadFile(std::string& filename) // returneaza contentul fisierului
{
	std::ifstream in(filename + ".txt", std::ios::binary);
	if (!in.is_open())
	{
		std::cerr << "Eroare: nu pot deschide fisierul pentru citire!\n";
		return nullptr;
	}

	std::string *content = new std::string();

	in.seekg(0, std::ios::end);
	int len = in.tellg();
	in.seekg(0, std::ios::beg);

	(*content).resize(len);

	in.read(&(*content)[0], len);
	in.close();

	return content;
}
