#ifndef DISPLAYCHAT_H
#define DISPLAYCHAT_H
#include <QTextBrowser>
#include <QWidget>
#include <QString>

#define TEXTEDIT_BACKGROUND_ACTIVE_COLOR "#DDDDDD"
#define TEXTEDIT_BACKGROUND_INACTIVE_COLOR "#BBBBBB"

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
    int _id = 0;
};

#endif // DISPLAYCHAT_H
