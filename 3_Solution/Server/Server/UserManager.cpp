#include "UserManager.h"

int UserManager::HandleClientLogger(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int i = 1, IDUser = 0;
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

void UserManager::HandleClientRegister(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
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



void UserManager::HandleClientChangePassword(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
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

void UserManager::HandleClientDeleteAccount(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int IDUser;

    std::getline(stream, token, delimiter);
    std::cout << "ID: ";
    IDUser = std::stoi(token);
    std::cout << IDUser << std::endl;

    resultOperation = DatabaseManagerAccounts::deleteUser(IDUser);
    DatabaseManagerAccounts::disconnect();
}

void UserManager::HandleClientUploadFile(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation, const std::vector<uint8_t>& binaryData)
{
    DatabaseManagerAccounts::connect();
    int size = 0, IDUser = 0;
    std::string filename;
    char temp = 0;

    for (int i = 1; std::getline(stream, token, delimiter); i++)
    {
        switch (i)
        {
        case 1:
            IDUser = std::stoi(token);
            break;

        case 2:
            size = std::stoi(token);
            if (size > 0)
            {
                filename.resize(size);
                stream.read(&filename[0], size);
                bool result1 = DatabaseManagerAccounts::addFile(IDUser, filename);
                bool result2 = FileManager::UploadFile(filename, binaryData);
                resultOperation = result1 && result2;
            }
            break;
        default:
            break;
        }
    }

    DatabaseManagerAccounts::disconnect();
}

std::vector<uint8_t> UserManager::HandleClientDownloadFile(std::string& filename, bool& resultOperation)
{
    std::vector<uint8_t> content = FileManager::DownloadFile(filename);
    return content;
}

void UserManager::HandleClientDeleteFile(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int IDUser = 0;
    std::string filename;
    bool result1 = false, result2 = false;

    for (int i = 1; std::getline(stream, token, delimiter); i++)
    {
        switch (i)
        {
        case 1:
            IDUser = std::stoi(token);
            break;

        case 2:
            filename = token;
            result1 = DatabaseManagerAccounts::deleteFile(IDUser, filename);
            result2 = FileManager::removeFile(filename);
            resultOperation = result1 && result2;
            break;

        default:
            break;
        }
    }
    DatabaseManagerAccounts::disconnect();
}

void UserManager::HandleClientSendFileToAnotherUser(std::stringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int IDUser = 0;

    for (int i = 1; std::getline(stream, token, delimiter); i++)
    {
        switch (i)
        {
        case 1:
            IDUser = DatabaseManagerAccounts::selectUserWithoutPassword(token);//username dorit
            break;

        case 2:
            resultOperation = DatabaseManagerAccounts::UpdateFileID(IDUser, token);//filename-ul 
            break;

        default:
            break;
        }
    }
    DatabaseManagerAccounts::disconnect();
}
