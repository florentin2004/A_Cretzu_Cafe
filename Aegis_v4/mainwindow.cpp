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

    client = new Network(this);
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
    setUserId(id);
    qDebug() << "User ID updated:" << getUserId();

    if (getUserId() != "-1") {
        ui->Window_logic->setCurrentIndex(1);  // Move to the main app UI
        ui->kat->setCurrentIndex(0); // Idle
    }
}

void MainWindow::displayError(const QString &message)
{
    QMessageBox::warning(this, "Eroare", message);
}


void MainWindow::on_LoginButton_login_clicked()
{

    QString username = ui->user_lineEdit->text();
    QString password = ui->password_lineEdit->text();

    ui->user_lineEdit->clear();
    ui->password_lineEdit->clear();
    ui->PasswordButton->setChecked(false);

    //QByteArray key = "Cretzu_Cafe+Aegis"; // just in case

    // Hashing username and password with key
    QByteArray hashedUsername = QCryptographicHash::hash(getKey() + username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(getKey() + password.toUtf8(), QCryptographicHash::Sha256).toHex();


    QString loginData = "0:" + hashedUsername+":"+hashedPassword;
    setUserName(hashedUsername);
    setUserNameNH(username);
    client->sendMessage(loginData);

    connect(client, &Network::errorOccurred, this, &MainWindow::displayError);
    connect(client, &Network::userIdReceived, this, &MainWindow::handleUserIdReceived);


}


void MainWindow::on_UploadButton_clicked()
{
    ui->kat->setCurrentIndex(0); //Idle
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

    //QByteArray key = "Cretzu_Cafe+Aegis"; //just in case

    // Hashing username and password with key
    QByteArray hashedUsername = QCryptographicHash::hash(getKey() + username.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashedPassword = QCryptographicHash::hash(getKey() + password.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Prepare registration data
    QString registerData = "1:" + hashedUsername + ":" + hashedPassword;

    // Send data
    client->sendMessage(registerData);

    connect(client, &Network::errorOccurred, this, &MainWindow::displayError);

    ui->right_twix->setCurrentIndex(3); // Idle Page
}


void MainWindow::on_LogoButton_clicked()
{
    ui->right_twix->setCurrentIndex(4); // Credential Page
}


void MainWindow::on_LogOffButton_clicked()
{
    setUserId("-1");
    ui->Window_logic->setCurrentIndex(0); // MainMenu
    ui->right_twix->setCurrentIndex(3); // Idle
}

void MainWindow::updateFileList(const QStringList &fileNames)
{
    ui->listWidget->clear(); // Golește lista anterioară

    for (const QString &fileName : fileNames)
    {
        ui->listWidget->addItem(fileName);
    }
}


void MainWindow::on_DownloadButton_clicked()
{
    ui->kat->setCurrentIndex(1); // Download
    QString seeFile = "7:" + getUserId();
    client->sendMessage(seeFile);
    connect(client, QOverload<const QStringList &>::of(&Network::fileListReceived),this, &MainWindow::updateFileList);

}



void MainWindow::on_SettingsButton_clicked()
{
    ui->kat->setCurrentIndex(2); // Settings
    ui->UserId_label->setText("User Id: "+getUserId());
    ui->Username_label->setText("Username: "+getUserNameNH());
}


void MainWindow::on_ChangePasswordButton_clicked()
{
    ui->kat->setCurrentIndex(3); // Change Password
}


void MainWindow::on_showpasswordButton_clicked(bool checked)
{
    if (checked) {
        ui->oldpassword_lineEdit->setEchoMode(QLineEdit::Normal); // Show characters
        ui->newpassword_lineEdit->setEchoMode(QLineEdit::Normal);
    } else {
        ui->oldpassword_lineEdit->setEchoMode(QLineEdit::Password); // Hide characters
        ui->newpassword_lineEdit->setEchoMode(QLineEdit::Password);
    }
}


void MainWindow::on_ChangeButton_clicked()
{
    QString oldpassword = ui->oldpassword_lineEdit->text();
    QString newpassword = ui->newpassword_lineEdit->text();

    ui->oldpassword_lineEdit->clear();
    ui->newpassword_lineEdit->clear();

    //QByteArray key = "Cretzu_Cafe+Aegis"; //just in case

    QByteArray hashedoldpassword = QCryptographicHash::hash(getKey() + oldpassword.toUtf8(), QCryptographicHash::Sha256).toHex();
    QByteArray hashednewpassword = QCryptographicHash::hash(getKey() + newpassword.toUtf8(), QCryptographicHash::Sha256).toHex();

    // Prepare registration data
    // trebuie sa verific cu Arian asta
    QString changeData = "2:"+getUserName()+":"+hashednewpassword+":"+hashedoldpassword;

    // Send data
    client->sendMessage(changeData);

    ui->Window_logic->setCurrentIndex(0); // MainMenu
    ui->right_twix->setCurrentIndex(3); // Idle

}


void MainWindow::on_DeleteAccountButton_clicked()
{
    // trebuie  sa verific cu Arian asta
    QString deleteData = "3:"+getUserId();
    // Send data
     client->sendMessage(deleteData);
    setUserId("-1");
    ui->Window_logic->setCurrentIndex(0); // MainMenu
    ui->right_twix->setCurrentIndex(3); // Idle
}


void MainWindow::on_DownloadButton_d_clicked()
{
    QString filename = ui->filename_lineEdit->text();
    QString filesData = "5:" + filename;
    client->sendMessage(filesData);
    ui->filename_lineEdit->clear();
}

#include <QInputDialog>

void MainWindow::on_sendButton_clicked()
{
    // Deschide un dialog pentru introducerea numelui de utilizator
    bool ok;
    QString enteredUsername = QInputDialog::getText(this, "Introduceți numele","Nume utilizator:", QLineEdit::Normal,"", &ok);

    if (ok && !enteredUsername.isEmpty()) {
        userName = enteredUsername;
        QString filename = ui->filename_lineEdit->text();
        QByteArray hashedUsername = QCryptographicHash::hash(getKey() + userName.toUtf8(), QCryptographicHash::Sha256).toHex();
        QString filesData = "6:"+hashedUsername+ ":" + filename;
        client->sendMessage(filesData);
    } else {
        qDebug() << "Utilizatorul a anulat sau nu a introdus nimic.";
    }
    ui->filename_lineEdit->clear();
}


void MainWindow::on_DeleteButton_clicked()
{
    QString filename = ui->filename_lineEdit->text();
    QString filesData = "8:" + getUserId()+":"+filename;
    client->sendMessage(filesData);
    ui->filename_lineEdit->clear();
}

