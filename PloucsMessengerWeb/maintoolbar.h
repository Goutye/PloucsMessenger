#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QProcess>
#include <QMenu>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>
#endif

class MainToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MainToolBar(QWidget *parent = 0);
    ~MainToolBar();
    void addUserToMenu(QAction * a);
    void removeUserToMenu(QAction *a);

signals:

public slots:

private slots:
    void setStartup();
    void error(QProcess::ProcessError error);

private:
    QMenu *menu;
    QMenu *usersMenu;
};

#endif // MAINTOOLBAR_H
