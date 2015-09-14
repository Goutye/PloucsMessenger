#ifndef THREADCLIENT_H
#define THREADCLIENT_H

#include <QThread>
#include <QTcpSocket>
#include <QDebug>

class ThreadClient : public QThread
{
    Q_OBJECT
public:
    explicit ThreadClient(int id, QObject *parent = 0);
    void run();
    ~ThreadClient();

signals:
    void error(QTcpSocket::SocketError socketError);
    void bytesReceived(QByteArray data);
    void pmReceived(int targetID, QByteArray data);
    void disconnection(int id);
    void connection(int id, QString pseudo);

public slots:
    void readyRead();
    void disconnected();
    void write(QByteArray data, int id);

private:
    QTcpSocket *socket;
    int socketDescriptor;
};

#endif // THREADCLIENT_H
