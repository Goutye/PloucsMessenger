#include "threadclient.h"

ThreadClient::ThreadClient(int id, QObject *parent) : QThread(parent)
{
    this->socketDescriptor = id;
    this->socket = new QTcpSocket();

    qDebug() << socketDescriptor << " Starting thread";

    if(!socket->setSocketDescriptor(this->socketDescriptor))
    {
        emit error(socket->error());
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);
    qDebug() << socketDescriptor << " Client connected";
}

ThreadClient::~ThreadClient()
{

}

void ThreadClient::run()
{
    exec();
}

void ThreadClient::readyRead()
{
    QByteArray data;

    while((data = socket->readLine()) > 0 )
    {
        data.remove(data.size()-1, 1);
        QString s = QString(data);
        QString prefix = s.split(":").at(0);

        if (prefix.compare("msg") == 0)
        {
            data.insert(4, QString("%1: ").arg(socketDescriptor));
            qDebug() << data;
            emit bytesReceived(data);
        }
        else if (prefix.compare("pm") == 0)
        {
            QString target = s.split(":").at(1);
            data.replace(prefix.size() + 1, target.size(), QString("%1").arg(socketDescriptor).toUtf8());
            qDebug() << data;
            emit pmReceived(target.toInt(), data);
        }
        else if (prefix.compare("con") == 0)
        {
            qDebug() << s << s.mid(prefix.size() + 1);
            emit connection(socketDescriptor, s.mid(prefix.size() + 1));
            data.insert(4, QString("%1:").arg(socketDescriptor));
            qDebug() << data;
            emit bytesReceived(data);
        }
        else
        {
            qDebug() << "Fail to compare " << prefix << QString(" %1").arg(prefix.compare("con"));
        }
    }
}

void ThreadClient::disconnected()
{
    socket->deleteLater();

    QByteArray msg = QByteArray("dcn:");
    msg.insert(4,QString("%1").arg(socketDescriptor));

    emit bytesReceived(msg);
    emit disconnection(socketDescriptor);
    exit(0);
}

void ThreadClient::write(QByteArray data, int id)
{
    qDebug() << data;
    if (id == socketDescriptor)
    {
        data.append('\n');
        socket->write(data);
    }
}
