#include "Retea.h"

int Retea::Initialize() {
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("[EROARE] WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("[EROARE] getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }
    
    return 0;
}

int Retea::CreateSocket() {
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        printf("[EROARE] socket failed with error: %ld\n", WSAGetLastError());
        freeaddrinfo(result);
        //WSACleanup();
        return 1;
    }

    int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        printf("[EROARE] bind failed with error: %d\n", WSAGetLastError());
        freeaddrinfo(result);
        closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        printf("[EROARE] listen failed with error: %d\n", WSAGetLastError());
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
        printf("[EROARE] accept failed with error: %d\n", WSAGetLastError());
        //closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    //closesocket(ListenSocket);  // No longer need the server socket

    printf("[SERVER] Client conectat!\n");
    this->on = true;

    return 0;
}

void Retea::Shutdown() {
    int iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("[EROARE] shutdown failed with error: %d\n", WSAGetLastError());
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
            std::string token;
            bool resultOperation = true;
            std::istringstream stream(buffer);
            std::getline(stream, token, ':');
            if (token == "0")
            {
                
                std::cout << "[REQUEST TYPE] Logare: " << std::endl;
                IDUser = UserManager::HandleClientLogger(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = std::to_string(IDUser) + ":Te-ai logat cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "1")
            {

                 

                std::cout << "[REQUEST TYPE] Inregistrare: " << std::endl;
                UserManager::HandleClientRegister(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Te-ai inregistrat cu succes!";
                else
                    buffer = "A intervenit o eroare, va rog reveniti mai tarziu!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "2")
            {
                 

                std::cout << "[REQUEST TYPE] Schimbare Parola: " << std::endl;
                UserManager::HandleClientChangePassword(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Parola a fost schimbata cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "3")
            {
                 

                std::cout << "[REQUEST TYPE] Stergere cont: " << std::endl;
                UserManager::HandleClientDeleteAccount(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Contul a fost sters cu succes!";
                else
                    buffer = "Username sau parola incorecta!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "4")
            {
                int totalReceived = 0, receiving = iResult;
                std::getline(stream, token, ':');
                totalReceived = std::stoi(token);


                if (receiving<totalReceived)
                    do {
                        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); // -1 pentru loc de '\0'

                        recvbuf[iResult] = '\0'; // acum e sigur
                        buffer += recvbuf;
                        receiving += iResult;
                        
                    } while (receiving < totalReceived);
                

                 

                std::cout << "[REQUEST TYPE] Adaugare fisier: " << std::endl;
                UserManager::HandleClientUploadFile(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "Fisierul a fost adaugat cu succes!";
                else
                    buffer = "A intervenit o eroare la adaugarea fisierului in server!";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (token == "5") 
            {
                std::string* content = nullptr;
                std::cout << "[REQUEST TYPE] Descarcare fisier : " << std::endl;

                content = UserManager::HandleClientDownloadFile(stream, token, ':', resultOperation);

                if (resultOperation && content)
                {
                    buffer = "Fisierul a fost descarcat cu succes!";
                    iSendResult = send(ClientSocket, (*content).c_str(), (*content).size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                        //WSACleanup();
                        return;    
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
                else {
                    buffer = "A intervenit o eroare la descarcarea fisierului din server!";
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                        //WSACleanup();
                        return;
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
                delete content;
            }
            else if (token == "q")
            {
                printf("[SERVER] S-a deconectat clientul!\n");
                closesocket(ClientSocket);
            }
            else
            {
                std::cout << "[PROTOCOL] Protocol neidentificat, informatie aruncata!" << std::endl;
                buffer = "Protocol neidentificat";
                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }

        }
        else if (iResult == 0)
        {
            printf("[SERVER] S-a deconectat clientul!\n");
            closesocket(ClientSocket);
        }
        else {
            int errorCode = WSAGetLastError();
            if (errorCode == WSAECONNRESET) {
                std::cout << "[SERVER] S-a deconectat clientul!\n";
                closesocket(ClientSocket);
            }
            else {
                std::cerr << "[EROARE] recv failed with error: " << errorCode << std::endl;
                closesocket(ClientSocket);
            }
        }

    } while (iResult > 0);

    
}