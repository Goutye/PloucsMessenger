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

#define DEFAULT_WIDTH 400
#define LABEL_BACKGROUND_ACTIVE_COLOR "#BBBBBB"
#define LABEL_BACKGROUND_INACTIVE_COLOR "#888888"

#define CHECK_UPDATEw

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

    window = new ChatWidget(this);

    setWindowFlags(Qt::FramelessWindowHint);

    tb = new MainToolBar(this);
    addToolBar(Qt::RightToolBarArea, tb);

    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->setContentsMargins(0,0,0,0);

    tabs = new TabsArea(window);

    DisplayChat *dc = new DisplayChat(tabs);
    chats.insert(tabs->count(), dc);
    tabs->addTab(dc, "Plouc's");
    connect(socket, SIGNAL(newMessage(QString)), dc, SLOT(newMessage(QString)));

    vLayout->addWidget(tabs, 10);

    inputMsg = new QLineEdit();
    inputMsg->setFixedHeight(40);
    ChatWidget *inputChat = new ChatWidget(window);
    QVBoxLayout *inputChatLayout = new QVBoxLayout;
    inputChat->setLayout(inputChatLayout);
    inputChatLayout->addWidget(inputMsg, 1);
    vLayout->addWidget(inputChat, 1);

    window->setLayout(vLayout);
    vLayout->setSpacing(0);
    window->setContentsMargins(0,0,0,0);
    setContentsMargins(1,1,1,1);

    setCentralWidget(window);

    connect(inputMsg, SIGNAL(returnPressed()), this, SLOT(post()));

    QShortcut *changeWRight = new QShortcut(QKeySequence(Qt::Key_Tab), tabs, SLOT(nextChat()));
    QShortcut *changeWLeft = new QShortcut(QKeySequence(Qt::SHIFT + Qt::Key_Tab), tabs, SLOT(prevChat()));
    QShortcut *close = new QShortcut(QKeySequence(Qt::Key_Escape), this, SLOT(close()));

    window->setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("QMainWindow { background-color: #202020; "
                                "border: 5px solid black;}"
                  "QMainWindow > QWidget { background-color: #202020; }"
                  "QMainWindow > QWidget > QWidget { background-color: #414141; }"
                  "QMainWindow QLineEdit { border-width: 2px;"
                                          "border-style:solid;"
                                          "border-color: #525252;"
                                          "background-color: #414141;"
                                          "color: #F4F4F4;}"
                  "QTextEdit { border: 0px solid black; "
                              "border-bottom: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                  "stop: 0 #2E2F2F, stop: 0.5 #272827, stop: 1.0 #222222);"
                              "background-color: #2F2F2F;"
                              "color: #FFFFFF;}"
                  "QToolBar { border-color: #666666; }");
    setWindowOpacity(1);
    resize(DEFAULT_WIDTH, height());

#ifndef CHECK_UPDATE
    connectionUser();
    qDebug() << "\nDEBUG VERSION\n";
#endif
}

void MainWindow::addTab(int id, QString pseudo)
{
    DisplayChat *dc = new DisplayChat(tabs);
    dc->setId(id);
    chats.insert(tabs->count(), dc);
    QString label = pseudo;;
    tabs->addTab(dc, label);
    connect(socket, SIGNAL(newMessage(QString,int)), dc, SLOT(newMessage(QString,int)));
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
    qDebug() << "Connection!";

    //search if one is same id
    if (!tabs->findAndSet(id, pseudo))
        addTab(id, pseudo);
}

void MainWindow::disconnection(int id)
{
    !tabs->findAndAdd(id, " (Offline)");
}

void MainWindow::userPM(int id, QString pseudo)
{
    //If a tab with this user is already opened.
    if (tabs->findAndAdd(id, ""))
        return;

    //Else create a tab with this user.
    addTab(id, pseudo);
}

void MainWindow::post()
{
    if (inputMsg->text().isEmpty())
        return;

    int idUser = tabs->currentIdDC();

    if (idUser == -1)
        emit post(inputMsg->text());
    else
        emit post(inputMsg->text(), idUser);

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
        QTimer::singleShot(3000, this, SLOT(isDisconnected()));
    }
}
