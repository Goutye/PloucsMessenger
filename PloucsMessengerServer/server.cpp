#include "server.h"

Server::Server(QObject *parent) : QTcpServer(parent)
{
}

Server::~Server()
{
}

void Server::StartServer()
{
    if(!this->listen(QHostAddress::Any, 1234))
        qDebug() << "Server failed to start!";
    else
        qDebug() << "Server started!";
}

void Server::incomingConnection(int socketDescriptor)
{
    qDebug() << socketDescriptor << " Connecting...";
    ThreadClient *thread = new ThreadClient(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(bytesReceived(QByteArray)), this, SLOT(broadcast(QByteArray)));
    connect(thread, SIGNAL(pmReceived(int, QByteArray)), this, SLOT(privateMessage(int, QByteArray)));
    connect(this, SIGNAL(sendToClient(QByteArray,int)), thread, SLOT(write(QByteArray,int)));
    connect(thread, SIGNAL(connection(int, QString)), this, SLOT(connection(int,QString)));
    connect(thread, SIGNAL(disconnection(int)), this, SLOT(disconnection(int)));
    thread->start();
    threads.insert(socketDescriptor, thread);

    QByteArray users = QByteArray();
    for (QMap<int, QString>::iterator it = pseudos.begin(); it != pseudos.end(); ++it)
        users.append(QString(";%1:").arg(it.key()) + it.value());
    if (users.size() > 0)
    {
        users.remove(0, 1);
        users.insert(0, "list:");
        privateMessage(socketDescriptor, users);
    }
}

void Server::connection(int id, QString pseudo)
{
    pseudos.insert(id, pseudo);
    qDebug() << QString("Added:") + pseudo;
}

void Server::disconnection(int id)
{
    threads.remove(id);
    pseudos.remove(id);
    qDebug() << QString("Removed %1").arg(id);
}

void Server::broadcast(QByteArray data)
{
    for (QMap<int, ThreadClient*>::iterator it = threads.begin(); it != threads.end(); ++it)
    {
        qDebug() << "Received:" << data << it.key();
        emit sendToClient(data, it.key());
    }
}

void Server::privateMessage(int toId, QByteArray data)
{
    qDebug() << "ReceivedPM: " << data;
    emit sendToClient(data, toId);
}
