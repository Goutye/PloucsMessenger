#ifndef DISPLAYCHAT_H
#define DISPLAYCHAT_H
#include <QWidget>
#include <QString>
#include <QTextBrowser>
#include "soundmanager.h"

class DisplayChat : public QTextBrowser
{
    Q_OBJECT
public:
    explicit DisplayChat(QWidget *parent = 0);
    ~DisplayChat();
    void setId(int id);
    int id();

signals:
    void newNotification(int idUser, QString msg);
    void playSound(SoundManager::Name name);

public slots:
    void newMessage(QString data);
    void newMessage(QString data, int id);

private:
    void setNotification(QString msg);

    int _id = -1;
    QWidget *tabsWidget;
};

#endif // DISPLAYCHAT_H
