#include "Retea.h"

//REGULI:
//  1. Clientul trebuie sa nu uite sa reconfigureze ip-ul la care se va conecta cu serverul
//  2. Nu uita ca serverul, inainte sa fie rulat, sa fie conectat la baza de date

#include <iostream>
#include <thread>
#include <atomic>
#include <string>

int __cdecl main(void)
{
    Retea server;
     if (server.Initialize()) return 1;
        if (server.CreateSocket()) return 1;
    bool on = true;
    while (on) {
        std::cout << "[SERVER] Astept un client nou...\n";
        if (server.AcceptConnection() == 0)
        {
                server.HandleClient();
        }
    }
    server.Shutdown();
    return 0;
}