#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>
#include <QObject>

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = nullptr);

    void connectToServer();
    void sendMessage(const QString &message);
    void sendFile(const QString &filePath, const QString &idUser);

private slots:
    void onConnected();
    void onMessageReceived();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket *socket;
    QString serverIP;
    int serverPort;

    // Pentru recepție fișier
    QByteArray fileBuffer;
    int expectedFileSize = 0;
    bool receivingFile = false;
    QString receivedFileName;

    void saveFile(const QByteArray &fileContent);

private:
    QByteArray key = "Cretzu_Cafe+Aegis";
    QByteArray xorEncrypt(const QByteArray &data, const QByteArray &key);


signals:
    void userIdReceived(const QString &userId);
    void fileDownloaded(const QString &fileName);
    void fileListReceived(const QString &data);

    void errorOccurred(const QString &message);
    void fileListReceived(const QStringList &fileNames);

    void fileDeleted();
    void fileSended();


};

#endif // NETWORK_H
