#include "mainwindow.h"
#include <QCryptographicHash>
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

void MainWindow::on_LoginButton_2_clicked()
{
    QString username = ui->user_lineEdit->text();
    QString password = ui->password_lineEdit->text();
    QByteArray key = "Cretzu_Cafe+Aegis";

    // Hashing username and password with key
    QByteArray hashedUsername = QCryptographicHash::hash(key + username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(key + password.toUtf8(), QCryptographicHash::Sha256).toHex();


    QString loginData = "0:" + hashedUsername + ":" + hashedPassword;
    client->sendMessage(loginData);
}

