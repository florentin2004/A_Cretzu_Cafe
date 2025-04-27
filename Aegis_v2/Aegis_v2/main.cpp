#include "mainmenu.h"
#include "register.h"
#include "login.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainMenu mm;
    Register rg;
    Login l;
    mm.show();
    return a.exec();
}
