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
	static int HandleClientLogger(std::istringstream&, std::string&, const char, bool&);
	static void HandleClientRegister(std::istringstream&, std::string&, const char, bool&);
	static void HandleClientChangePassword(std::istringstream&, std::string&, const char, bool&);
	static void HandleClientDeleteAccount(std::istringstream&, std::string&, const char, bool&);
	static void HandleClientUploadFile(std::istringstream&, std::string&, const char, bool&);
	static std::string* HandleClientDownloadFile(std::istringstream&, std::string&, const char, bool&);
};

