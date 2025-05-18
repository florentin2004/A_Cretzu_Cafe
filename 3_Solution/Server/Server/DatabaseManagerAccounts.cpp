#include "DatabaseManagerAccounts.h"

SQLHENV DatabaseManagerAccounts::hEnv = nullptr;
SQLHDBC DatabaseManagerAccounts::hDbc = nullptr;
SQLHSTMT DatabaseManagerAccounts::hStmt = nullptr;


void DatabaseManagerAccounts::showSQLError(SQLHANDLE handle, SQLSMALLINT type) {
    SQLCHAR sqlState[6], message[512];
    SQLINTEGER nativeError;
    SQLSMALLINT textLength;

    if (SQLGetDiagRecA(type, handle, 1, sqlState, &nativeError, message, sizeof(message), &textLength) == SQL_SUCCESS) {

        std::cerr << "[SQL ERROR] " << message << " (SQLState: " << sqlState << ")";
        //logMessage = "[SQL ERROR] " + message + " (SQLState: " + sqlState + ")"; //vezi cum faci conversia aia
        //std::cerr << logMessage << std::endl;

        //Logger::logAction(logMessage);
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
       /* logMessage = "[SQL DATABASE] Conectarea a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/
        showSQLError(hDbc, SQL_HANDLE_DBC);
        return false;
    }

    /*logMessage = "[SQL DATABASE] Conectat la SQL Server!";

    std::cout << logMessage << std::endl;

    Logger::logAction(logMessage);*/
    return true;
}

int DatabaseManagerAccounts::selectUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "SELECT * FROM Utilizatori WHERE Username = '" + username +"' AND Password = '" + password + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR *)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        /*logMessage = "[SQL DATABASE] SELECT Log in a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/
        showSQLError(hStmt, SQL_HANDLE_STMT);
        return false;
    }

    int IDUser = -1;

    if (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &IDUser, 0, NULL);
    }

    /*logMessage = "[SQL DATABASE] SELECT Log in a mers cu succes!";
    std::cout << logMessage << std::endl;

    Logger::logAction(logMessage);*/

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return IDUser;
}

int DatabaseManagerAccounts::selectUserWithoutPassword(std::string& username)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "SELECT * FROM Utilizatori WHERE Username = " + username;
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO)
    {

       /* logMessage = "[SQL DATABASE] SELECT Username a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/
        showSQLError(hStmt, SQL_HANDLE_STMT);
        return -1;
    }

    int IDUser = -1;

    if (SQLFetch(hStmt) == SQL_SUCCESS)
    {
        SQLGetData(hStmt, 1, SQL_C_SLONG, &IDUser, 0, NULL);
    }

    /*logMessage = "[SQL DATABASE] SELECT Username a mers cu succes!";
    std::cout << logMessage << std::endl;

    Logger::logAction(logMessage);*/

    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    return IDUser;
}

bool DatabaseManagerAccounts::insertUser(std::string& username, std::string& password) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "INSERT INTO Utilizatori (Username, Password) VALUES ('" + username + "', '" + password + "')";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        /*logMessage = "[SQL DATABASE] INSERT User a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        /*logMessage = "[SQL DATABASE] Utilizator adaugat cu succes!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::updatePassword(std::string& username, std::string& newPassword, std::string& oldPassword) {
    

    if (DatabaseManagerAccounts::selectUser(username, oldPassword) == false)
    {
        /*logMessage = "[SQL DATABASE] Username sau parola gresite pentru schimbare de parola!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/
        return false;
    }

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "UPDATE Utilizatori SET Password = '" + newPassword + "' WHERE Username = '" + username + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        /*logMessage = "[SQL DATABASE] UPDATE Parola a esuat!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
       /* logMessage = "[SQL DATABASE] Parola actualizata!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::deleteUser(int& IDUser) {
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "DELETE FROM UserFiles WHERE UserID = " + std::to_string(IDUser);
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    /*if (!SQL_SUCCEEDED(retcode)) {
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }*/

    query = "DELETE FROM Utilizatori WHERE ID = " + std::to_string(IDUser);
    retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
       /* logMessage = "[SQL DATABASE] DELETE User a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        /*logMessage = "[SQL DATABASE] Utilizator sters!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/

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
       /* logMessage = "[SQL DATABASE] INSERT fisier a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {
        /*logMessage = "[SQL DATABASE] Numele fisierului adaugat cu succes!";
        std::cout << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

std::string DatabaseManagerAccounts::selectFiles(int& userID) {

    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    // Pregătește interogarea parametrizată
    std::string query = "SELECT FileName FROM UserFiles WHERE UserID = " + std::to_string(userID);
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return "";
    }

    
    char fileNameBuffer[256]; 
    SQLLEN fileNameLength;
    retcode = SQLBindCol(hStmt, 1, SQL_C_CHAR, fileNameBuffer, sizeof(fileNameBuffer), &fileNameLength);
    if (!SQL_SUCCEEDED(retcode)) {
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return "";
    }

    
    std::vector<std::string> files;
    while ((retcode = SQLFetch(hStmt)) == SQL_SUCCESS) {
        if (fileNameLength == SQL_NULL_DATA) {
            files.push_back(""); // Gestionăm cazul NULL, dacă FileName poate fi NULL
        }
        else {
            files.push_back(std::string(fileNameBuffer, fileNameLength));
        }
    }

    if (retcode != SQL_NO_DATA) {
        std::cerr << "[SQL DATABASE] Eroare la preluarea fișierelor!\n";
        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return "";
    }

    // Eliberează mânerul
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    // Construiește rezultatul: <numberFiles>:file1:file2:...:filen
    std::string result = std::to_string(files.size());
    for (const auto& name : files) {
        result += ":" + name;
    }

    // Logare (opțional)
    std::cout << "[SQL DATABASE] Rezultat: " << result << "\n";

    return result;
}

bool DatabaseManagerAccounts::deleteFile(int& IDUser, std::string& filename)
{
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    std::string query = "DELETE FROM UserFiles WHERE FileName = '" + filename + "' AND UserID = '" + std::to_string(IDUser) + "';";
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);

    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
       /* logMessage = "[SQL DATABASE] DELETE Fisier a esuat!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }
    else {

        /*logMessage = "[SQL DATABASE] Fisier sters din baza de date!";
        std::cerr << logMessage << std::endl;

        Logger::logAction(logMessage);*/

        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return true;
    }
}

bool DatabaseManagerAccounts::UpdateFileID(int newFileID, const std::string& filename) {
    //std::string logMessage;
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);

    // Construiește interogarea UPDATE
    std::string query = "UPDATE UserFiles SET FileID = " + std::to_string(newFileID) +
        " WHERE FileName = '" + filename + "'";

    // Loghează interogarea (opțional, pentru depanare)
    /*logMessage = "[SQL DATABASE] Execută interogare: " + query;
    std::cerr << logMessage << std::endl;
    Logger::logAction(logMessage);*/

    // Execută interogarea direct cu SQLExecDirectA
    SQLRETURN retcode = SQLExecDirectA(hStmt, (SQLCHAR*)query.c_str(), SQL_NTS);
    if (retcode != SQL_SUCCESS && retcode != SQL_SUCCESS_WITH_INFO) {
        /* logMessage = "[SQL DATABASE] INSERT fisier a esuat!";
         std::cerr << logMessage << std::endl;

         Logger::logAction(logMessage);*/

        showSQLError(hStmt, SQL_HANDLE_STMT);
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
        return false;
    }

    // Eliberează mânerul statement-ului
    SQLFreeHandle(SQL_HANDLE_STMT, hStmt);

    /*logMessage = "[INFO] FileID actualizat cu succes: FileName = '" + filename + "', FileID = " + std::to_string(newFileID);
    std::cerr << logMessage << std::endl;
    Logger::logAction(logMessage);*/
    return true;
}

void DatabaseManagerAccounts::disconnect() {
    if (hDbc) {
        SQLDisconnect(hDbc);
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    }
    if (hEnv) {
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
    }

    /*logMessage = "[SQL DATABASE] Deconectat de la SQL SERVER!";
    std::cout << logMessage << std::endl;

    Logger::logAction(logMessage);*/
}
