#include "Network.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QStandardPaths>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>

Network::Network(QObject *parent) : QObject(parent), serverIP("10.10.25.122"), serverPort(27015)
{
    socket = new QTcpSocket(this);

    connect(socket, &QTcpSocket::connected, this, &Network::onConnected);
    connect(socket, &QTcpSocket::readyRead, this, &Network::onMessageReceived);
    connect(socket, &QTcpSocket::errorOccurred, this, &Network::onErrorOccurred);
}

void Network::setServerDetails(const QString &ip, int port)
{
    serverIP = ip;
    serverPort = port;
}

void Network::connectToServer()
{
    qDebug() << "Connecting to server at:" << serverIP << ":" << serverPort;
    socket->connectToHost(serverIP, serverPort);

    if (!socket->waitForConnected(5000)) {
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

void Network::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(message.toUtf8());
        qDebug() << "Sent message:" << message;
    } else {
        qDebug() << "Cannot send message, socket not connected!";
    }
}

QByteArray Network::xorEncrypt(const QByteArray &data, const QByteArray &key)
{
    QByteArray encrypted = data;
    for (int i = 0; i < encrypted.size(); ++i) {
        encrypted[i] = encrypted[i] ^ key[i % key.size()];
    }
    return encrypted;
}

void Network::sendFile(const QString &filePath, const QString &idUser)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();

            QByteArray encryptedData = xorEncrypt(fileData, key);  // Encrypt data

            QString fileName = QFileInfo(filePath).fileName();
            QString fileNameLength = QString::number(fileName.toUtf8().size());
            QString fileSize = QString::number(encryptedData.size());

            QString header = "4:" + fileSize + ":" + idUser + ":" + fileNameLength + ":" + fileName;
            QByteArray kitMessage = header.toUtf8();

            socket->write(kitMessage);
            socket->waitForBytesWritten();
            QThread::msleep(100);
            socket->write(encryptedData);
            socket->waitForBytesWritten();
        } else {
            qDebug() << "Failed to open file:" << filePath;
        }
    } else {
        qDebug() << "Cannot send file, socket not connected!";
    }
}


void Network::onConnected()
{
    qDebug() << "Successfully connected to the server!";
}

void Network::onMessageReceived()
{
    QByteArray data = socket->readAll();

    if (receivingFile) {
        fileBuffer.append(data);

        if (fileBuffer.size() >= expectedFileSize) {
            qDebug() << "Received entire file. Size:" << fileBuffer.size();
            saveFile(fileBuffer);
            receivingFile = false;
            expectedFileSize = 0;
            fileBuffer.clear();
        }
        return;
    }
    if (data == "0") {
        emit errorOccurred("Date incorecte");
        return;
    }
    if(data == "2")
    {
        emit errorOccurred("Nume utilizator deja existent.");
        return;
    }

    if (data.startsWith("1:")) { // Autentificare
        QList<QByteArray> parts = data.split(':');
        if (parts.size() > 1) {
            emit userIdReceived(QString::fromUtf8(parts[1]));
        }
    }
    else if (data.startsWith("5:")) {
        QList<QByteArray> parts = data.split(':');

        if (parts.size() >= 4) {
            // Extragere nume fișier și dimensiune
            receivedFileName = QString::fromUtf8(parts[1]);
            expectedFileSize = parts[2].toInt();

            // Caută poziția datelor (după al treilea ':')
            int thirdColonIndex = data.indexOf(':', data.indexOf(':', data.indexOf(':') + 1) + 1);
            if (thirdColonIndex != -1) {
                fileBuffer = data.mid(thirdColonIndex + 1);
            } else {
                fileBuffer.clear();
            }

            receivingFile = true;

            qDebug() << "Start receiving file:";
            qDebug() << " - File name:" << receivedFileName;
            qDebug() << " - Expected size:" << expectedFileSize;
            qDebug() << " - Initial payload size:" << fileBuffer.size();

            if (fileBuffer.size() >= expectedFileSize) {
                saveFile(fileBuffer);
                receivingFile = false;
                expectedFileSize = 0;
                fileBuffer.clear();
            }
        } else {
            qDebug() << "Invalid file message format.";
        }
    }
    if (data.startsWith("7:"))
    {
        qDebug() << "Received file list:" << data;

        QList<QByteArray> parts = data.split(':');

        if (parts.size() >= 3) {
            int nrFisiere = parts[1].toInt();
            QStringList fileNames;

            // Verificăm dacă numărul de fișiere se potrivește cu restul datelor
            for (int i = 2; i < parts.size(); ++i) {
                fileNames << QString::fromUtf8(parts[i]);
            }

            if (fileNames.size() == nrFisiere) {
                emit fileListReceived(fileNames);
            } else {
                qDebug() << "Avertisment: nr_fisiere declarat (" << nrFisiere << ") diferă de numărul real (" << fileNames.size() << ")";
                emit fileListReceived(fileNames); // Sau ignori dacă preferi
            }
        }
    }

}

void Network::onErrorOccurred(QAbstractSocket::SocketError)
{
    qDebug() << "Socket error occurred:" << socket->errorString();
}

void Network::saveFile(const QByteArray &fileContent)
{
    //  Decriptează conținutul
    QByteArray decryptedData = xorEncrypt(fileContent, key);

    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullFilePath = QDir::cleanPath(downloadPath + "/" + receivedFileName);

    QFile file(fullFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to save file.";
        return;
    }

    file.write(decryptedData);  //  Scriem fișierul decriptat
    file.close();

    emit fileDownloaded(receivedFileName);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fullFilePath));
}

