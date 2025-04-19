#pragma once

#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include <iostream>
#include <string>
#include <sstream>
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
    static bool selectUser(std::string& username, std::string& password);
    static bool insertUser(std::string& username, std::string& password);
    static bool updatePassword(std::string& username, std::string& newPassword);
    static bool deleteUser(std::string& username);
    static void disconnect();
    ~DatabaseManagerAccounts() {
        disconnect();
    }
};
