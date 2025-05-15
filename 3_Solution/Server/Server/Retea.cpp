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
    char temp;

    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        printf("Bytes received: %d\n", iResult);
        recvbuf[iResult] = '\0';
        if (iResult > 0) {
            int IDUser = -1;
            std::string buffer = recvbuf;
            std::string token;
            bool resultOperation = true;
            std::stringstream stream(buffer);
            std::getline(stream, token, ':');
            if (buffer[0] == '0')//era token == "0"
            {
                
                std::cout << "[REQUEST TYPE] Logare: " << std::endl;
                IDUser = UserManager::HandleClientLogger(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "1:" + std::to_string(IDUser);
                else
                    buffer = "0";

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '1')
            {

                std::cout << "[REQUEST TYPE] Inregistrare: " << std::endl;
                UserManager::HandleClientRegister(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "1";
                else
                    buffer = "0";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '2')
            {

                std::cout << "[REQUEST TYPE] Schimbare Parola: " << std::endl;
                UserManager::HandleClientChangePassword(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "1";
                else
                    buffer = "0";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '3')
            {

                std::cout << "[REQUEST TYPE] Stergere cont: " << std::endl;
                UserManager::HandleClientDeleteAccount(stream, token, ':', resultOperation);

                if (resultOperation)
                    buffer = "1";
                else
                    buffer = "0";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '4')
            {

                int totalReceived = 0, receiving = 0;
                std::getline(stream, token, ':');
                totalReceived = std::stoi(token);

                std::vector<uint8_t> binaryData;

                //aici dupa cererea de upload, clientul pune in protocol lungimea contentului fisierului ca sa pun atatia octeti de date si dupa sa le stochez in server

                if (receiving<totalReceived)
                    do {
                        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); 

                        //recvbuf[iResult] = '\0';
                        binaryData.insert(binaryData.end(), recvbuf, recvbuf + iResult);
                        receiving += iResult;
                        
                    } while (receiving < totalReceived);

               // buffer[receiving] = '\0';
                 

                std::cout << "[REQUEST TYPE] Adaugare fisier: " << std::endl;
                UserManager::HandleClientUploadFile(stream, token, ':', resultOperation, binaryData);

                if (resultOperation)
                    buffer = "1";
                else
                    buffer = "0";

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '5')
            {
                
                std::cout << "[REQUEST TYPE] Descarcare fisier : " << std::endl;

                std::getline(stream, token, ':');
                IDUser = std::stoi(token);

                std::string selectFiles = DatabaseManagerAccounts::selectFiles(IDUser);
                iSendResult = send(ClientSocket, selectFiles.c_str(), selectFiles.size(), 0);//<numberFiles>:file1:file2:...:filen

                iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); //numele fisierului selectat
                buffer = recvbuf;

                std::vector<uint8_t> content = {};

                content = UserManager::HandleClientDownloadFile(buffer, resultOperation);

                if (resultOperation && !content.empty())
                {
                    iSendResult = send(ClientSocket, std::to_string(content.size()).c_str(), std::to_string(content.size()).size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                        return;
                    }

                                     

                    Sleep(100);

                    int iSendResult = send(ClientSocket,
                        reinterpret_cast<const char*>(content.data()),
                        static_cast<int>(content.size()),
                        0);

                    if (iSendResult == SOCKET_ERROR) {
                        printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                        return;
                    }

                    printf("Bytes sent: %d\n", iSendResult);
                }
                else {
                    buffer = "0";
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    Sleep(100);
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        printf("[EROARE] send failed with error: %d\n", WSAGetLastError());
                        //WSACleanup();
                        return;
                    }
                    printf("Bytes sent: %d\n", iSendResult);
                }
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