#include "mainwindow.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QFileDialog>
#include "./ui_mainwindow.h"
#include <QIcon>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/Images/Aegis.ico"));
}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::on_ExitButton_clicked()
{
    ui->right_twix->setCurrentIndex(2); // Exit page
}


void MainWindow::on_noButton_clicked()
{
    ui->right_twix->setCurrentIndex(3); // Idle page
}


void MainWindow::on_yesButton_clicked()
{
    client->sendMessage("q"); // Notify the server
    QApplication::quit(); // Exit the app
}


void MainWindow::on_PasswordButton_clicked(bool checked)
{
    if (checked) {
        ui->password_lineEdit->setEchoMode(QLineEdit::Normal); // Show characters
    } else {
        ui->password_lineEdit->setEchoMode(QLineEdit::Password); // Hide characters
    }
}


void MainWindow::on_LoginButton_clicked()
{
    ui->right_twix->setCurrentIndex(0); // Login page
}


void MainWindow::on_RegisterButton_clicked()
{
    ui->right_twix->setCurrentIndex(1); // Register page
}


#include <QCryptographicHash>

void MainWindow::handleUserIdReceived(const QString &id)
{
    setUserId(id);  // Store ID
    qDebug() << "User ID updated:" << getUserId();
}

void MainWindow::on_LoginButton_login_clicked()
{
    QString username = ui->user_lineEdit->text();
    QString password = ui->password_lineEdit->text();

    ui->user_lineEdit->clear();
    ui->password_lineEdit->clear();
    ui->PasswordButton->setChecked(false);

    QByteArray key = "Cretzu_Cafe+Aegis";

    // Hashing username and password with key
    QByteArray hashedUsername = QCryptographicHash::hash(key + username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(key + password.toUtf8(), QCryptographicHash::Sha256).toHex();


    QString loginData = "0:" + hashedUsername+":"+hashedPassword;
    connect(client, &Network::userIdReceived, this, &MainWindow::handleUserIdReceived);
    client->sendMessage(loginData);
    ui->Window_logic->setCurrentIndex(1); // App
}


void MainWindow::on_UploadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Select File", QDir::homePath(), "All Files (*.*)");
    if (!fileName.isEmpty())
    {
        client->sendFile(fileName, getUserId());
    }
}


void MainWindow::on_user_lineEdit_returnPressed()
{
    ui->password_lineEdit->setFocus();
}


void MainWindow::on_password_lineEdit_returnPressed()
{
    ui->LoginButton_login->click();
}


void MainWindow::on_PasswordButton_register_clicked(bool checked)
{
    if (checked) {
        ui->password_lineEdit_register->setEchoMode(QLineEdit::Normal); // Show characters
        ui->password_again_lineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        ui->password_lineEdit_register->setEchoMode(QLineEdit::Password); // Hide characters
        ui->password_again_lineEdit->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::on_user_lineEdit_register_returnPressed()
{
    ui->password_lineEdit_register->setFocus();
}


void MainWindow::on_password_lineEdit_register_returnPressed()
{
    ui->password_again_lineEdit->setFocus();
}




void MainWindow::on_password_again_lineEdit_returnPressed()
{
    ui->RegisterButton_register->click();
}


void MainWindow::on_RegisterButton_register_clicked()
{
    QString username = ui->user_lineEdit_register->text();
    QString password = ui->password_lineEdit_register->text();

    // Check if passwords match
    if (ui->password_lineEdit_register->text() != ui->password_again_lineEdit->text())
    {
        QMessageBox::warning(this, "Eroare", "Parolele nu sunt identice!");
        return; // Stop execution if passwords don't match
    }

    ui->user_lineEdit_register->clear();
    ui->password_lineEdit_register->clear();
    ui->password_again_lineEdit->clear();
    ui->PasswordButton_register->setChecked(false);

    QByteArray key = "Cretzu_Cafe+Aegis";

    // Hashing username and password with key
    QByteArray hashedUsername = QCryptographicHash::hash(key + username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(key + password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Prepare registration data
    QString registerData = "1:" + hashedUsername + ":" + hashedPassword;

    // Send data
    client->sendMessage(registerData);
    ui->right_twix->setCurrentIndex(3); // Idle Page
}


void MainWindow::on_LogoButton_clicked()
{
    ui->right_twix->setCurrentIndex(4); // Credential Page
}


void MainWindow::on_LogOffButton_clicked()
{
    ui->Window_logic->setCurrentIndex(0); // MainMenu
    ui->right_twix->setCurrentIndex(3); // Idle
}


void MainWindow::on_DownloadButton_clicked()
{

}

