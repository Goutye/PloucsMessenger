#include "displaychat.h"

#include <qDebug>
#include <QTabBar>
#include "tabsarea.h"

DisplayChat::DisplayChat(QWidget *parent) : QTextBrowser(parent)
{
    tabsWidget = parent;
    setReadOnly(true);
    setOpenExternalLinks(true);
    setStyleSheet("QTextEdit { padding:10px; }");
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

void DisplayChat::setNotification()
{
    TabsArea *tabs = (TabsArea *) tabsWidget;
    QTabBar *tabBar = tabs->findChild<QTabBar *>();

    int idTab = -1;
    for (int i = 0; i < tabs->count(); ++i) {
        if (tabs->idDC(i) == _id) {
            idTab = i;
            break;
        }
    }

    if (idTab == -1)
        return;

    if (tabs->currentIndex() != idTab) {
        tabs->setNotify(idTab, true);
    }
}

void DisplayChat::newMessage(QString data)
{
    qDebug() << "Message received: " + data;
    QStringList list = data.split(":");
    moveCursor(QTextCursor::End);
    if (list.count() > 1)
        insertHtml("<br><span style='font-family: Roboto-Bold;font-size:12px;color:#448AFF;'>"+ list.at(0) +": </span><span style='font-family: Roboto-Light;font-size:12px;'>"+ list.at(1) +"</span>");
    else
        insertHtml("<br><span style='font-family: Roboto-Light;font-size:12px;'>"+ data +"</span>");
    moveCursor(QTextCursor::End);
    setNotification();
}

void DisplayChat::newMessage(QString data, int id)
{
    if (id == _id) {
        qDebug() << "Message received: " + data + " " + id;
        moveCursor(QTextCursor::End);
        insertHtml("<br />" + data);
        moveCursor(QTextCursor::End);
        setNotification();
    }
}
