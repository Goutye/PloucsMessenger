#include <QStackedLayout>
#include <QWindow>
#include "mainwindow.h"
#include "chatwidget.h"
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
#include <QFontDatabase>
#include <QApplication>
#include <QDesktopWidget>
#include <QtCore>

#define DEFAULT_WIDTH 400
#define LABEL_BACKGROUND_ACTIVE_COLOR "#BBBBBB"
#define LABEL_BACKGROUND_INACTIVE_COLOR "#888888"

#define CHECK_UPDATEw

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    qsrand(time(0));

    qRegisterMetaType<SoundManager::Name>("SoundManager::Name");
    sm = new SoundManager();
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

    window = new ChatWidget(this);

    setWindowFlags(Qt::FramelessWindowHint);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);

    tabs = new TabsArea(window);

    DisplayChat *dc = new DisplayChat(tabs);
    chats.insert(tabs->count(), dc);
    tabs->addTab(dc, "Plouc's");
    connect(socket, SIGNAL(newMessage(QString)), dc, SLOT(newMessage(QString)));
    connect(dc, SIGNAL(newNotification(int,QString)), this, SLOT(notified(int,QString)));
    connect(dc, SIGNAL(playSound(SoundManager::Name)), sm, SLOT(play(SoundManager::Name)));

    QStackedLayout *stackLayout = new QStackedLayout;
    stackLayout->setStackingMode(QStackedLayout::StackAll);
    int id = stackLayout->addWidget(tabs);

    optionButton = new OptionButton(tabs);
    connect( optionButton, SIGNAL(mute(bool)), sm, SLOT(setMute(bool)));

    windowButtons = new WindowButtons(tabs);
    connect(windowButtons, SIGNAL(close()), this, SLOT(close()));
    connect(windowButtons, SIGNAL(minimize()), this, SLOT(showMinimized()));

    vLayout->addLayout(stackLayout, 10);

    inputMsg = new InputTextEdit(window);
    inputMsg->setFixedHeight(60);
    ChatWidget *inputChat = new ChatWidget(window);
    QVBoxLayout *inputChatLayout = new QVBoxLayout;
    inputChat->setLayout(inputChatLayout);
    inputChatLayout->addWidget(inputMsg, 1);
    vLayout->addWidget(inputChat, 1);

    window->setLayout(vLayout);
    vLayout->setSpacing(0);
    window->setContentsMargins(0,0,0,0);
    setContentsMargins(0,0,0,0);

    setCentralWidget(window);

    connect(inputMsg, SIGNAL(returnPressed()), this, SLOT(post()));

    QShortcut *next = new QShortcut(QKeySequence(Qt::Key_Tab), tabs, SLOT(nextChat()));
    QShortcut *prev = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), tabs, SLOT(prevChat()));
    QShortcut *close = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(close()));

    window->setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QMainWindow { background-color: #202020; "
                                "border: 5px solid black;}"
                  "QMainWindow > QWidget { background-color: #202020; }"
                  "QMainWindow > QWidget > QWidget { background-color: #414141; }"
                  "QTextEdit { border-width: 2px;"
                              "border-style:solid;"
                              "border-color: #525252;"
                              "background-color: #414141;"
                              "color: #F4F4F4;}"
                  "QToolBar { border-color: #666666; }"
                  "QTabWidget QTextEdit { border: 0px solid black; "
                              "border-bottom: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                  "stop: 0 #2E2F2F, stop: 0.5 #272827, stop: 1.0 #222222);"
                              "background-color: #2F2F2F;"
                              "color: #FFFFFF;}"
                  "QMenu::item { background-color: #202020; "
                  "              color: #888888;"
                  "              border-color: #525252;"
                  "              padding: 2px 16px 2px 16px;}"
                  "QMenu { border: 0px solid #111111;}"
                  "QMenu::item::selected { color:#EEEEEE; }"
                  "QMenu::separator { background-color: #404040;"
                  "                   height: 2px;}");
    setWindowOpacity(1);
    resize(DEFAULT_WIDTH, height());

#ifndef CHECK_UPDATE
    connectionUser();
    qDebug() << "\nDEBUG VERSION\n";
#endif

    notifWindow = new QWidget();
    notifWindow->setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint);
    notifWindow->setFixedWidth(200);
    notifWindow->setFixedHeight(60);
    notifWindow->setContentsMargins(0,0,0,0);
    notifWindow->setStyleSheet("QTextEdit {"
                               "    background-color: #2F2F2F;"
                               "    color: #FFFFFF;"
                               "    padding-left:5px;"
                               "    padding-right:5px;"
                               "}"
                               "QWidget {"
                               "    border: 1px solid black;"
                               "}");

    QHBoxLayout *notifDialogLayout = new QHBoxLayout(notifWindow);
    notifDialogLayout->setSpacing(0);
    notifDialogLayout->setContentsMargins(0,0,0,0);

    notif = new QTextEdit(notifWindow);
    notif->setReadOnly(true);
    notif->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    notifDialogLayout->addWidget(notif);
    notifWindow->setLayout(notifDialogLayout);
    QRect scr = QApplication::desktop()->screenGeometry();
    notifWindow->move(scr.bottomRight() - QPoint(notifWindow->rect().width(), notifWindow->rect().height()));

    notifTimer = new QTimer();

    connect(notifTimer, SIGNAL(timeout()), notifWindow, SLOT(hide()));
}

void MainWindow::notified(int userId, QString msg)
{
    if (QGuiApplication::applicationState() < 4) {
        QString pseudo;
        QString tabName;
        QString message;
        pseudo = socket->getUsers().value(userId);
        if (userId == -1) {
            tabName = "Plouc's";
            QStringList list = msg.split(":");
            if (list.count() > 1) {
                pseudo = list.at(0) + ": ";
                message = list.at(1);
                for (int i = 2; i < list.count(); ++i)
                    message += ":" + list.at(i);
            } else {
                message = msg;
            }
        } else {
            tabName = pseudo;
            message = msg;
            pseudo = "";
        }

        QString notifText = "<b style='font-family: Roboto;font-size:15px;color: #82B1FF;'>"+ tabName +"</b>";
        notifText += "<br><b style='font-family: Roboto;font-size:12px;color:#82B1FF;'>"+ pseudo +"</b><span style='font-family: Roboto;font-size:12px;'>"+ message +"</span>";
        notif->setHtml(notifText);
        notifWindow->show();
        notifTimer->start(5000);
    }
}

void MainWindow::addTab(int id, QString pseudo)
{
    DisplayChat *dc = new DisplayChat(tabs);
    dc->setId(id);
    chats.insert(tabs->count(), dc);
    QString label = pseudo;;
    tabs->addTab(dc, label);
    connect(socket, SIGNAL(newMessage(QString,int)), dc, SLOT(newMessage(QString,int)));
    connect(dc, SIGNAL(newNotification(int,QString)), this, SLOT(notified(int,QString)));
    connect(dc, SIGNAL(playSound(SoundManager::Name)), sm, SLOT(play(SoundManager::Name)));
    resizeEvent(new QResizeEvent(size(), size()));
}

MainWindow::~MainWindow()
{

}

void MainWindow::resizeEvent(QResizeEvent *e)
{
    optionButton->move((TAB_WIDTH + TAB_PADDING * 2) * tabs->count() + 11, 11);
    windowButtons->move(width() - 40, 13);
}

void MainWindow::updateAvailable(bool newUpdate)
{
    if (newUpdate || !QFile(QCoreApplication::applicationDirPath() + "/resources/Roboto-Regular.ttf").exists()) {
        fd = new FileDownloader(QUrl("http://www.goutye.com/ploucs/PloucsMessenger.exe"), this);
        connect(fd, SIGNAL(downloaded()), this, SLOT(getFont()));
    } else {
        connectionUser();
    }
}

void MainWindow::getFont()
{
    disconnect(fd, SIGNAL(downloaded()), this, SLOT(updateSettingsApp()));
    QFile filePMupdated(QCoreApplication::applicationFilePath() + ".update");
    filePMupdated.open(QFile::WriteOnly);
    filePMupdated.write(fd->downloadedData());
    filePMupdated.close();
    if (!QFile(QCoreApplication::applicationDirPath() + "/resources/Roboto-Regular.ttf").exists()) {
        fd = new FileDownloader(QUrl("http://www.goutye.com/ploucs/Roboto-Regular.ttf"), this);
        connect(fd, SIGNAL(downloaded()), this, SLOT(updateSettingsApp()));
    } else {
        qDebug() << "Just basic update";
        updateSettingsApp();
    }
}

void MainWindow::updateSettingsApp()
{
    if (!QFile(QCoreApplication::applicationDirPath() + "/resources/Roboto-Regular.ttf").exists()) {
        disconnect(fd, SIGNAL(downloaded()), this, SLOT(updateSettingsApp()));
        QFile fileTTFupdated(QCoreApplication::applicationDirPath() + "/resources/Roboto-Regular.ttf");
        fileTTFupdated.open(QFile::WriteOnly);
        fileTTFupdated.write(fd->downloadedData());
        fileTTFupdated.close();
    }
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
        if (it.value() == pseudo)
            continue;
        a = new UserAction(it.value(), it.key());
        connect(a, SIGNAL(triggered(int,QString)), this, SLOT(userPM(int,QString)));
        optionButton->addUserToMenu(a);
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
    qDebug() << "Connection!";

    //search if one is same id
    if (!tabs->findAndSet(id, pseudo)) {
        addTab(id, pseudo);
        if (!socket->isOnline(id))
            tabs->findAndSetDisconnect(id, true);
    } else {
        tabs->findAndSetDisconnect(id, false);
    }
}

void MainWindow::disconnection(int id)
{
    tabs->findAndSetDisconnect(id, true);
}

void MainWindow::userPM(int id, QString pseudo)
{
    //If a tab with this user is already opened.
    if (tabs->findAndSet(id, pseudo))
        return;

    //Else create a tab with this user.
    addTab(id, pseudo);
    if (!socket->isOnline(id))
        tabs->findAndSetDisconnect(id, true);
}

void MainWindow::post()
{
    if (inputMsg->toPlainText().isEmpty())
        return;

    int idUser = tabs->currentIdDC();
    QString text = inputMsg->toPlainText();
    text.remove(text.count()-1, 1);
    text.replace('\n', "<br>");
    if (idUser == -1)
        emit post(text);
    else
        emit post(text, idUser);

    inputMsg->setText("");
    qDebug() << "Posted!";
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
        QTimer::singleShot(1000, this, SLOT(isDisconnected()));
    }
}
