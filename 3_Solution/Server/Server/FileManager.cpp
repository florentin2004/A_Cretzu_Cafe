#include "FileManager.h"

bool FileManager::UploadFile(std::string& filename, const std::vector<uint8_t>& content)
{
	std::ofstream out(filename, std::ios::binary);
	if (!out.is_open()) {
        /*logMessagefile = "[ERROR] Nu pot deschide fisierul pentru scriere!";
        std::cerr << logMessagefile << std::endl;

        Logger::logAction(logMessagefile);*/
		return false;
	}
	out.write(reinterpret_cast<const char*>(content.data()), content.size());
	out.close();
	return true;
}

std::vector<uint8_t> FileManager::DownloadFile(std::string& filename) {
    std::ifstream in(filename, std::ios::binary);
    if (!in.is_open()) {
        /*logMessagefile = "[ERROR] Nu pot deschide fisierul pentru citire!";
        std::cerr << logMessagefile << std::endl;

        Logger::logAction(logMessagefile);*/
        return {};
    }

    in.seekg(0, std::ios::end);
    std::streamsize len = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<uint8_t> content(len);
    if (!in.read(reinterpret_cast<char*>(content.data()), len)) {
        /*logMessagefile = "[ERROR] Probleme la citirea fisierului!";
        std::cerr << logMessagefile << std::endl;

        Logger::logAction(logMessagefile);*/
        return {};
    }

    in.close();
    return content;
}
