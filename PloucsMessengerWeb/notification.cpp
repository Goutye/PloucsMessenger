#include <QApplication>
#include "notification.h"
#include "mainwindow.h"

Notification::Notification(QWidget *parent, QWidget *mainWindow, TabsArea *tabs) : QTextBrowser(parent), mainWindow(mainWindow), tabs(tabs)
{
    setReadOnly(true);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

Notification::~Notification()
{

}

void Notification::setId(int id)
{
    userId = id;
}

void Notification::mousePressEvent(QMouseEvent *ev)
{
    MainWindow *m = (MainWindow*) mainWindow;
    QApplication::setActiveWindow(m);
    tabs->setCurrentIdDC(userId);
}
