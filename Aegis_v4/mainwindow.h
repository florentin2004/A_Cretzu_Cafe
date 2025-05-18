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

    void on_LoginButton_login_clicked();

    void on_UploadButton_clicked();

    void on_user_lineEdit_returnPressed();

    void on_password_lineEdit_returnPressed();

    void on_PasswordButton_register_clicked(bool checked);

    void on_user_lineEdit_register_returnPressed();

    void on_password_lineEdit_register_returnPressed();

    void on_password_again_lineEdit_returnPressed();

    void on_RegisterButton_register_clicked();

    void on_LogoButton_clicked();

    void on_LogOffButton_clicked();

    void on_DownloadButton_clicked();

    void on_SettingsButton_clicked();

    void on_ChangePasswordButton_clicked();

    void on_showpasswordButton_clicked(bool checked);

    void on_ChangeButton_clicked();

    void on_DeleteAccountButton_clicked();

    void on_DownloadButton_d_clicked();

public slots:
    void updateFileList(const QStringList &fileNames);

private:
    Ui::MainWindow *ui;
    Network *client;
    bool is_connection_made = false;

private:
    QString userId = "-1";  // user ID
    QString userName;
    QByteArray key = "Cretzu_Cafe+Aegis"; // key

public:
    void setUserId(const QString &id) { userId = id; }
    QString getUserId() const { return userId; }
    QByteArray getKey(){return this->key;};
    void setUserName(const QString &name){userName = name;};
    QString getUserName() const {return userName;};
    void handleUserIdReceived(const QString &id);
    void displayError(const QString &message);

};
#endif // MAINWINDOW_H
