#include "UserManager.h"

//std::string generateSymmetricKey(size_t length) {
//    const char minChar = 32;  // ' '
//    const char maxChar = 126; // '~'
//    std::string key;
//    key.reserve(length);
//
//    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr))); // Mersenne Twister RNG
//    std::uniform_int_distribution<int> dist(minChar, maxChar);
//
//    for (size_t i = 0; i < length; ++i) {
//        key += static_cast<char>(dist(rng));
//    }
//
//    return key;
//}

int UserManager::HandleClientLogger(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int i = 1, IDUser;
    std::string username, password;
    while(std::getline(stream, token, delimiter))
    {
        if (i == 1)
        {
           std::cout << "Username: ";
           username = token;
           std::cout << username << std::endl;

        }
        else if (i == 2)
        {
           std::cout << "Parola: ";
           password = token;
           std::cout << password << std::endl;
        }
        i++;
    }
    IDUser = DatabaseManagerAccounts::selectUser(username, password);
    DatabaseManagerAccounts::disconnect();

    if (IDUser >= 1)
        resultOperation = true;
    else resultOperation = false;

    return IDUser;
}

void UserManager::HandleClientRegister(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int i = 1;
    std::string username, password;
    while (std::getline(stream, token, delimiter))
    {
        if (i == 1)
        {
            std::cout << "Username: ";
            username = token;
            std::cout << username << std::endl;

        }
        else if (i == 2)
        {
            std::cout << "Parola: ";
            password = token;
            std::cout << password << std::endl;
        }
        i++;
    }
    resultOperation = DatabaseManagerAccounts::insertUser(username, password);
    DatabaseManagerAccounts::disconnect();
}



void UserManager::HandleClientChangePassword(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    std::string username, newPassword, oldPassword;
    int i = 1;
    while (std::getline(stream, token, delimiter))
    {
        if (i == 1)
        {
            std::cout << "Username: ";
            username = token;
            std::cout << username << std::endl;

        }
        else if (i == 2)
        {
            std::cout << "Noua parola: ";
            newPassword = token;
            std::cout << newPassword << std::endl;
        }
        else if (i == 3)
        {
            std::cout << "Vechea parola: ";
            oldPassword = token;
            std::cout << oldPassword << std::endl;
        }
        i++;
    }
    resultOperation = DatabaseManagerAccounts::updatePassword(username, newPassword, oldPassword);
    DatabaseManagerAccounts::disconnect();
}

void UserManager::HandleClientDeleteAccount(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    std::string username;

    std::getline(stream, token, delimiter);
    std::cout << "Username: ";
    username = token;
    std::cout << username << std::endl;

    resultOperation = DatabaseManagerAccounts::deleteUser(username);
    DatabaseManagerAccounts::disconnect();
}

void UserManager::HandleClientUploadFile(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int size, IDUser;
    std::string filename;
    std::vector<char> buffer;
    for (int i = 1; std::getline(stream, token, delimiter); i++)
    {
        switch (i)
        {
        case 1:
            IDUser = std::stoi(token);
            break;
        case 2:
            size = std::stoi(token);
            filename.resize(size);
            stream.read(&filename[0], size);
            resultOperation = DatabaseManagerAccounts::addFile(IDUser, filename);
            stream.read(&buffer[0], 1); //aruncam delimitatorul de la finalul numelui fisierului
            break;
        case 3:
            size = std::stoi(token);
            buffer.resize(size);
            stream.read(buffer.data(), size);
            FileManager::UploadFile(filename, buffer);
            break;
        default:
            break;
        }
    }
    DatabaseManagerAccounts::disconnect();
}

std::string* UserManager::HandleClientDownloadFile(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int size, IDUser;
    std::string filename;
    std::string* content;
    for (int i = 1; std::getline(stream, token, delimiter); i++)
    {
        switch (i)
        {
        case 1:
            IDUser = std::stoi(token);
            break;
        case 2:
            size = std::stoi(token);
            filename.resize(size);
            stream.read(&filename[0], size);
            resultOperation = DatabaseManagerAccounts::deleteFile(IDUser, filename);
            break;
        default:
            break;
        }
    }
    content = FileManager::DownloadFile(filename);
    DatabaseManagerAccounts::disconnect();
    return content;
}
