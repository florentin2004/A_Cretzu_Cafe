#include "Network.h"
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QStandardPaths>

Network::Network(QObject *parent) : QObject(parent), serverIP("25.40.149.96"), serverPort(27015)
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
    QString data = socket->readAll();
    qDebug() << "Received from server:" << data;

    // Check if it's a login response that contains an ID
    if (data.startsWith("1:"))  // Assuming format "1:<id>"
    {
        QString userIdString = data.split(':').value(1);
        emit userIdReceived(userIdString); // Emit signal to MainWindow
    }
    if (data.startsWith("5:"))
        emit fileListReceived(data); // Semnal către MainWindow
}

void Network::onErrorOccurred(QAbstractSocket::SocketError error)
{
    qDebug() << "Socket error occurred:" << socket->errorString();
}


void Network::onFileReceived(const QString &requestedFileName)
{
    static bool waitingForFileSize = true;
    static int expectedFileSize = 0;
    static QByteArray receivedFileData;

    QByteArray data = socket->readAll(); // Citește toate datele disponibile
    qDebug() << "onFileReceived called. Requested file:" << requestedFileName;
    qDebug() << "Received chunk (" << data.size() << " bytes): " << data.left(100);

    if (waitingForFileSize)
    {
        qDebug() << "Waiting for file size. Raw data:" << data;
        int separatorIndex = data.indexOf(':');
        qDebug() << "Separator index:" << separatorIndex;
        if (separatorIndex == -1)
        {
            qDebug() << "Invalid format! Missing ':' delimiter.";
            return;
        }

        QByteArray sizeBytes = data.left(separatorIndex).trimmed();
        qDebug() << "Size bytes extracted:" << sizeBytes;
        bool ok = false;
        expectedFileSize = sizeBytes.toInt(&ok);
        qDebug() << "Conversion to int successful:" << ok << ", Expected file size:" << expectedFileSize;

        if (!ok || expectedFileSize <= 0)
        {
            qDebug() << "Invalid file size received!";
            waitingForFileSize = true; // Resetăm pentru a evita blocarea
            return;
        }

        waitingForFileSize = false;
        qDebug() << "Expected file size set to:" << expectedFileSize;

        // Salvează restul datelor din pachet (după ':')
        QByteArray initialData = data.mid(separatorIndex + 1);
        receivedFileData.append(initialData);
        qDebug() << "Initial payload size:" << initialData.size();
    }
    else
    {
        receivedFileData.append(data);
        qDebug() << "Appended data chunk. Total size so far:" << receivedFileData.size();
    }

    qDebug() << "Total bytes received so far: " << receivedFileData.size() << "/" << expectedFileSize;

    if (receivedFileData.size() >= expectedFileSize)
    {
        qDebug() << "Enough data received. Proceeding to save file.";
        if (requestedFileName.isEmpty())
        {
            qDebug() << "Error: No file name provided!";
            waitingForFileSize = true;
            expectedFileSize = 0;
            receivedFileData.clear();
            return;
        }

        QString downloadPath = QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
        QString fullFilePath = downloadPath + "/" + requestedFileName;
        qDebug() << "Attempting to save file to:" << fullFilePath;

        QFile file(fullFilePath);
        if (!file.open(QIODevice::WriteOnly))
        {
            qDebug() << "Failed to open file for writing! Path:" << fullFilePath << ", Error:" << file.errorString();
            waitingForFileSize = true;
            expectedFileSize = 0;
            receivedFileData.clear();
            return;
        }

        qint64 bytesWritten = file.write(receivedFileData);
        file.close();
        qDebug() << "Bytes written:" << bytesWritten;

        if (bytesWritten < expectedFileSize)
        {
            qDebug() << "Error: Only" << bytesWritten << "bytes written, expected" << expectedFileSize;
        }
        else
        {
            qDebug() << "File successfully saved to:" << fullFilePath;
            emit fileDownloaded(requestedFileName);
        }

        // Resetare variabile statice pentru următorul transfer
        waitingForFileSize = true;
        expectedFileSize = 0;
        receivedFileData.clear();
        qDebug() << "Reset static variables for next transfer.";
    }
    else
    {
        qDebug() << "Not enough data yet. Waiting for more.";
    }
}

