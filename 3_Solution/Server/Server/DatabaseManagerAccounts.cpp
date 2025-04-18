﻿#include "DatabaseManagerAccounts.h"

SQLHENV DatabaseManagerAccounts::hEnv = nullptr;
SQLHDBC DatabaseManagerAccounts::hDbc = nullptr;
SQLHSTMT DatabaseManagerAccounts::hStmt = nullptr;

void DatabaseManagerAccounts::showSQLError(SQLHANDLE handle, SQLSMALLINT type) {
    SQLCHAR sqlState[6], message[512];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;

    if (SQLGetDiagRecA(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength) == SQL_SUCCESS) {
        std::cerr << "SQL Error: " << message << " (SQLState: " << sqlState << ")" << std::endl;
    }
}

bool DatabaseManagerAccounts::connect() {
    SQLRETURN retcode;

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &DatabaseManagerAccounts::hEnv);
    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0);
    SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);

    SQLCHAR connectionString[] = "DRIVER={ODBC Driver 18 for SQL Server};SERVER=DESKTOP-NIMPP4E\\SQLEXPRESS;DATABASE=Cretzu_Cafe;Trusted_Connection=Yes;TrustServerCertificate=Yes;";
    retcode = SQLDriverConnectA(hDbc, NULL, connectionString, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "Conectarea a esuat!" << std::endl;
        showSQLError(hDbc, SQL_HANDLE_DBC);
        return false;
    }

    std::cout << "Conectat la SQL Server!" << std::endl;
    return true;
}

bool DatabaseManagerAccounts::selectUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "SELECT COUNT(*) FROM Utilizatori WHERE Username = '" + username +"' AND Password = '" + password + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR *)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "SELECT a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        return false;
    }

    int count = 0;

    if (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &count, 0, NULL);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return (count > 0);
}

bool DatabaseManagerAccounts::insertUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "INSERT INTO Utilizatori (Username, Password) VALUES ('" + username + "', '" + password + "')";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "INSERT a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "Utilizator adaugat cu succes!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::updatePassword(std::string& username, std::string& newPassword) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "UPDATE Utilizatori SET Password = '" + newPassword + "' WHERE Username = '" + username + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "UPDATE a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "Parola actualizata!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::deleteUser(std::string& username) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "DELETE FROM Utilizatori WHERE Username = '" + username + "'";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "DELETE a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "Utilizator sters!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

void DatabaseManagerAccounts::disconnect() {
    if (hDbc) {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }
    if (hEnv) {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }
}
