#include "mainmenu.h"
#include "register.h"
#include "login.h"
#include "Network.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Network *client = new Network();
    client->connectToServer();

    MainMenu mainMenu;
    mainMenu.show();

    return app.exec();
}
