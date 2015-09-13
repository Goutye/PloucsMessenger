#include "socketclient.h"

SocketClient::SocketClient(QString pseudo = "", QObject *parent) : QObject(parent)
{
    this->pseudo = pseudo;
}

SocketClient::~SocketClient()
{

}

void SocketClient::start()
{
    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    qDebug() << "Connecting...";

    socket->connectToHost("25.122.33.4", 1234);

    if(!socket->waitForDisconnected(1000))
        qDebug() << "Error: " << socket->errorString();
}

void SocketClient::connected()
{
    qDebug() << "Connected!";
    write(QString("con:" + pseudo).toLatin1());
}

void SocketClient::disconnected()
{
    qDebug() << "Disconnected!";
}

void SocketClient::bytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes;
}

void SocketClient::readyRead()
{
    qDebug() << "Reading...";
    QString s;

    while((s = QString(socket->readLine())) > 0 )
    {
        s.remove(s.size()-1, 1);
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

void SocketClient::post(QString data)
{
    write(QString("msg:" + data).toLatin1());
}

void SocketClient::post(QString data, int id)
{
    write(QString("pm:%1:" + data).arg(id).toLatin1());
}

qint64 SocketClient::write(QByteArray data)
{
    data.append('\n');
    return socket->write(data);
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
