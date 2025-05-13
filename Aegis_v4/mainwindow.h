#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "Network.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void make_connection() {
        if(is_connection_made == false)
        {
            client = new Network();
            client->connectToServer();
            is_connection_made = true;
        }
        else
            qDebug() << "Connection already made!";
    }

private slots:
    void on_ExitButton_clicked();

    void on_noButton_clicked();

    void on_yesButton_clicked();

    void on_PasswordButton_clicked(bool checked);

    void on_LoginButton_clicked();

    void on_RegisterButton_clicked();

    void on_LoginButton_2_clicked();

    void on_uploadButton_clicked();

private:
    Ui::MainWindow *ui;
    Network *client;
    bool is_connection_made = false;

};
#endif // MAINWINDOW_H
