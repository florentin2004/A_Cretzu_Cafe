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

void Network::sendFile(const QString &filePath, const QString &idUser)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QByteArray fileData = file.readAll();
            file.close();

            QString fileName = QFileInfo(filePath).fileName();
            QString fileNameLength = QString::number(fileName.toUtf8().size());
            QString fileSize = QString::number(fileData.size());

            QString header = "4:" + fileSize + ":" + idUser + ":" + fileNameLength + ":" + fileName;
            QByteArray kitMessage = header.toUtf8();

            socket->write(kitMessage);
            socket->waitForBytesWritten();
            QThread::msleep(100);
            socket->write(fileData);
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

    if (data.startsWith("1:")) { // Autentificare
        QList<QByteArray> parts = data.split(':');
        if (parts.size() > 1) {
            emit userIdReceived(QString::fromUtf8(parts[1]));
        }
    }
    else if (data.startsWith("5:")) { // Început de fișier
        QList<QByteArray> parts = data.split(':');
        if (parts.size() >= 3) {
            expectedFileSize = parts[1].toInt();
            fileBuffer = data.mid(data.indexOf(':', 2) + 1);
            receivingFile = true;
            receivedFileName = "elf.png"; // Poate fi adaptat dacă serverul trimite numele

            qDebug() << "Start receiving file. Expected size:" << expectedFileSize;

            if (fileBuffer.size() >= expectedFileSize) {
                saveFile(fileBuffer);
                receivingFile = false;
                expectedFileSize = 0;
                fileBuffer.clear();
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
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullFilePath = QDir::cleanPath(downloadPath + "/" + receivedFileName);

    QFile file(fullFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to save file.";
        return;
    }

    file.write(fileContent);
    file.close();

    emit fileDownloaded(receivedFileName);
    QDesktopServices::openUrl(QUrl::fromLocalFile(fullFilePath));
}
