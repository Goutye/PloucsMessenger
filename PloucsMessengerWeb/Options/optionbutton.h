#ifndef OPTIONBUTTON_H
#define OPTIONBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QProcess>
#include <QMenu>
#include <QAction>
#include <QMap>

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
    void initOptions();

signals:
    void mute(bool);
    void minimize();

public slots:
    void saveOptions();

private slots:
    void emitMinimize();
    void emitMute(bool b);
    void setStartup();
    void error(QProcess::ProcessError error);

private:
    QMenu *menu;
    QMenu *usersMenu;
    QMap<QString, QAction*> actionsINI;
};

#endif // OPTIONBUTTON_H
