#ifndef DISPLAYCHAT_H
#define DISPLAYCHAT_H
#include <QTextBrowser>
#include <QWidget>
#include <QString>

class DisplayChat : public QTextBrowser
{
    Q_OBJECT
public:
    explicit DisplayChat(QWidget *parent = 0);
    ~DisplayChat();
    void setId(int id);
    int id();

signals:

public slots:
    void newMessage(QString data);
    void newMessage(QString data, int id);

private:
    void setNotification();

    int _id = -1;
    QWidget *tabsWidget;
};

#endif // DISPLAYCHAT_H
