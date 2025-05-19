#include "FileManager.h"

std::string FileManager::logMessage;

bool FileManager::UploadFile(std::string& filename, const std::vector<uint8_t>& content, int& IDUser)
{
    if (CreateDirectoryA(std::to_string(IDUser).c_str(), NULL) || GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Folderul a fost creat sau deja exista.\n";
    }
    else {
        std::cerr << "Eroare la crearea folderului. Cod eroare: " << GetLastError() << "\n";
    }

	std::ofstream out(std::to_string(IDUser) + "\\" + filename, std::ios::binary);
	if (!out.is_open()) {
        FileManager::logMessage = "[ERROR] Nu pot deschide fisierul pentru scriere!";
        std::cerr << FileManager::logMessage << std::endl;

        Logger::logAction(FileManager::logMessage);
		return false;
	}
	out.write(reinterpret_cast<const char*>(content.data()), content.size());
	out.close();
	return true;
}

std::vector<uint8_t> FileManager::DownloadFile(std::string& filename, int& IDUser) {


    std::ifstream in(std::to_string(IDUser) + "\\" + filename, std::ios::binary);
    if (!in.is_open()) {
        logMessage = "[ERROR] Nu pot deschide fisierul pentru citire!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);
        return {};
    }

    in.seekg(0, std::ios::end);
    std::streamsize len = in.tellg();
    in.seekg(0, std::ios::beg);

    std::vector<uint8_t> content(len);
    if (!in.read(reinterpret_cast<char*>(content.data()), len)) {
        logMessage = "[ERROR] Probleme la citirea fisierului!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);
        return {};
    }

    in.close();

    /*if (std::remove(filename.c_str()) != 0) {
        logMessage = "[EROARE] Eșec la ștergerea fișierului: " + filename;
        std::cerr << logMessage << std::endl;
        Logger::logAction(logMessage);
        return {};
    }*/
    return content;
}

bool FileManager::removeFile(std::string& filename)
{
    if (std::remove(filename.c_str()) != 0) {
        logMessage = "[EROARE] Eroare la stergerea fisierului: " + filename;
        std::cerr << logMessage << std::endl;
        Logger::logAction(logMessage);
        return false;
    }
    return true;
}

bool FileManager::changeDirectoryFile(int& IDUserSource, int& IDUserDestination, std::string& filename)
{
    std::string source = std::to_string(IDUserSource) + "\\" + filename;
    std::string destination = std::to_string(IDUserDestination) + "\\" + filename;

    if (MoveFileA(source.c_str(), destination.c_str())) {
        std::cout << "Fisierul a fost mutat cu succes!\n";
        return true;
    }
    else {
        logMessage = "[EROARE] Probleme la mutarea fisierului. Cod eroare: " + GetLastError();
        std::cerr << logMessage << std::endl;
        Logger::logAction(logMessage); 
        return false;
    }
}
