#include "Network.h"
#include <QDebug>

Network::Network(QObject *parent) : QObject(parent), serverIP("192.168.122.63"), serverPort(27015)
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
