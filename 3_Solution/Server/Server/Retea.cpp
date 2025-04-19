#include "Retea.h"

int Retea::Initialize() {
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    return 0;
}

int Retea::CreateSocket() {
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        WSACleanup();
        return 1;
    }

    int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    return 0;
}

int Retea::AcceptConnection() {
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    closesocket(ListenSocket);  // No longer need the server socket
    return 0;
}

void Retea::Shutdown() {
    int iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
    }

    closesocket(ClientSocket);
    WSACleanup();
}

void HandleClientLogger(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    int i = 1;
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
    resultOperation = DatabaseManagerAccounts::selectUser(username, password);
    DatabaseManagerAccounts::disconnect();
}

void HandleClientRegister(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
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

void HandleClientChangePassword(std::istringstream& stream, std::string token, const char delimiter, bool& resultOperation)
{
    DatabaseManagerAccounts::connect();
    std::string username, newPassword;
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
        i++;
    }
    resultOperation = DatabaseManagerAccounts::updatePassword(username, newPassword);
    DatabaseManagerAccounts::disconnect();
}

void HandleClientDeleteAccount(std::istringstream& stream, std::string& token, const char delimiter, bool& resultOperation)
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

void Retea::HandleClient() {
    int iResult;
    int iSendResult;

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        printf("Bytes received: %d\n", iResult);
        recvbuf[iResult] = '\0';
        if (iResult > 0) {
            std::string buffer = recvbuf;
            std::istringstream stream(buffer);
            std::string token;
            bool resultOperation = true;
            std::getline(stream, token, ':');
            if (token == "0")
            {
                std::cout << "Logare: " << std::endl;
                HandleClientLogger(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Te-ai logat cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "1")
            {
                std::cout << "Inregistrare: " << std::endl;
                HandleClientRegister(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Te-ai inregistrat cu succes!";
                else
                    buffer = "A intervenit o eroare, va rog reveniti mai tarziu!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "2")
            {
                std::cout << "Schimbare Parola: " << std::endl;
                HandleClientChangePassword(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Parola a fost schimbata cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "3")
            {
                std::cout << "Stergere cont: " << std::endl;
                HandleClientDeleteAccount(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Contul a fost sters cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(ClientSocket);
                    WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }

        }
        else if (iResult == 0)
            printf("Connection closing...\n");
        else {
            printf("recv failed with error: %d\n", WSAGetLastError());
            closesocket(ClientSocket);
            WSACleanup();
            return;
        }

    } while (iResult > 0);

    Shutdown();
}