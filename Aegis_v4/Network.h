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

private slots:
    void onConnected();
    void onMessageReceived();
    void onErrorOccurred(QAbstractSocket::SocketError error);

private:
    QTcpSocket *socket;
    QString serverIP;
    int serverPort;

signals:
    void userIdReceived(const QString &userId); // ID utilizator
    void fileDownloaded(const QString &fileName); // Fisier descărcat
    void fileListReceived(const QString &data); // Lista de fișiere primită
};

#endif // NETWORK_H
