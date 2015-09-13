#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include "socketclient.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void connection(int id, QString pseudo);
    void disconnection(int id);
    void newMessage(QString data);
    void newMessage(QString data, int id);

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

    int idLeft = 0;
    int idRight = 0;

    int idCurrent = 1;
};

#endif // MAINWINDOW_H
