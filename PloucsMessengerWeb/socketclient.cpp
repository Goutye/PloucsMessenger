#include "socketclient.h"
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QCoreApplication>
#include <QTime>

SocketClient::SocketClient(QObject *parent) : QObject(parent)
{
    this->url = QUrl("http://www.goutye.com/ploucs/index.php");
    manager = new QNetworkAccessManager;
    manager->setCookieJar(new QNetworkCookieJar);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}

SocketClient::~SocketClient()
{

}

void SocketClient::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}

void SocketClient::start(QString pseudo, QString password)
{
    this->pseudo = pseudo;
    this->password = password;

    qDebug() << "Connecting...";

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QNetworkReply *reply =  manager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(connected()));
}

void SocketClient::connected()
{
    qDebug() << "Connected to the server! Login...";
    write(QString("con:" + pseudo + ":" + password).toUtf8());
    password = "";
    timerPing = new QTimer;
    connect(timerPing, SIGNAL(timeout()), this, SLOT(ping()));
}

void SocketClient::disconnected()
{
    qDebug() << "Disconnected!";
    write(QString("dcn").toUtf8());
}

void SocketClient::replyFinished(QNetworkReply* reply)
{
    QString s;

    while((s = QString(reply->readLine())) > 0 )
    {
        if (s.compare("NO") == 0) {
            qDebug() << s;
        }
        else if (s.compare("OK") == 0) {

        }
        else if (s.compare("EXIT") == 0) {
            qDebug() << s;
            timerPing->stop();
            emit isDisconnected();
        }
        else if (s.compare("BAD_PWD") == 0) {
            qDebug() << s;
            emit wrongPassword("Wrong combination of pseudo and password.");
            return;
        }
        else {
            s.remove(s.size()-1, 1);
            qDebug() << s;
            QString prefix = s.split(":").at(0);
            s.remove(0, prefix.size() + 1);

            if (prefix.compare("msg") == 0)
            {
                QStringList sl = s.split(":");
                s = users.value(sl.at(0).toInt());
                for (QStringList::iterator it = ++sl.begin(); it < sl.end(); it++)
                    s += ":" + *it;
                qDebug() << s;
                emit newMessage(s);
            }
            else if (prefix.compare("pm") == 0)
            {
                qDebug() << s;
                QStringList sl = s.split(":");
                int target = sl.at(0).toInt();
                s = users.value(target);
                for (QStringList::iterator it = ++sl.begin(); it < sl.end(); it++)
                    s += ":" + *it;
                emit newMessage(s, target);
            }
            else if (prefix.compare("con") == 0)
            {
                QStringList sl = s.split(":");
                addUser(sl.at(0).toInt(), sl.at(1));

                if (ownID == 0) {
                    ownID = sl.at(0).toInt();
                }
                else {
                    emit connection(sl.at(0).toInt(), sl.at(1));
                }

                emit newMessage(sl.at(1) + " is now Online.");
            }
            else if (prefix.compare("dcn") == 0)
            {
                emit newMessage(users.value(s.toInt()) + " is now Offline.");
                removeUser(s.toInt());
                emit disconnection(s.toInt());
            }
            else if (prefix.compare("list") == 0)
            {
                timerPing->start(100);
                qDebug() << s;
                emit isConnected();
                if (s.isEmpty())
                    return;
                QStringList users = s.split(";");
                for (QStringList::iterator it = users.begin(); it != users.end(); ++it)
                {
                    QStringList infos = QString(*it).split(":");
                    emit connection(infos.at(0).toInt(), infos.at(1));
                    addUser(infos.at(0).toInt(), infos.at(1));
                }
            }
            else
            {
                qDebug() << prefix;
            }
        }
    }
}

void SocketClient::ping()
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    manager->get(request);
    timerPing->start(100);  // @todo Adaptative ping
}

void SocketClient::post(QString data)
{
    write(QString("msg:" + data).toUtf8());
}

void SocketClient::post(QString data, int id)
{
    write(QString("pm:%1:" + data).arg(id).toUtf8());
}

bool SocketClient::write(QByteArray data)
{
    data.append('\n');
    qDebug() << QString("Write: ") + data;

    QUrlQuery postData;
    postData.addQueryItem("cmd", QString(data));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");
    request.setRawHeader("Accept-Charset", "utf-8");
    manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    return true;
}

void SocketClient::addUser(int id, QString pseudo)
{
    users.insert(id, pseudo);
    qDebug() << QString("User added: %1 -> ").arg(id) + pseudo;
}

void SocketClient::removeUser(int id)
{
    users.remove(id);
    qDebug() << QString("User removed: %1").arg(id);
}
