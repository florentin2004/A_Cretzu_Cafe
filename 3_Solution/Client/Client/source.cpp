#include "Network.h"

int __cdecl main(int argc, char** argv)
{
    Network* server = new Network();

    server->connectServer(argc, argv);
    server->sent();// aici se va pune un parametru de tip char/string care acela va respecta acel protocol de sent
    server->receive();// in functie de ce sent am dat inainte, o sa il folosesc pentru a deserializa acel pachet dat de server

    delete server;

    return 0;
}