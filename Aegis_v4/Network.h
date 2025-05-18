#ifndef NETWORK_H
#define NETWORK_H

#include <QTcpSocket>
#include <QObject>

class Network : public QObject
{
    Q_OBJECT

public:
    explicit Network(QObject *parent = nullptr);

    void connectToServer();    // No need for parameters, uses stored IP/Port
    void sendMessage(const QString &message);
    void setServerDetails(const QString &ip, int port);  // Allows setting IP & Port dynamically
    void sendFile(const QString &filePath, const QString &idUser);
    void saveFile(const QByteArray &fileContent);

private slots:
    void onConnected();
    void onMessageReceived();
    void onErrorOccurred(QAbstractSocket::SocketError error);
    void onFileReceived(const QByteArray &message);

private:
    QTcpSocket *socket;
    QString serverIP;
    int serverPort;
    int expectedFileSize; // 🔹 Variabilă membru pentru dimensiunea fișierului
    QByteArray receivedFileData;

signals:
    void userIdReceived(const QString &userId); // ID utilizator
    void fileDownloaded(const QString &fileName); // Fisier descărcat
    void fileListReceived(const QString &data); // Lista de fișiere primită
};

#endif // NETWORK_H
