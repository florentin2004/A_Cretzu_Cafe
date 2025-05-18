#include "Retea.h"

int Retea::Initialize() {
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::string errorMessage = "[EROARE] WSAStartup failed with error: " + std::to_string(iResult);
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        std::string errorMessage = "[EROARE] getaddrinfo failed with error: " + std::to_string(iResult);
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);
        //WSACleanup();
        return 1;
    }
    
    return 0;
}

int Retea::CreateSocket() {
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::string errorMessage = "[EROARE] socket failed with error: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        freeaddrinfo(result);

        Logger::logAction(errorMessage);
        //WSACleanup();
        return 1;
    }

    int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] bind failed with error: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);

        freeaddrinfo(result);
        closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] listen failed with error: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);

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
        std::string errorMessage = "[EROARE] accept failed with error: " + std::to_string(WSAGetLastError());
        std::cerr << errorMessage << std::endl;

        Logger::logAction(errorMessage);

        //closesocket(ListenSocket);
        //WSACleanup();
        return 1;
    }

    printf("[SERVER] Client conectat!\n");
    this->on = true;

    return 0;
}

void Retea::Shutdown() {
    int iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] shutdown failed with error: " + std::to_string(iResult);
        std::cerr << errorMessage << std::endl;

        Logger::logAction(errorMessage);
    }

    closesocket(ClientSocket);
    WSACleanup();
}




void Retea::HandleClient() {
    int iResult = 0;
    int iSendResult = 0;
    //std::string logMessage;

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
                {
                    buffer = "1:" + std::to_string(IDUser);
                    logMessage = "[REQUEST RESULT] Logare cu success (ID = " + std::to_string(IDUser) + ")";
                    std::cout << logMessage << std::endl;
                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    logMessage = "[REQUEST RESULT] Logare esuata!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '1')
            {

                //std::cout << "[REQUEST TYPE] Inregistrare: " << std::endl;
                UserManager::HandleClientRegister(stream, token, ':', resultOperation);

                if (resultOperation)
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] Inregistrare cu succes!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    logMessage = "[REQUEST RESULT] Inregistrare esuata!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

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
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] Parola a fost schimbata cu succes!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    logMessage = "[REQUEST RESULT] Probleme la schimbarea parolei!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

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
                {
                    buffer = "1";
                    std::cout << buffer;
                    logMessage = "[REQUEST RESULT] Stergere cont cu succes!";;
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    std::cout << buffer;
                    logMessage = "[REQUEST RESULLT] Probleme la stergerea contului!";;
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '4')
            {
                std::cout << "[REQUEST TYPE] Adaugare fisier: " << std::endl;

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
                 

                UserManager::HandleClientUploadFile(stream, token, ':', resultOperation, binaryData);

                if (resultOperation)
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] Fisier adaugat cu succes!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    logMessage = "[REQUEST RESULT] Probleme la adaugarea fisierelor!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '5')
            {
                
                std::cout << "[REQUEST TYPE] Descarcare fisier : " << std::endl;

                std::getline(stream, token, ':');

                buffer = token;
                resultOperation = true;

                //IDUser = std::stoi(token);

                //std::string selectFiles = DatabaseManagerAccounts::selectFiles(IDUser);
                //iSendResult = send(ClientSocket, selectFiles.c_str(), selectFiles.size(), 0);//<numberFiles>:file1:file2:...:filen

                //iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); //numele fisierului selectat
                //buffer = recvbuf;

                std::vector<uint8_t> content = {};

                content = UserManager::HandleClientDownloadFile(buffer, resultOperation);

                if (resultOperation && !content.empty())
                {

                    std::vector<uint8_t> sendBuffer;

                    // Adaugă lungimea ca șir de caractere
                    std::string sizeStr = std::to_string(content.size());
                    sendBuffer.insert(sendBuffer.end(), sizeStr.begin(), sizeStr.end());

                    // Adaugă delimitatorul ":"
                    sendBuffer.insert(sendBuffer.end(), ':');

                    // Adaugă conținutul
                    sendBuffer.insert(sendBuffer.end(), content.begin(), content.end());
                    
                    content.clear();

                    const size_t chunkSize = 512;
                    size_t bytesSent = 0;
                    while (bytesSent < sendBuffer.size()) {
                        size_t bytesToSend = std::min<size_t>(chunkSize, sendBuffer.size() - bytesSent);
                        iSendResult = send(ClientSocket,
                            reinterpret_cast<const char*>(sendBuffer.data() + bytesSent),
                            static_cast<int>(bytesToSend),
                            0);
                        if (iSendResult == SOCKET_ERROR) {
                            logMessage = "[EROARE] send continut failed with error: " + std::to_string(WSAGetLastError());
                            std::cerr << logMessage << std::endl;
                            Logger::logAction(logMessage);
                            return;
                        }
                        if (iSendResult != static_cast<int>(bytesToSend)) {
                            logMessage = "[AVERTISMENT] send incomplet: trimis " + std::to_string(iSendResult) + "/" + std::to_string(bytesToSend) + " octeți";
                            std::cerr << logMessage << std::endl;
                            Logger::logAction(logMessage);
                            // Continuă să trimiți restul datelor
                        }
                        bytesSent += iSendResult;
                    }

                    if (bytesSent != sendBuffer.size()) {
                        logMessage = "[EROARE] send incomplet: " + std::to_string(bytesSent) + "/" + std::to_string(sendBuffer.size()) + " octeți trimiși";
                        std::cerr << logMessage << std::endl;
                        Logger::logAction(logMessage);
                    }

                    if (iSendResult == SOCKET_ERROR) {
                        logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                        std::cerr << logMessage << std::endl;

                        Logger::logAction(logMessage);
                        return;
                    }

                    logMessage = "[REQUEST RESULT] Descarcare fisier reusita!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);

                    //printf("Bytes sent: %d\n", iSendResult);
                }
                else {
                    buffer = "0";
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    Sleep(100);
                    iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                    if (iSendResult == SOCKET_ERROR) {
                        logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                        std::cerr << logMessage << std::endl;

                        Logger::logAction(logMessage);
                        //WSACleanup();
                        return;
                    }
                    printf("Bytes sent: %d\n", iSendResult);

                    logMessage = "[REQUEST RESULT] Probleme la descarcarea fisierelor!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
            }
            else if (buffer[0] == '6')//vad in ce ordine trimite si clientul username-ul clientului la care vrea sa il trimita
            {
                std::cout << "[REQUEST TYPE] Trimitere fisier catre alt client";

                std::getline(stream, token, ':');
                IDUser = std::stoi(token); // cu tot cu ID-ul userului

                std::string selectFiles = DatabaseManagerAccounts::selectFiles(IDUser);
                iSendResult = send(ClientSocket, selectFiles.c_str(), selectFiles.size(), 0);
                printf("Bytes sent: %d\n", iSendResult);

                iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);// username:filename
                buffer = recvbuf;
                
                stream.clear();
                stream.str(buffer);

                getline(stream, token, ':');

                IDUser = DatabaseManagerAccounts::selectUserWithoutPassword(token);

                getline(stream, token, ':');

                resultOperation = DatabaseManagerAccounts::UpdateFileID(IDUser, token);

                if (resultOperation)
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] Fisier trimis catre alt utilizator cu succes!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "0";
                    logMessage = "[REQUEST RESULT] Probleme la trimiterea fisierelor la alt utilizator!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);

                //asta este ID-ul userului la care ii se va trimite fisierul

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

                logMessage = "[PROTOCOL] Protocol neidentificat, informatie aruncata!";
                std::cout << logMessage << std::endl;

                Logger::logAction(logMessage);

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] send failed with error: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
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
                logMessage = "[EROARE] recv failed with error: " + std::to_string(errorCode);
                std::cerr << logMessage << std::endl;

                Logger::logAction(logMessage);

                closesocket(ClientSocket);
            }
        }

    } while (iResult > 0);

    
}