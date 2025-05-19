#include "Retea.h"

int Retea::Initialize() {
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::string errorMessage = "[EROARE] WSAStartUp eroare: " + std::to_string(iResult);
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
        std::string errorMessage = "[EROARE] Probleme la getaddrinfo: " + std::to_string(iResult);
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);
        return 1;
    }
    
    return 0;
}

int Retea::CreateSocket() {
    ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
    if (ListenSocket == INVALID_SOCKET) {
        std::string errorMessage = "[EROARE] Nu se creeaza socket-ul cu eroarea: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        freeaddrinfo(result);

        Logger::logAction(errorMessage);
        return 1;
    }

    int iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] Nu se leaga cu eroarea: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);

        freeaddrinfo(result);
        closesocket(ListenSocket);
        return 1;
    }

    freeaddrinfo(result);

    iResult = listen(ListenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] Nu asculta cu eroarea: " + std::to_string(WSAGetLastError());
        std::cout << errorMessage << std::endl;

        Logger::logAction(errorMessage);

        closesocket(ListenSocket);
        return 1;
    }

    printf("[SERVER] S-a initializat cu succes!\n");

    return 0;
}

int Retea::AcceptConnection() {
    ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        std::string errorMessage = "[EROARE] Nu accepta cu eroarea: " + std::to_string(WSAGetLastError());
        std::cerr << errorMessage << std::endl;

        Logger::logAction(errorMessage);
        return 1;
    }

    printf("[SERVER] Client conectat!\n");
    this->on = true;

    return 0;
}

void Retea::Shutdown() {
    int iResult = shutdown(ClientSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::string errorMessage = "[EROARE] Probleme la shutdown: " + std::to_string(iResult);
        std::cerr << errorMessage << std::endl;

        Logger::logAction(errorMessage);
    }

    closesocket(ClientSocket);
    WSACleanup();
}




void Retea::HandleClient() {
    int iResult = 0;
    int iSendResult = 0;

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
            if (buffer[0] == '0')
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
                    buffer = "0"; // eroare logare, clientul stie ca eroarea este de tip log
                    logMessage = "[REQUEST RESULT] Logare esuata!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
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
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] Inregistrare cu succes!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "2"; //eroare register, clietnul stie ca eroarea este de tip register
                    logMessage = "[REQUEST RESULT] Inregistrare esuata!";
                    std::cout << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
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

                std::cout << buffer << std::endl;

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
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
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
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

                //aici dupa cererea de upload, clientul pune in protocol lungimea contentului fisierului ca sa pun atatia octeti de date si dupa sa le stochez in fisierele locale ale server-ului

                if (receiving<totalReceived)
                    do {
                        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0); 

                        //recvbuf[iResult] = '\0';
                        binaryData.insert(binaryData.end(), recvbuf, recvbuf + iResult);
                        receiving += iResult;
                        
                    } while (receiving < totalReceived);

                 

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
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);
            }
            else if (buffer[0] == '5')
            {
                
                std::cout << "[REQUEST TYPE] Descarcare fisier : " << std::endl;

                std::getline(stream, token, ':');

                buffer = token;

                std::vector<uint8_t> content = {};

                content = UserManager::HandleClientDownloadFile(buffer, resultOperation);

                if (resultOperation && !content.empty())
                {

                    std::vector<uint8_t> sendBuffer;

                    std::string sizeStr = std::to_string(content.size());

                    sendBuffer.insert(sendBuffer.end(), '5');//numele protocolului
                    sendBuffer.insert(sendBuffer.end(), ':');

                    sendBuffer.insert(sendBuffer.end(), buffer.begin(), buffer.end());//numele fisierului
                    sendBuffer.insert(sendBuffer.end(), ':');


                    sendBuffer.insert(sendBuffer.end(), sizeStr.begin(), sizeStr.end());//size-ul content-ului fisierului
                    sendBuffer.insert(sendBuffer.end(), ':');

                    sendBuffer.insert(sendBuffer.end(), content.begin(), content.end());//content binar fisier
                    
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
                            logMessage = "[EROARE] Problema send socket:: " + std::to_string(WSAGetLastError());
                            std::cerr << logMessage << std::endl;
                            Logger::logAction(logMessage);
                            return;
                        }
                        if (iSendResult != static_cast<int>(bytesToSend)) {
                            logMessage = "[AVERTISMENT] Send incomplet: trimis " + std::to_string(iSendResult) + "/" + std::to_string(bytesToSend) + " octeti";
                            std::cerr << logMessage << std::endl;
                            Logger::logAction(logMessage);
                            // Continuă să trimiți restul datelor
                        }
                        bytesSent += iSendResult;
                    }

                    std::cout << bytesSent << std::endl;
                    std::cout << sendBuffer.size() << std::endl;

                    if (bytesSent != sendBuffer.size()) {
                        logMessage = "[EROARE] Send incomplet: " + std::to_string(bytesSent) + "/" + std::to_string(sendBuffer.size()) + " octeti trimisi";
                        std::cerr << logMessage << std::endl;
                        Logger::logAction(logMessage);
                    }

                    if (iSendResult == SOCKET_ERROR) {
                        logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
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
                        logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
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

                UserManager::HandleClientSendFileToAnotherUser(stream, token, ':', resultOperation);

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
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);

                    //WSACleanup();
                    return;
                }
                printf("Bytes sent: %d\n", iSendResult);

                //asta este ID-ul userului la care ii se va trimite fisierul

            }
            else if (buffer[0] == '7')
            {
                std::cout << "[REQUEST TYPE] Selectia de fisiere: " << std::endl;

                std::getline(stream, token, ':');

                buffer = token;
                IDUser = std::stoi(token);

                DatabaseManagerAccounts::connect();
                std::string selectFiles = DatabaseManagerAccounts::selectFiles(IDUser);
                DatabaseManagerAccounts::disconnect();

                buffer.clear();
                buffer = "7:" + selectFiles;
                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);//<numberFiles>:file1:file2:...:filen

                std::cout << buffer << std::endl;

                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                    return;
                }

                if (selectFiles == "")
                {
                    buffer = "0";
                    logMessage = "[EROARE] Probleme la selectia de fisiere!";
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] A trimis serverul catre client selectia de fisiere!";
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }

            }
            else if (buffer[0] == '8')
            {
                std::cout << "[REQUEST TYPE] Stergere fisier: " << std::endl;

                UserManager::HandleClientDeleteFile(stream, token, ':', resultOperation);

                if (resultOperation == false)
                {
                    buffer = "0";
                    logMessage = "[EROARE] Probleme la selectia de fisiere!";
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                else
                {
                    buffer = "1";
                    logMessage = "[REQUEST RESULT] A trimis serverul catre client selectia de fisiere!";
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                }
                
                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);//<numberFiles>:file1:file2:...:filen

                std::cout << buffer << std::endl;

                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
                    std::cerr << logMessage << std::endl;

                    Logger::logAction(logMessage);
                    return;
                }

            }
            else if (token == "q")
            {
                printf("[SERVER] S-a deconectat clientul!\n");
                closesocket(ClientSocket);
            }
            else
            {

                buffer = "Protocol neidentificat";

                logMessage = "[PROTOCOL] Protocol neidentificat, informatie aruncata!";
                std::cout << logMessage << std::endl;

                Logger::logAction(logMessage);

                iSendResult = send(ClientSocket, buffer.c_str(), buffer.size(), 0);
                if (iSendResult == SOCKET_ERROR) {
                    logMessage = "[EROARE] Problema send socket: " + std::to_string(WSAGetLastError());
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
                logMessage = "[EROARE] Problema receive socket: " + std::to_string(errorCode);
                std::cerr << logMessage << std::endl;

                Logger::logAction(logMessage);

                closesocket(ClientSocket);
            }
        }

    } while (iResult > 0);

    
}