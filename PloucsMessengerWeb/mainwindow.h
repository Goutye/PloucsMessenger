#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSplitter>
#include <QTextEdit>
#include <QTextBrowser>
#include <QLabel>
#include <QMouseEvent>
#include "socketclient.h"
#include "filedownloader.h"

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
    void newMessage(QString data);
    void newMessage(QString data, int id);
    void post();
    void nextChat();
    void prevChat();
    void connectionUser(QString error = 0);
    void isConnected();
    void isDisconnected();
    void updateAvailable(bool b);
    void updateSettingsApp();
    void updatePM();

signals:
    void post(QString data);
    void post(QString data, int id);
    void disconnectionUser();

protected:
    void closeEvent(QCloseEvent *event);

private slots:


private:
    SocketClient *socket;
    QWidget *window;
    QTextBrowser *left;
    QLabel *leftLabel;
    QTextBrowser *middle;
    QLabel *middleLabel;
    QTextBrowser *right;
    QLabel *rightLabel;
    QLineEdit *inputMsg;
    QSplitter *splitter;
    FileDownloader *fd;

    int idLeft = 0;
    int idRight = 0;

    int idCurrent = 1;

    QString pseudo;
    QMap< int, QPair<QLabel*, QTextEdit*> > chats;
    bool userIsDisconnected = true;

    int  m_nMouseClick_X_Coordinate = 0;
    int  m_nMouseClick_Y_Coordinate = 0;
    bool tokenConnectionFailed = false;
};

#endif // MAINWINDOW_H
