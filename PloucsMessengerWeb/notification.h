#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QTextBrowser>
#include "Tabs/tabsarea.h"

class Notification : public QTextBrowser
{
    Q_OBJECT
public:
    explicit Notification(QWidget *parent, QWidget *mainWindow, TabsArea *tabs);
    ~Notification();
    void setId(int id);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *ev);

private:
    QWidget *mainWindow;
    TabsArea *tabs;
    int userId = 0;
};

#endif // NOTIFICATION_H
