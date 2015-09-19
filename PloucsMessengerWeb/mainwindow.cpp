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
#include <QCoreApplication>

#define DEFAULT_WIDTH 200
#define LABEL_BACKGROUND_ACTIVE_COLOR "#BBBBBB"
#define LABEL_BACKGROUND_INACTIVE_COLOR "#888888"

//#define CHECK_UPDATE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qsrand(time(0));

    socket = new SocketClient();

    connect(socket, SIGNAL(updateAvailable(bool)), this, SLOT(updateAvailable(bool)));
#ifdef CHECK_UPDATE
    socket->checkUpdate();
    qDebug() << "Ready for Release";
#endif

    connect(socket, SIGNAL(connection(int,QString)), this, SLOT(connection(int,QString)));
    connect(socket, SIGNAL(disconnection(int)), this, SLOT(disconnection(int)));
    connect(this, SIGNAL(post(QString)), socket, SLOT(post(QString)));
    connect(this, SIGNAL(post(QString,int)), socket, SLOT(post(QString,int)));
    connect(socket, SIGNAL(wrongPassword(QString)), this, SLOT(connectionUser(QString)));
    connect(this, SIGNAL(disconnectionUser()), socket, SLOT(disconnected()));
    connect(socket, SIGNAL(isConnected()), this, SLOT(isConnected()));
    connect(socket, SIGNAL(isDisconnected()), this, SLOT(isDisconnected()));

    window = new QWidget;

    setWindowFlags(Qt::FramelessWindowHint);

    tb = new MainToolBar(this);
    addToolBar(Qt::RightToolBarArea, tb);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(2,2,2,2);

    splitter = new QSplitter;
    left = new DisplayChat();
    leftLabel = new QLabel("Offline");
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLabel->setFixedHeight(30);
    leftLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.insert(0,qMakePair(leftLabel, left));
    middle = new DisplayChat();
    middle->setId(-1);
    middleLabel = new QLabel("Plouc's");
    middleLabel->setAlignment(Qt::AlignCenter);
    middleLabel->setFixedHeight(30);
    middleLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_ACTIVE_COLOR + "; }");
    chats.insert(1,qMakePair(middleLabel, middle));
    right = new DisplayChat();
    rightLabel = new QLabel("Offline");
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setFixedHeight(30);
    rightLabel->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.insert(2,qMakePair(rightLabel, right));

    connect(socket, SIGNAL(newMessage(QString)), middle, SLOT(newMessage(QString)));
    connect(socket, SIGNAL(newMessage(QString,int)), left, SLOT(newMessage(QString,int)));
    connect(socket, SIGNAL(newMessage(QString,int)), right, SLOT(newMessage(QString,int)));

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
    sizes.append(1);
    sizes.append(1);
    sizes.append(1);
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

#ifndef CHECK_UPDATE
    connectionUser();
    qDebug() << "\nDEBUG VERSION\n";
#endif
}

MainWindow::~MainWindow()
{

}

void MainWindow::updateAvailable(bool newUpdate)
{
    if (newUpdate) {
        fd = new FileDownloader(QUrl("http://www.goutye.com/ploucs/PloucsMessenger.exe"), this);
        connect(fd, SIGNAL(downloaded()), this, SLOT(updateSettingsApp()));
    } else {
        connectionUser();
    }
}

void MainWindow::updateSettingsApp()
{
    disconnect(fd, SIGNAL(downloaded()), this, SLOT(updateSettingsApp()));
    QFile filePMupdated(QCoreApplication::applicationFilePath() + ".update");
    filePMupdated.open(QFile::WriteOnly);
    filePMupdated.write(fd->downloadedData());
    filePMupdated.close();
    fd = new FileDownloader(QUrl("http://www.goutye.com/ploucs/settings.exe"), this);
    connect(fd, SIGNAL(downloaded()), this, SLOT(updatePM()));
}

void MainWindow::updatePM()
{
    QString program = QCoreApplication::applicationDirPath() + "/settings.exe";
    QFile fileSettingsupdated(QCoreApplication::applicationDirPath() + "/settings.exe.update");
    fileSettingsupdated.open(QFile::WriteOnly);
    fileSettingsupdated.write(fd->downloadedData());
    fileSettingsupdated.close();

    if (!QFile(program).exists() || QFile::remove(program)) {
        QFile(QCoreApplication::applicationDirPath() + "/settings.exe.update").rename(QCoreApplication::applicationDirPath() + "/settings.exe");
    }

#ifdef Q_OS_WIN
    int result = (int)::ShellExecuteA(0, "open", program.toUtf8().constData(), "update", 0, SW_SHOWNORMAL);
    if (SE_ERR_ACCESSDENIED == result)
    {
        // Requesting elevation
        result = (int)::ShellExecuteA(0, "runas", program.toUtf8().constData(), "update", 0, SW_SHOWNORMAL);
    }
    if (result <= 32)
    {
        // error handling
    } else {
        QTimer::singleShot(0, this, SLOT(close()));
    }
#else
    if (!QProcess::startDetached(program, "PM"))
    {
        // error handling
    } else {
        QTimer::singleShot(0, this, SLOT(close()));
    }
#endif
}

void MainWindow::connectionUser(QString error)
{
    if (error.isEmpty())
        error = "Connection to Ploucs messenger";

    QFile file(QCoreApplication::applicationDirPath() + tokenFile);
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
                    dialogLabel->setText("Le pseudo était vide.");
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

    QMap<int, QString> usersMap = socket->getUsers();
    UserAction *a;

    for (QMap<int, QString>::iterator it = usersMap.begin(); it != usersMap.end(); ++it) {
        a = new UserAction(it.value(), it.key());
        connect(a, SIGNAL(triggered(int,QString)), this, SLOT(userPM(int,QString)));
        tb->addUserToMenu(a);
        users.append(a);
    }
}

void MainWindow::isDisconnected()
{
    userIsDisconnected = true;
    QTimer::singleShot(0, this, SLOT(close()));
}

void MainWindow::connection(int id, QString pseudo)
{
    DisplayChat *current;
    qDebug() << "Connection!";

    //search if one is same id
    for (QMap<int, QPair<QLabel*, DisplayChat*> >::iterator it = chats.begin(); it != chats.end(); ++it) {
        current = it.value().second;

        if (current->id() == id) {
            current->setId(id);
            return;
        }
    }

    //search for an available spot (Case tabs => create a tab)
    for (QMap<int, QPair<QLabel*, DisplayChat*> >::iterator it = chats.begin(); it != chats.end(); ++it) {
        current = it.value().second;

        if (current->id() == 0) {
            current->setId(id);
            it.value().first->setText(pseudo);
            return;
        }
    }
}

void MainWindow::disconnection(int id)
{
    DisplayChat *current;
    QLabel *currentLabel;
    qDebug() << "Disconnection!";

    for (QMap<int, QPair<QLabel*, DisplayChat*> >::iterator it = chats.begin(); it != chats.end(); ++it) {
        current = it.value().second;
        currentLabel = it.value().first;

        if (current->id() == id) {
            current->setId(0);
            currentLabel->setText("Offline");
            return;
        }
    }
}

void MainWindow::userPM(int id, QString pseudo)
{
    if (left->id() == 0 && right->id() != id) {
        QString label = pseudo;
        if (!socket->isOnline(id)) label += " (Offline)";
        leftLabel->setText(label);
        left->setId(id);
    }
    else if (right->id() == 0 && left->id() != id) {
        QString label = pseudo;
        if (!socket->isOnline(id)) label += " (Offline)";
        rightLabel->setText(label);
        right->setId(id);
    }
    else if (left->id() == id) {
        left->setId(0);
        leftLabel->setText("Offline");
    } else if (right->id() == id) {
        right->setId(0);
        rightLabel->setText("Offline");
    }
    else {
        //nope
    }
}

void MainWindow::post()
{
    if (inputMsg->text().isEmpty())
        return;
    switch(idCurrent) {
        case 0:
            emit post(inputMsg->text(), left->id());
            break;
        case 2:
            emit post(inputMsg->text(), right->id());
            break;
        default:
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
    } while((idCurrent == 0 && left->id() == 0) || (idCurrent == 2 && right->id() == 0));

    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_ACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_ACTIVE_COLOR + "; }");
}

void MainWindow::nextChat()
{
    chats.value(idCurrent).first->setStyleSheet(QString("QLabel { background-color: ") + LABEL_BACKGROUND_INACTIVE_COLOR + "; }");
    chats.value(idCurrent).second->setStyleSheet(QString("QTextEdit { background-color: ") + TEXTEDIT_BACKGROUND_INACTIVE_COLOR + "; }");

    do {
        idCurrent = (idCurrent + 1) % 3;
    } while((idCurrent == 0 && left->id() == 0) || (idCurrent == 2 && right->id() == 0));

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
