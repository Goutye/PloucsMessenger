#include <QUrl>
#include <QRegExp>
#include "displaychat.h"
#include <QGuiApplication>
#include <QTextDocument>
#include <qDebug>
#include <QTabBar>
#include "tabsarea.h"

DisplayChat::DisplayChat(Emoticons *em, QWidget *parent) : QTextBrowser(parent), em(em)
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

    if (tabs->currentIndex() != idTab || QGuiApplication::applicationState() < 4) {
        if (!tabs->notify(idTab)) {
            tabs->setNotify(idTab, true);
            qDebug() << QString("NOTIF " + msg + " %1").arg(_id);
            emit newNotification(_id, msg);
        }

        if (msg.split(":").count() > 1)
            emit playSound(SoundManager::newMessage);
    }
}

void DisplayChat::newMessage(QString data)
{
    data = data.toHtmlEscaped();
    qDebug() << "Message received: " + data;
    QStringList list = data.split(":");
    moveCursor(QTextCursor::End);
    if (list.count() > 1) {
        QString message;
        insertHtml("<br><b style='font-family: Roboto;font-size:15px;color:#82B1FF;'>"+ list.at(0) +": </b>");

        bool hasEmoteBefore = false;

        QRegExp regex("\\d+");
        QString wholeText = "<span style='font-family: Roboto;font-size:15px;'> "+ list.at(1);

        for (int i = 2; i < list.count(); ++i) {
            if (i != list.count() - 1 && regex.exactMatch(list.at(i))) {
                wholeText += "</span>";
                insertHtml(wholeText);
                qDebug() << wholeText;
                wholeText = "<span style='font-family: Roboto;font-size:15px;'>";
                if (em->exist(list.at(i))) {
                    QTextCursor c = textCursor();
                    c.insertImage(em->emote(list.at(i))->toImage());
                } else {
                    em->loadEmoticon(list.at(i), textCursor());
                }
                hasEmoteBefore = true;
            } else {

                if (!hasEmoteBefore) {
                    message = ":";
                } else {
                    message = "";
                }
                wholeText += message + list.at(i);
                hasEmoteBefore = false;
            }
        }
        wholeText += "</span>";
        insertHtml(wholeText);
        qDebug() << wholeText;
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
    data = data.toHtmlEscaped();
    if (id == _id) {
        qDebug() << "Message received: " + data + " " + id;
        QStringList list = data.split(":");
        moveCursor(QTextCursor::End);
        if (list.count() > 1) {
            QString message;
            insertHtml("<br><b style='font-family: Roboto;font-size:15px;color:#82B1FF;'>"+ list.at(0) +": </b>");

            bool hasEmoteBefore = false;

            QRegExp regex("\\d+");
            QString wholeText = "<span style='font-family: Roboto;font-size:15px;'> "+ list.at(1);

            for (int i = 2; i < list.count(); ++i) {
                if (i != list.count() - 1 && regex.exactMatch(list.at(i))) {
                    wholeText += "</span>";
                    insertHtml(wholeText);
                    qDebug() << wholeText;
                    wholeText = "<span style='font-family: Roboto;font-size:15px;'>";
                    if (em->exist(list.at(i))) {
                        QTextCursor c = textCursor();
                        c.insertImage(em->emote(list.at(i))->toImage());
                    } else {
                        em->loadEmoticon(list.at(i), textCursor());
                    }
                    hasEmoteBefore = true;
                } else {

                    if (!hasEmoteBefore) {
                        message = ":";
                    } else {
                        message = "";
                    }
                    wholeText += message + list.at(i);
                    hasEmoteBefore = false;
                }
            }
            wholeText += "</span>";
            insertHtml(wholeText);
            qDebug() << wholeText;
            setNotification(data);
        }
        else {
            insertHtml("<br><span style='font-family: Roboto;font-size:15px;'>"+ data +"</span>");
            setNotification(data);
        }
        moveCursor(QTextCursor::End);
        setNotification(data);
    }
}

void DisplayChat::mousePressEvent(QMouseEvent *ev)
{
    QTextBrowser::mousePressEvent(ev);
    TabsArea *ta = ((TabsArea*) tabsWidget);
    ta->setNotify(ta->currentIndex(), false);
}

void DisplayChat::insertHtml(const QString &oldText)
{
    QString text = oldText;
    text.replace("&lt;br&gt;", "<br>");
    QRegExp validURLRegex("(?:https?|ftp)://\\S+");
    QRegExp spanRegex("</span>.*");
    int pos = 0;
    while ((pos = validURLRegex.indexIn(text, pos)) != -1) {
        QString urlString = text.mid(pos, validURLRegex.matchedLength());
        int i = spanRegex.indexIn(urlString, 0);
        if (i >= 0)
            urlString.remove(i, spanRegex.matchedLength());
        QUrl *url = new QUrl(urlString);
        if (url->isValid()) {
            QString urlHTML = QString("<a style='color:#EEEEEE;' href='"+ url->toString() +"'>") + url->toString() + "</a>";
            int nbReplacedChars = validURLRegex.matchedLength();
            if(spanRegex.matchedLength() > 0)
                nbReplacedChars += spanRegex.matchedLength();
            text.replace(pos, nbReplacedChars, urlHTML);
            pos += urlHTML.count();
        } else {
            pos += validURLRegex.matchedLength();
        }
    }
    QTextBrowser::insertHtml(text);
}
