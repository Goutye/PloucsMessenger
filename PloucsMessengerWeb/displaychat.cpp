#include "displaychat.h"

#include <qDebug>
#include <QTabBar>
#include "tabsarea.h"

DisplayChat::DisplayChat(QWidget *parent) : QTextBrowser(parent)
{
    tabsWidget = parent;
    setReadOnly(true);
    setOpenExternalLinks(true);
    setStyleSheet("QTextBrowser { padding:10px; font-family: Roboto; }");
}

DisplayChat::~DisplayChat()
{

}

void DisplayChat::setId(int id)
{
    _id = id;
}

int DisplayChat::id()
{
    return _id;
}

void DisplayChat::setNotification(QString msg)
{
    TabsArea *tabs = (TabsArea *) tabsWidget;

    int idTab = -1;
    for (int i = 0; i < tabs->count(); ++i) {
        if (tabs->idDC(i) == _id) {
            idTab = i;
            break;
        }
    }

    if (idTab == -1)
        return;

    if (tabs->currentIndex() != idTab && !tabs->notify(idTab)) {
        tabs->setNotify(idTab, true);
        qDebug() << QString("NOTIF " + msg + " %1").arg(_id);
        emit newNotification(_id, msg);
    }
}

void DisplayChat::newMessage(QString data)
{
    qDebug() << "Message received: " + data;
    QStringList list = data.split(":");
    moveCursor(QTextCursor::End);
    if (list.count() > 1) {
        QString message = list.at(1);
        for (int i = 2; i < list.count(); ++i)
            message += ":" + list.at(i);
        insertHtml("<br><b style='font-family: Roboto;font-size:15px;color:#448AFF;'>"+ list.at(0) +": </b><span style='font-family: Roboto;font-size:15px;'>"+ message +"</span>");
        setNotification(data);
    }
    else {
        insertHtml("<br><span style='font-family: Roboto;font-size:15px;'>"+ data +"</span>");
        setNotification(data);
    }
    moveCursor(QTextCursor::End);
}

void DisplayChat::newMessage(QString data, int id)
{
    if (id == _id) {
        qDebug() << "Message received: " + data + " " + id;
        QStringList list = data.split(":");
        moveCursor(QTextCursor::End);
        if (list.count() > 1) {
            QString message = list.at(1);
            for (int i = 2; i < list.count(); ++i)
                message += ":" + list.at(i);
            insertHtml("<br><b style='font-family: Roboto;font-size:15px;color:#448AFF;'>"+ list.at(0) +": </b><span style='font-family: Roboto;font-size:15px;'>"+ message +"</span>");
            setNotification(list.at(1));
        }
        else {
            insertHtml("<br><span style='font-family: Roboto;font-size:15px;'>"+ data +"</span>");
            setNotification(data);
        }
        moveCursor(QTextCursor::End);
        setNotification(data);
    }
}
