#include "Network.h"

int Network::connectServer(int argc, char** argv)
{
    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s Server\n", argv[0]);
        return 1;
    }

    // Initialize Winsock
    this->iResult = WSAStartup(MAKEWORD(2, 2), &this->wsaData);
    if (this->iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&this->hints, sizeof(this->hints));
    this->hints.ai_family = AF_UNSPEC;
    this->hints.ai_socktype = SOCK_STREAM;
    this->hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    this->iResult = getaddrinfo(argv[1], DEFAULT_PORT, &this->hints, &this->result);
    if (this->iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", this->iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (this->ptr = this->result; this->ptr != NULL; this->ptr = this->ptr->ai_next) {

        // Create a SOCKET for connecting to server
        this->ConnectSocket = socket(this->ptr->ai_family, this->ptr->ai_socktype,
            this->ptr->ai_protocol);
        if (this->ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        this->iResult = connect(this->ConnectSocket, this->ptr->ai_addr, (int)this->ptr->ai_addrlen);
        if (this->iResult == SOCKET_ERROR) {
            closesocket(this->ConnectSocket);
            this->ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (this->ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    return 0;
}

int Network::sent(std::string& message)
{

    this->sendbuf = message;
    // Send an initial buffer
    this->iResult = send(this->ConnectSocket, this->sendbuf.c_str(), (int)this->sendbuf.length(), 0);
    if (this->iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(this->ConnectSocket);
        //WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    /*this->iResult = shutdown(this->ConnectSocket, SD_SEND);
    if (this->iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(this->ConnectSocket);
        WSACleanup();
        return 1;
    }*/

    return 0;
}

int Network::receive()
{
    this->iResult = recv(this->ConnectSocket, this->recvbuf, this->recvbuflen - 1, 0);
    if (this->iResult > 0)
    {
        this->recvbuf[iResult] = '\0';
        std::cout << this->recvbuf << std::endl;
        printf("Bytes received: %d\n", iResult);
    }
    else if (iResult == 0)
        printf("Connection closed\n");
    else
    {
        printf("recv failed with error: %d\n", WSAGetLastError());
        return 1;
    }

    return 0;
}

Network::~Network()
{
    // cleanup
    closesocket(this->ConnectSocket);
    WSACleanup();
}
