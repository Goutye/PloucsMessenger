#ifndef SERVER_H
#define SERVER_H

#include <QVector>
#include <QPair>
#include <QDebug>
#include <QTcpServer>
#include "threadClient.h"

class Server : public QTcpServer
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    void StartServer();
    ~Server();

signals:
    void sendToClient(QByteArray data, int id);

public slots:
    void broadcast(QByteArray data);
    void privateMessage(int toId, QByteArray data);
    void connection(int id, QString pseudo);
    void disconnection(int id);

protected:
    void incomingConnection(int socketDescriptor);

private:

    QMap<int, ThreadClient*> threads;
    QMap<int, QString> pseudos;
};

#endif // SERVER_H
