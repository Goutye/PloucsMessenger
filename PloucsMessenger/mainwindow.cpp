#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QSizeGrip>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    socket = new SocketClient();
    socket->start();
    window = new QWidget;

    setWindowFlags(Qt::FramelessWindowHint);


    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(2,2,2,2);

    QSplitter *splitter = new QSplitter;
    left = new QTextEdit();
    left->setDisabled(true);
    leftLabel = new QLabel("Offline");
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLabel->setFixedHeight(30);
    middle = new QTextEdit();
    middle->setDisabled(true);
    middleLabel = new QLabel("Plouc's");
    middleLabel->setAlignment(Qt::AlignCenter);
    middleLabel->setFixedHeight(30);
    right = new QTextEdit();
    right->setDisabled(true);
    rightLabel = new QLabel("Offline");
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setFixedHeight(30);

    QVBoxLayout *vSplitLayout = new QVBoxLayout;
    vSplitLayout->addWidget(leftLabel);
    vSplitLayout->addWidget(left);
    vSplitLayout->setContentsMargins(0, 0, 0, 0);
    vSplitLayout->setSpacing(2);

    QWidget *widgetV = new QWidget;
    widgetV->setLayout(vSplitLayout);
    splitter->addWidget(widgetV);

    vSplitLayout = new QVBoxLayout;
    vSplitLayout->setContentsMargins(0, 0, 0, 0);
    vSplitLayout->addWidget(middleLabel);
    vSplitLayout->addWidget(middle);
    vSplitLayout->setSpacing(2);

    widgetV = new QWidget;
    widgetV->setLayout(vSplitLayout);
    splitter->addWidget(widgetV);

    vSplitLayout = new QVBoxLayout;
    vSplitLayout->setContentsMargins(0, 0, 0, 0);
    vSplitLayout->addWidget(rightLabel);
    vSplitLayout->addWidget(right);
    vSplitLayout->setSpacing(2);

    widgetV = new QWidget;
    widgetV->setLayout(vSplitLayout);
    splitter->addWidget(widgetV);

    vLayout->addWidget(splitter, 5);

    inputMsg = new QLineEdit();
    inputMsg->setFixedHeight(20);

    vLayout->addWidget(inputMsg, 1);
    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setFixedHeight(5);
    vLayout->addWidget(sizeGrip, 0, Qt::AlignBottom | Qt::AlignRight);

    window->setLayout(vLayout);

    setCentralWidget(window);

    connect(socket, SIGNAL(connection(int,QString)), this, SLOT(connection(int,QString)));
    connect(socket, SIGNAL(disconnection(int)), this, SLOT(disconnection(int)));
    connect(socket, SIGNAL(newMessage(QString)), this, SLOT(newMessage(QString)));
    connect(socket, SIGNAL(newMessage(QString,int)), this, SLOT(newMessage(QString,int)));
    connect(this, SIGNAL(post(QString)), socket, SLOT(post(QString)));
    connect(this, SIGNAL(post(QString,int)), socket, SLOT(post(QString,int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::connection(int id, QString pseudo)
{
    qDebug() << "Connection!";
    if (idLeft == 0) {
        leftLabel->setText(pseudo);
        idLeft = id;
    }
    else if (idRight == 0) {
        rightLabel->setText(pseudo);
        idRight = id;
    }
}

void MainWindow::disconnection(int id)
{
    qDebug() << "Disconnection!";
    if (idLeft == id) {
        idLeft = 0;
        leftLabel->setText("Offline");
    }
    else if (idRight == id) {
        idRight = 0;
        rightLabel->setText("Offline");
    }
}

void MainWindow::newMessage(QString data)
{
    qDebug() << "Message received: " + data;
    middle->moveCursor(QTextCursor::End);
    middle->insertPlainText("\n" + data);
}

void MainWindow::newMessage(QString data, int id)
{
    qDebug() << "Message received: " + data + " " + id;
    if (id == idLeft) {
        left->moveCursor(QTextCursor::End);
        left->insertPlainText("\n" + data);
    }
    else if (id == idRight) {
        right->moveCursor(QTextCursor::End);
        right->insertPlainText("\n" + data);
    }
}
