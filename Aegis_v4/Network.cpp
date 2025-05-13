#include "Network.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>

Network::Network(QObject *parent) : QObject(parent), serverIP("192.168.97.63"), serverPort(27015)
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

    if (!socket->waitForConnected(5000))
    {
        qDebug() << "Connection failed:" << socket->errorString();
    }
}

void Network::sendMessage(const QString &message)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->write(message.toUtf8());
        qDebug() << "Sent message:" << message;
    }
    else
    {
        qDebug() << "Cannot send message, socket not connected!";
    }
}

void Network::sendFile(const QString &filePath, const QString &idUser)
{
    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray fileData = file.readAll();
            file.close();

            // Extract file name and calculate file size
            QString fileName = QFileInfo(filePath).fileName();
            QString fileNameLength = QString::number(fileName.toUtf8().size());
            QString fileSize = QString::number(fileData.size());

            // Create message header
            QString header = "4:" + idUser + ":" + fileNameLength + ":" + fileName + ":" + fileSize + ":";
            QByteArray fullMessage = header.toUtf8() + fileData;

            // Send data over socket
            socket->write(fullMessage);
            socket->waitForBytesWritten();

            qDebug() << "Sent file:" << filePath << " (Size: " << fileSize << " bytes)";
        }
        else
        {
            qDebug() << "Failed to open file:" << filePath;
        }
    }
    else
    {
        qDebug() << "Cannot send file, socket not connected!";
    }
}

void Network::onConnected()
{
    qDebug() << "Successfully connected to the server!";
}

void Network::onMessageReceived()
{
    QString data = socket->readAll();
    qDebug() << "Received from server:" << data;
}

void Network::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error occurred:" << socket->errorString();
}
