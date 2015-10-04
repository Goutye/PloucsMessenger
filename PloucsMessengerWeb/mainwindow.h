#include <QPushButton>
#include "Options/windowbuttons.h"
#include "Tabs/tabsarea.h"
#include "soundmanager.h"
#include "inputtextedit.h"
#include "emoticons.h"
#include <QAction>
#include <QList>
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QMouseEvent>
#include "socketclient.h"
#include "filedownloader.h"
#include "Options/optionbutton.h"
#include "Options/useraction.h"
#include "Tabs/displaychat.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

public slots:
    void connection(int id, QString pseudo);
    void disconnection(int id);
    void post();
    void connectionUser(QString error = 0);
    void isConnected();
    void isDisconnected();
    void updateAvailable(bool b);
    void updateSettingsApp();
    void updatePM();
    void userPM(int id, QString pseudo);
    void getFont();
    void notified(int userId, QString msg);

signals:
    void post(QString data);
    void post(QString data, int id);
    void disconnectionUser();

protected:
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *e);

private slots:


private:
    void addTab(int id, QString pseudo);

    SocketClient *socket;
    QWidget *window;
    DisplayChat *left;
    QLabel *leftLabel;
    DisplayChat *middle;
    QLabel *middleLabel;
    DisplayChat *right; //#313131
    QLabel *rightLabel;
    InputTextEdit *inputMsg;
    QSplitter *splitter;
    FileDownloader *fd;
    SoundManager *sm;

    TabsArea *tabs;
    OptionButton *optionButton;
    WindowButtons *windowButtons;

    QList<UserAction *> users;

    QString pseudo;
    QMap<int, DisplayChat*> chats;
    bool userIsDisconnected = true;

    int  m_nMouseClick_X_Coordinate = 0;
    int  m_nMouseClick_Y_Coordinate = 0;
    bool tokenConnectionFailed = false;

    QTextEdit *notif;
    QWidget *notifWindow;
    QTimer *notifTimer;

    Emoticons *em;
};

#endif // MAINWINDOW_H
