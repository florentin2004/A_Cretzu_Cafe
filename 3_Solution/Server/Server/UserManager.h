#pragma once
#include <random>
#include <ctime>
#include "DatabaseManagerAccounts.h"
#include "FileManager.h"
class UserManager
{
private:
	UserManager() = default;
	UserManager(const UserManager&) = default;
	UserManager(UserManager&&) = default;
	~UserManager() = default;
public:
	static int HandleClientLogger(std::stringstream&, std::string&, const char, bool&);
	static void HandleClientRegister(std::stringstream&, std::string&, const char, bool&);
	static void HandleClientChangePassword(std::stringstream&, std::string&, const char, bool&);
	static void HandleClientDeleteAccount(std::stringstream&, std::string&, const char, bool&);
	static void HandleClientUploadFile(std::stringstream&, std::string&, const char, bool&, const std::vector<uint8_t>&);
	static std::vector<uint8_t> HandleClientDownloadFile(std::string&, bool&);
	static void HandleClientDeleteFile(std::stringstream&, std::string&, const char, bool&);
	static void HandleClientSendFileToAnotherUser(std::stringstream&, std::string&, const char, bool&);
};

