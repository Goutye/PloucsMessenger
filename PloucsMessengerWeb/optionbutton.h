#ifndef OPTIONBUTTON_H
#define OPTIONBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QProcess>
#include <QMenu>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>
#endif

class OptionButton : public QToolButton
{
    Q_OBJECT
public:
    explicit OptionButton(QWidget *parent = 0);
    ~OptionButton();
    void addUserToMenu(QAction * a);
    void removeUserToMenu(QAction *a);

signals:
    void mute(bool);

public slots:

private slots:
    void emitMute(bool b);
    void setStartup();
    void error(QProcess::ProcessError error);

private:
    QMenu *menu;
    QMenu *usersMenu;
};

#endif // OPTIONBUTTON_H
