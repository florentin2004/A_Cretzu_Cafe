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
        //WSACleanup();
        return 1;
    }

    int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("listen failed with error: %d\n", WSAGetLastError());
        closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    printf("[SERVER] S-a initializat cu succes!\n");

    return 0;
}

int Retea::AcceptConnection() {
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        printf("accept failed with error: %d\n", WSAGetLastError());
        //closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    //closesocket(ListenSocket);  // No longer need the server socket

    printf("[SERVER] Client conectat!\n");

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


void Retea::HandleClient() {
    int iResult;
    int iSendResult;

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        printf("Bytes received: %d\n", iResult);
        recvbuf[iResult] = '\0';
        if (iResult > 0) {
            int IDUser = -1;
            std::string buffer = recvbuf;
            std::istringstream stream(buffer);
            std::string token;
            bool resultOperation = true;
            std::getline(stream, token, ':');
            if (token == "0")
            {
                std::cout << "Logare: " << std::endl;
                IDUser = UserManager::HandleClientLogger(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Te-ai logat cu succes! (ID-ul tau este " + std::to_string(IDUser) + " )";
                else
                    buffer = "Username sau parola incorecta!";

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "1")
            {
                std::cout << "Inregistrare: " << std::endl;
                UserManager::HandleClientRegister(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Te-ai inregistrat cu succes!";
                else
                    buffer = "A intervenit o eroare, va rog reveniti mai tarziu!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "2")
            {
                std::cout << "Schimbare Parola: " << std::endl;
                UserManager::HandleClientChangePassword(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Parola a fost schimbata cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "3")
            {
                std::cout << "Stergere cont: " << std::endl;
                UserManager::HandleClientDeleteAccount(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Contul a fost sters cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "4")
            {
                std::cout << "Adaugare fisier: " << std::endl;
                UserManager::HandleClientUploadFile(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Fisierul a fost adaugat cu succes!";
                else
                    buffer = "A intervenit o eroare la adaugarea fisierului in server!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "5") 
            {
                std::string* content = nullptr;
                std::cout << "Descarcare fisier : " << std::endl;
                content = UserManager::HandleClientDownloadFile(stream, token, ':', resultOperation);

                if (resultOperation && content)
                {
                    buffer = "Fisierul a fost descarcat cu succes!";
                    iSendResult = send(ClientSocket, (*content).c_str(), (*content).size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("send failed with error: %d\n", WSAGetLastError());
                        //WSACleanup();
                        return;    
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
                else {
                    buffer = "A intervenit o eroare la descarcarea fisierului din server!";
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("send failed with error: %d\n", WSAGetLastError());
                        //WSACleanup();
                        return;
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
                delete content;
            }
            else
            {
                
            }

        }
        else if (iResult == 0)
        {
            printf("[SERVER] S-a deconectat clientul!\n");
            closesocket(ClientSocket);
        }
        else {
            printf("[SERVER] recv failed with error: %d\n", WSAGetLastError());
            //WSACleanup();
            closesocket(ClientSocket);
        }

    } while (iResult > 0);

    
}