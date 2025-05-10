#include "Network.h"

int __cdecl main(int argc, char** argv)
{
    Network* server = new Network();
    std::string message;

    if (server->connectServer(argc, argv) != 0) return 1;

    while (true)
    {
        std::cout << "Introdu protocolul: ";
        std::getline(std::cin, message);
        if (message == "quit")
            break;

        if (server->sent(message) != 0) break;
        if (server->receive() != 0) break;
    }

    delete server;
    return 0;
}