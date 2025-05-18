#include "Network.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QStandardPaths>
#include <QDir>
#include <QProcess>

// deschidere fisier
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

            // Extract file name and calculate individual sizes
            QString fileName = QFileInfo(filePath).fileName();
            QString fileNameLength = QString::number(fileName.toUtf8().size());
            QString fileSize = QString::number(fileData.size());

            // Create initial header (before adding total size)
            QString header = "4:" + fileSize + ":" + idUser +":"+ fileNameLength+ ":" + fileName;

            // Combine header and file data
            QByteArray KitMessage = header.toUtf8();

            // Send data over socket
            socket->write(KitMessage);
            socket->waitForBytesWritten();
            QThread::msleep(100);
            socket->write(fileData);
            socket->waitForBytesWritten();
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
    QByteArray data = socket->readAll();
    qDebug() << "Received from server: "<<data;

    if (data.startsWith("1:")) // Autentificare
    {
        QList<QByteArray> parts = data.split(':');
        if (parts.size() > 1) {
            QString userIdString = QString::fromUtf8(parts[1]);
            emit userIdReceived(userIdString);
        }
    }
    else if (data.startsWith("5:")) // Mesaj de început pentru transferul fișierului
    {
        qDebug() << "Detected file transfer message. Forwarding data to onFileReceived().";
        onFileReceived(data); // Trimitem mesajul complet ca `QByteArray`
    }
}

void Network::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error occurred:" << socket->errorString();
}


void Network::onFileReceived(const QByteArray &message)
{
    qDebug() << "onFileReceived called. Raw received data size: " << message.size();

    // Verificăm delimitatorii ':'
    int separatorIndex1 = message.indexOf(':');
    if (separatorIndex1 == -1) {
        qDebug() << "Invalid format! Missing first ':' delimiter.";
        return;
    }

    // Extragem codul protocolului
    QByteArray protocolCode = message.left(separatorIndex1).trimmed();

    // Extragem restul datelor
    QByteArray remainingData = message.mid(separatorIndex1 + 1);

    int separatorIndex2 = remainingData.indexOf(':');
    if (separatorIndex2 == -1) {
        qDebug() << "Invalid format! Missing second ':' delimiter.";
        return;
    }

    // Extragem dimensiunea corectă a fișierului
    QByteArray fileSizeBytes = remainingData.left(separatorIndex2).trimmed();
    QByteArray fileContent = remainingData.mid(separatorIndex2 + 1);

    qDebug() << "Extracted - Protocol:" << protocolCode << ", File size:" << fileSizeBytes;

    bool ok = false;
    int expectedFileSize = fileSizeBytes.toInt(&ok);
    if (!ok || expectedFileSize <= 0) {
        qDebug() << "Invalid file size extracted!";
        return;
    }

    qDebug() << "Expected file size set to:" << expectedFileSize;
    qDebug() << "Extracted content size:" << fileContent.size();

    if (fileContent.size() < expectedFileSize) {
        qDebug() << "Error: File content size is smaller than expected. Received:" << fileContent.size() << ", Expected:" << expectedFileSize;
        return;
    }

    // Salvare fișier - folosim `QIODevice::WriteOnly | QIODevice::Unbuffered`
    QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    QString fullFilePath = QDir::cleanPath(downloadPath + "/test.txt");
    qDebug() << "Attempting to save file to:" << fullFilePath;

    QFile file(fullFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Unbuffered)) {
        qDebug() << "Failed to open file for writing! Error:" << file.errorString();
        return;
    }

    qint64 bytesWritten = file.write(fileContent);
    file.close();

    if (bytesWritten != expectedFileSize) {
        qDebug() << "Error: Mismatch in written file size!";
        return;
    }

    qDebug() << "File successfully saved to:" << fullFilePath;
    emit fileDownloaded(QString::fromUtf8("broasca.txt"));

    // Deschide automat fișierul
    if (!QDesktopServices::openUrl(QUrl::fromLocalFile(fullFilePath))) {
        qDebug() << "Failed to open file:" << fullFilePath;
    } else {
        qDebug() << "File opened successfully.";
    }

}
