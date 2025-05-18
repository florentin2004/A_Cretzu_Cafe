#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <string>
#include <vector>
#include "Logger.h"
class DatabaseManagerAccounts
{
private:
    static SQLHENV hEnv;
    static SQLHDBC hDbc;
    static SQLHSTMT hStmt;

    //static std::string logMessage;

    static void showSQLError(SQLHANDLE handle, SQLSMALLINT type);

public:
    DatabaseManagerAccounts() = default;
    static bool connect();
    static int selectUser(std::string& username, std::string& password);
    static int selectUserWithoutPassword(std::string& username);
    static bool insertUser(std::string& username, std::string& password);
    static bool updatePassword(std::string& username, std::string& newPassword, std::string& oldPassword);
    static bool deleteUser(int& IDUser);
    static bool addFile(int& IDUser, std::string& filename);
    static std::string selectFiles(int& IDUser);
    static bool deleteFile(int& IDUser, std::string& filename);
    static bool UpdateFileID(int newFileID, const std::string& filename);
    static void disconnect();
    ~DatabaseManagerAccounts() {
        disconnect();
    }
};
