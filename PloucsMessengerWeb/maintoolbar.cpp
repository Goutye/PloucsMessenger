#include "maintoolbar.h"

#include <QToolButton>
#include <QSize>
#include <QSizeGrip>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>

MainToolBar::MainToolBar(QWidget *parent) : QToolBar(parent)
{
    QToolButton *button = new QToolButton(this);
    menu = new QMenu(button);
    usersMenu = new QMenu("Users", menu);
    menu->addMenu(usersMenu);

    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QAction *actionStartUp = new QAction("Start-up", menu);
    actionStartUp->setCheckable(true);
    actionStartUp->setChecked(settings.contains("PloucsMessenger"));
    connect(actionStartUp, SIGNAL(triggered()), this, SLOT(setStartup()));
    menu->addAction(actionStartUp);

    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);
    this->setIconSize(QSize(5,10));
    this->addWidget(button);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->addWidget(spacer);

    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setFixedHeight(5);
    this->addWidget(sizeGrip);
}

MainToolBar::~MainToolBar()
{

}

void MainToolBar::error(QProcess::ProcessError error)
{

    qDebug() << "ERROR" << error;
}

void MainToolBar::addUserToMenu(QAction *a)
{
    a->setParent(usersMenu);
    usersMenu->addAction(a);
}

void MainToolBar::removeUserToMenu(QAction *a)
{
    usersMenu->removeAction(a);
}

void MainToolBar::setStartup()
{
    qDebug() << QCoreApplication::applicationDirPath() + "/settings.exe";
    QString program = QCoreApplication::applicationDirPath() + "/settings.exe";
#ifdef Q_OS_WIN
    int result = (int)::ShellExecuteA(0, "open", program.toUtf8().constData(), "startup", 0, SW_SHOWNORMAL);
    if (SE_ERR_ACCESSDENIED == result)
    {
        // Requesting elevation
        result = (int)::ShellExecuteA(0, "runas", program.toUtf8().constData(), "startup", 0, SW_SHOWNORMAL);
    }
    if (result <= 32)
    {
        // error handling
    }
#else
    if (!QProcess::startDetached(program, "startup"))
    {
        // error handling
    }
#endif
}
