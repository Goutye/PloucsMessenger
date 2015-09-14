#include "socketclient.h"
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QCoreApplication>
#include <QTime>

SocketClient::SocketClient(QString pseudo = "", QObject *parent) : QObject(parent)
{
    this->pseudo = pseudo;
    this->url = QUrl("http://www.goutye.com/ploucs/index.php");
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

void SocketClient::start()
{
    manager = new QNetworkAccessManager;
    manager->setCookieJar(new QNetworkCookieJar);
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

    qDebug() << "Connecting...";

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

    QNetworkReply *reply =  manager->get(request);
    connect(reply, SIGNAL(finished()), this, SLOT(connected()));
}

void SocketClient::connected()
{
    qDebug() << "Connected!";
    write(QString("con:" + pseudo).toUtf8());
}

void SocketClient::disconnected()
{
    qDebug() << "Disconnected!";
}

void SocketClient::replyFinished(QNetworkReply* reply)
{
    qDebug() << "Reading...";
    QString s;

    while((s = QString(reply->readLine())) > 0 )
    {
        if (s.compare("NO") || s.compare("OK")) {
            qDebug() << s;
        }
        else {
            s.remove(s.size()-1, 1);
            qDebug() << s;
            QString prefix = s.split(":").at(0);
            s.remove(0, prefix.size() + 1);

            if (prefix.compare("msg") == 0)
            {
                QStringList sl = s.split(":");
                s = users.value(sl.at(0).toInt()) + ":" + sl.at(1);
                qDebug() << s;
                emit newMessage(s);
            }
            else if (prefix.compare("pm") == 0)
            {
                qDebug() << s;
                QStringList sl = s.split(":");
                int target = sl.at(0).toInt();
                s = users.value(target) + ":" + sl.at(1);
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

void SocketClient::post(QString data)
{
    write(QString("msg:" + data).toLatin1());
}

void SocketClient::post(QString data, int id)
{
    write(QString("pm:%1:" + data).arg(id).toLatin1());
}

bool SocketClient::write(QByteArray data)
{
    data.append('\n');

    QUrlQuery postData;
    postData.addQueryItem("cmd", QString(data));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,
        "application/x-www-form-urlencoded");

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
