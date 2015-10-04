#ifndef DISPLAYCHAT_H
#define DISPLAYCHAT_H
#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include "soundmanager.h"
#include "emoticons.h"

class DisplayChat : public QTextBrowser
{
    Q_OBJECT
public:
    explicit DisplayChat(Emoticons* em, QWidget *parent = 0);
    ~DisplayChat();
    void setId(int id);
    int id();

signals:
    void newNotification(int idUser, QString msg);
    void playSound(SoundManager::Name name);

protected:
    void mousePressEvent(QMouseEvent *ev);

public slots:
    void newMessage(QString data);
    void newMessage(QString data, int id);

private:
    void setNotification(QString msg);

    int _id = -1;
    QWidget *tabsWidget;
    Emoticons *em;
};

#endif // DISPLAYCHAT_H
