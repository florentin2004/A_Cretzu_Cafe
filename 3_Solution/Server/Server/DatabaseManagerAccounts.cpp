#include "DatabaseManagerAccounts.h"

SQLHENV DatabaseManagerAccounts::hEnv = nullptr;
SQLHDBC DatabaseManagerAccounts::hDbc = nullptr;
SQLHSTMT DatabaseManagerAccounts::hStmt = nullptr;


void DatabaseManagerAccounts::showSQLError(SQLHANDLE handle, SQLSMALLINT type) {
    SQLCHAR sqlState[6], message[512];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;

    if (SQLGetDiagRecA(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength) == SQL_SUCCESS) {
        std::cerr << "[SQL DATABASE] SQL Error: " << message << " (SQLState: " << sqlState << ")" << std::endl;
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
        std::cerr << "[SQL DATABASE] Conectarea a esuat!" << std::endl;
        showSQLError(hDbc, SQL_HANDLE_DBC);
        return false;
    }

    std::cout << "[SQL DATABASE] Conectat la SQL Server!" << std::endl;
    return true;
}

int DatabaseManagerAccounts::selectUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "SELECT * FROM Utilizatori WHERE Username = '" + username +"' AND Password = '" + password + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR *)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] SELECT Log in a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        return false;
    }

    int IDUser = -1;

    if (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &IDUser, 0, NULL);
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return IDUser;
}

bool DatabaseManagerAccounts::insertUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "INSERT INTO Utilizatori (Username, Password) VALUES ('" + username + "', '" + password + "')";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] INSERT User a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "[SQL DATABASE] Utilizator adaugat cu succes!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::updatePassword(std::string& username, std::string& newPassword, std::string& oldPassword) {
    

    if (DatabaseManagerAccounts::selectUser(username, oldPassword) == false)
    {
        std::cerr << "[SQL DATABASE] Username sau parola gresite!" << std::endl;
        return false;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "UPDATE Utilizatori SET Password = '" + newPassword + "' WHERE Username = '" + username + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] UPDATE Parola a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "[SQL DATABASE] Parola actualizata!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::deleteUser(std::string& username) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "DELETE FROM Utilizatori WHERE Username = '" + username + "'";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] DELETE User a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "[SQL DATABASE] Utilizator sters!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::addFile(int& IDUser, std::string& filename)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "INSERT INTO UserFiles (UserID, FileName) VALUES ('" + std::to_string(IDUser) + "', '" + filename + "')";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] INSERT fisier a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "[SQL DATABASE] Numele fisierului adaugat cu succes!" << std::endl;
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

std::string DatabaseManagerAccounts::selectFiles(int& userID) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "SELECT FileName FROM UserFiles WHERE UserID = ?";
    SQLPrepareA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &userID, 0, NULL);

    SQLRETURN ret = SQLExecute(hStmt);
    if (ret != SQL_SUCCESS && ret != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] SELECT fisier a esuat!\n";
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return "";
    }

    char fileName[256];
    SQLBindCol(hStmt, 1, SQL_C_CHAR, fileName, sizeof(fileName), NULL);

    std::vector<std::string> files;
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        files.push_back(std::string(fileName));
    }

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    std::string result = std::to_string(files.size());
    for (const auto& name : files) {
        result += ":" + name;
    }

    return result;
}

bool DatabaseManagerAccounts::deleteFile(int& IDUser, std::string& filename)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "DELETE FROM UserFiles WHERE FileName = '" + filename + "' AND UserID = '" + std::to_string(IDUser) + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        std::cerr << "[SQL DATABASE] DELETE Fisier a esuat!" << std::endl;
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        std::cout << "[SQL DATABASE] Fisier sters!" << std::endl;
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
