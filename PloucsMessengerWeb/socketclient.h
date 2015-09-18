#ifndef SOCKETCLIENT_H
#define SOCKETCLIENT_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QAbstractSocket>
#include <QVector>
#include <QUrl>
#include <QTimer>
#include <QDir>

#define tokenFile "/resources/token.dat"

class SocketClient : public QObject
{
    Q_OBJECT
public:
    explicit SocketClient(QObject *parent = 0);
    ~SocketClient();
    void delay( int millisecondsToWait );
    void start(QString pseudo, QString password, bool autoLogin = false);
    bool write(QByteArray data);
    bool checkUpdate();

signals:
    void connection(int id, QString pseudo);
    void wrongPassword(QString error);
    void disconnection(int id);
    void newMessage(QString data);
    void newMessage(QString data, int id);
    void isConnected();
    void isDisconnected();
    void updateAvailable(bool);

public slots:
    void post(QString data);
    void post(QString data, int id);
    void connected();
    void disconnected();

private slots:
    void ping();
    void replyFinished(QNetworkReply* reply);
    void saveToken(QString s);

private:
    void addUser(int id, QString pseudo);
    void removeUser(int id);

    QString pseudo;
    QString password;
    bool autoLogin;
    QNetworkAccessManager *manager;
    QUrl url;
    QMap<int, QString> users;
    int ownID = 0;
    QTimer *timerPing;
};

#endif // SOCKETCLIENT_H
