#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
class DatabaseManagerAccounts
{
private:
    static SQLHENV hEnv;
    static SQLHDBC hDbc;
    static SQLHSTMT hStmt;


    static void showSQLError(SQLHANDLE handle, SQLSMALLINT type);

public:
    DatabaseManagerAccounts() = default;
    static bool connect();
    static int selectUser(std::string& username, std::string& password);
    static bool insertUser(std::string& username, std::string& password);
    static bool updatePassword(std::string& username, std::string& newPassword);
    static bool deleteUser(std::string& username);
    static bool addFile(int& IDUser, std::string& filename);
    static bool deleteFile(int& IDUser, std::string& filename);
    static void disconnect();
    ~DatabaseManagerAccounts() {
        disconnect();
    }
};
