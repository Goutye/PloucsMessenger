#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QVector>
#include <QUrl>

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QString pseudo, QObject *parent = 0);
    ~SocketClient();
    void delay( int millisecondsToWait );
    void start();
    bool write(QByteArray data);

signals:
    void connection(int id, QString pseudo);
    void disconnection(int id);
    void newMessage(QString data);
    void newMessage(QString data, int id);

public slots:
    void post(QString data);
    void post(QString data, int id);
    void connected();
    void disconnected();
    void replyFinished(QNetworkReply* reply);

private:
    void addUser(int id, QString pseudo);
    void removeUser(int id);

    QString pseudo;
    QNetworkAccessManager *manager;
    QUrl url;
    QMap<int, QString> users;
    int ownID = 0;
};

#endif // SOCKETCLIENT_H
