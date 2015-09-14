#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QSplitter>
#include <QTextEdit>
#include <QLabel>
#include <QMouseEvent>
#include "socketclient.h"

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

signals:
    void post(QString data);
    void post(QString data, int id);

private:
    SocketClient *socket;
    QWidget *window;
    QTextEdit *left;
    QLabel *leftLabel;
    QTextEdit *middle;
    QLabel *middleLabel;
    QTextEdit *right;
    QLabel *rightLabel;
    QLineEdit *inputMsg;
    QSplitter *splitter;

    int idLeft = 0;
    int idRight = 0;

    int idCurrent = 1;

    QString pseudo;
    QMap< int, QPair<QLabel*, QTextEdit*> > chats;

    int  m_nMouseClick_X_Coordinate = 0;
    int  m_nMouseClick_Y_Coordinate = 0;
};

#endif // MAINWINDOW_H
