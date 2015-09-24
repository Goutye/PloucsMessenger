#include "tabsarea.h"
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
#include "maintoolbar.h"
#include "useraction.h"
#include "displaychat.h"

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

signals:
    void post(QString data);
    void post(QString data, int id);
    void disconnectionUser();

protected:
    void closeEvent(QCloseEvent *event);

private slots:


private:
    void addTab(int id, QString pseudo);

    SocketClient *socket;
    QWidget *window;
    DisplayChat *left;
    QLabel *leftLabel;
    DisplayChat *middle;
    QLabel *middleLabel;
    DisplayChat *right;
    QLabel *rightLabel;
    QLineEdit *inputMsg;
    QSplitter *splitter;
    FileDownloader *fd;
    MainToolBar *tb;

    TabsArea *tabs;

    QList<UserAction *> users;

    QString pseudo;
    QMap<int, DisplayChat*> chats;
    bool userIsDisconnected = true;

    int  m_nMouseClick_X_Coordinate = 0;
    int  m_nMouseClick_Y_Coordinate = 0;
    bool tokenConnectionFailed = false;
};

#endif // MAINWINDOW_H
