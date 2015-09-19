#include "displaychat.h"

#include <qDebug>

DisplayChat::DisplayChat(QWidget *parent) : QTextBrowser(parent)
{
    setReadOnly(true);
    setOpenExternalLinks(true);
    setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");
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

void DisplayChat::newMessage(QString data)
{
    qDebug() << "Message received: " + data;
    moveCursor(QTextCursor::End);
    insertHtml("<br />" + data);
    moveCursor(QTextCursor::End);
}

void DisplayChat::newMessage(QString data, int id)
{
    if (id == _id) {
        qDebug() << "Message received: " + data + " " + id;
        moveCursor(QTextCursor::End);
        insertHtml("<br />" + data);
        moveCursor(QTextCursor::End);
    }
}
