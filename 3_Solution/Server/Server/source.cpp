#include "Retea.h"

int __cdecl main(void)
{
    Retea server;

    if (server.Initialize() != 0) return 1;
    if (server.CreateSocket() != 0) return 1;
    if (server.AcceptConnection() != 0) return 1;

    server.HandleClient();

    return 0;
}