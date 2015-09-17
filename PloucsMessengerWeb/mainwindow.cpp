#include "mainwindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QShortcut>
#include <QKeySequence>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <time.h>
#include <QCryptographicHash>
#include <QCheckBox>

#include "maintoolbar.h"

#define LABEL_BACKGROUND_ACTIVE_COLOR "#BBBBBB"
#define LABEL_BACKGROUND_INACTIVE_COLOR "#888888"
#define TEXTEDIT_BACKGROUND_ACTIVE_COLOR "#DDDDDD"
#define TEXTEDIT_BACKGROUND_INACTIVE_COLOR "#BBBBBB"
#define DEFAULT_WIDTH 200

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qsrand(time(0));

    socket = new SocketClient();
    connect(socket, SIGNAL(connection(int,QString)), this, SLOT(connection(int,QString)));
    connect(socket, SIGNAL(disconnection(int)), this, SLOT(disconnection(int)));
    connect(socket, SIGNAL(newMessage(QString)), this, SLOT(newMessage(QString)));
    connect(socket, SIGNAL(newMessage(QString,int)), this, SLOT(newMessage(QString,int)));
    connect(this, SIGNAL(post(QString)), socket, SLOT(post(QString)));
    connect(this, SIGNAL(post(QString,int)), socket, SLOT(post(QString,int)));
    connect(socket, SIGNAL(wrongPassword(QString)), this, SLOT(connectionUser(QString)));
    connect(this, SIGNAL(disconnectionUser()), socket, SLOT(disconnected()));
    connect(socket, SIGNAL(isConnected()), this, SLOT(isConnected()));
    connect(socket, SIGNAL(isDisconnected()), this, SLOT(isDisconnected()));

    window = new QWidget;

    setWindowFlags(Qt::FramelessWindowHint);

    MainToolBar *tb = new MainToolBar(this);
    addToolBar(Qt::RightToolBarArea, tb);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(2,2,2,2);

    splitter = new QSplitter;
    left = new QTextEdit();
    left->setReadOnly(true);
    leftLabel = new QLabel("Offline");
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLabel->setFixedHeight(30);
    leftLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    left->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.insert(0,qMakePair(leftLabel, left));
    middle = new QTextEdit();
    middle->setReadOnly(true);
    middleLabel = new QLabel("Plouc's");
    middleLabel->setAlignment(Qt::AlignCenter);
    middleLabel->setFixedHeight(30);
    middleLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_ACTIVE_COLOR + "; }");
    middle->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_ACTIVE_COLOR + "; }");
    chats.insert(1,qMakePair(middleLabel, middle));
    right = new QTextEdit();
    right->setReadOnly(true);
    rightLabel = new QLabel("Offline");
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setFixedHeight(30);
    rightLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    right->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.insert(2,qMakePair(rightLabel, right));

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

    QList<int> sizes;
    sizes.append(0);
    sizes.append(1);
    sizes.append(0);
    splitter->setSizes(sizes);

    vLayout->addWidget(splitter, 5);

    inputMsg = new QLineEdit();
    inputMsg->setFixedHeight(20);
    vLayout->addWidget(inputMsg, 1);

    window->setLayout(vLayout);

    setCentralWidget(window);

    connect(inputMsg, SIGNAL(returnPressed()), this, SLOT(post()));

    QShortcut *changeWRight = new QShortcut(QKeySequence(Qt::Key_Tab), this, SLOT(nextChat()));
    QShortcut *changeWLeft = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), this, SLOT(prevChat()));
    QShortcut *close = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(close()));

    window->setAttribute(Qt::WA_TranslucentBackground);
    window->setStyleSheet("background:transparent;");
    setWindowOpacity(0.9);
    resize(DEFAULT_WIDTH, height());
    connectionUser();
}

MainWindow::~MainWindow()
{

}

void MainWindow::connectionUser(QString error)
{
    if (error.isEmpty())
        error = "Connection to Ploucs messenger";

    QFile file(QDir::currentPath() + tokenFile);
    if ((!tokenConnectionFailed) && file.open(QIODevice::ReadOnly)) {
        pseudo = QString(file.readLine());
        pseudo.remove(pseudo.size() - 1, 1);
        inputMsg->setFocus();
        socket->start(pseudo, QString(file.readLine()));
        tokenConnectionFailed = true;
    }
    else {
        QDialog dialog(this);
        QFormLayout form(&dialog);
        QLabel *dialogLabel = new QLabel(error);
        form.addRow(dialogLabel);

        QLineEdit *lineEditPseudo = new QLineEdit(&dialog);
        form.addRow("Pseudo:", lineEditPseudo);
        QLineEdit *lineEditPassword = new QLineEdit(&dialog);
        lineEditPassword->setEchoMode(QLineEdit::Password);
        form.addRow("Password:", lineEditPassword);
        QCheckBox *checkBox = new QCheckBox(&dialog);
        checkBox->setChecked(true);
        form.addRow("Auto-login:", checkBox);

        QDialogButtonBox buttonBox(QDialogButtonBox::Ok, Qt::Horizontal, &dialog);
        form.addRow(&buttonBox);
        QObject::connect(&buttonBox, SIGNAL(accepted()), &dialog, SLOT(accept()));
        QString password;

        do {
            if (dialog.exec() == QDialog::Accepted) {
                pseudo = lineEditPseudo->text();
                password = lineEditPassword->text();
                if (pseudo.isEmpty()) {
                    dialogLabel->setText("Pseudo field was empty.");
                }
            } else {
                QTimer::singleShot(0, this, SLOT(close()));
                return;
            }
        } while(pseudo.isEmpty());

        QCryptographicHash hash(QCryptographicHash::Sha3_512);
        hash.addData(password.toStdString().c_str(), password.length());
        QString result(hash.result());
        result.replace(":",";");
        socket->start(pseudo, result, checkBox->isChecked());
    }
}

void MainWindow::isConnected()
{
    userIsDisconnected = false;
}

void MainWindow::isDisconnected()
{
    userIsDisconnected = true;
    QTimer::singleShot(0, this, SLOT(close()));
}

void MainWindow::connection(int id, QString pseudo)
{
    qDebug() << "Connection!";
    if (idLeft == 0) {
        leftLabel->setText(pseudo);
        idLeft = id;
        QList<int> sizes;
        sizes.append(1);
        sizes.append(1);
        sizes.append(idRight != 0 ? 1 : 0);
        splitter->setSizes(sizes);
        int widthBlock = width() / (1 + (idRight != 0 ? 1 : 0));
        resize(widthBlock * (2 + (idRight != 0 ? 1 : 0)), height());
    }
    else if (idRight == 0) {
        rightLabel->setText(pseudo);
        idRight = id;
        QList<int> sizes;
        sizes.append(idLeft != 0 ? 1 : 0);
        sizes.append(1);
        sizes.append(1);
        splitter->setSizes(sizes);
        int widthBlock = width() / (1 + (idLeft != 0 ? 1 : 0));
        resize(widthBlock * (2 + (idLeft != 0 ? 1 : 0)), height());
    }
}

void MainWindow::disconnection(int id)
{
    qDebug() << "Disconnection!";
    if (idLeft == id) {
        idLeft = 0;
        leftLabel->setText("Offline");
        QList<int> sizes;
        sizes.append(0);
        sizes.append(1);
        sizes.append(idRight != 0 ? 1 : 0);
        splitter->setSizes(sizes);
        int widthBlock = width() / (2 + (idRight != 0 ? 1 : 0));
        resize(widthBlock * (1 + (idRight != 0 ? 1 : 0)), height());
    }
    else if (idRight == id) {
        idRight = 0;
        rightLabel->setText("Offline");
        QList<int> sizes;
        sizes.append(idLeft != 0 ? 1 : 0);
        sizes.append(1);
        sizes.append(0);
        splitter->setSizes(sizes);
        int widthBlock = width() / (2 + (idLeft != 0 ? 1 : 0));
        resize(widthBlock * (1 + (idLeft != 0 ? 1 : 0)), height());
    }
}

void MainWindow::newMessage(QString data)
{
    qDebug() << "Message received: " + data;
    middle->moveCursor(QTextCursor::End);
    middle->insertHtml("<br />" + data);
    middle->moveCursor(QTextCursor::End);
}

void MainWindow::newMessage(QString data, int id)
{
    qDebug() << "Message received: " + data + " " + id;
    if (id == idLeft) {
        left->moveCursor(QTextCursor::End);
        left->insertHtml("<br />" + data);
        left->moveCursor(QTextCursor::End);
    }
    else if (id == idRight) {
        right->moveCursor(QTextCursor::End);
        right->insertHtml("<br />" + data);
        right->moveCursor(QTextCursor::End);
    }
}

void MainWindow::post()
{
    if (inputMsg->text().isEmpty())
        return;
    if (idCurrent == 0) {
        emit post(inputMsg->text(), idLeft);
        newMessage(pseudo + ": " + inputMsg->text(), idLeft);
    }
    else if (idCurrent == 2) {
        emit post(inputMsg->text(), idRight);
        newMessage(pseudo + ": " + inputMsg->text(), idRight);
    }
    else {
        emit post(inputMsg->text());
    }

    inputMsg->setText("");
    qDebug() << "Posted!";
}

void MainWindow::prevChat()
{
    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");

    do {
        idCurrent = (idCurrent - 1) % 3;
        if (idCurrent < 0) idCurrent += 3;
    } while((idCurrent == 0 && idLeft == 0) || (idCurrent == 2 && idRight == 0));

    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_ACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_ACTIVE_COLOR + "; }");
}

void MainWindow::nextChat()
{
    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");

    do {
        idCurrent = (idCurrent + 1) % 3;
    } while((idCurrent == 0 && idLeft == 0) || (idCurrent == 2 && idRight == 0));

    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_ACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_ACTIVE_COLOR + "; }");
}

void MainWindow::mousePressEvent(QMouseEvent* event)
{
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void MainWindow::mouseMoveEvent(QMouseEvent* event)
{
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (userIsDisconnected) {
        qDebug() << "ACCEPTED";
        event->accept();
    } else {
        qDebug() << "IGNORE";
        emit disconnectionUser();
        event->ignore();
    }
}
