#include "optionbutton.h"

#include <QSize>
#include <QSizeGrip>
#include <QDir>
#include <QSettings>
#include <QDebug>
#include <QTimer>
#include <QCoreApplication>
#include <QSettings>

OptionButton::OptionButton(QWidget *parent) : QToolButton(parent)
{
    menu = new QMenu(this);
    usersMenu = new QMenu("Users", menu);
    menu->addMenu(usersMenu);

    QSettings settingsUser(QCoreApplication::applicationDirPath() + "/resources/user_config.ini", QSettings::IniFormat);
    QAction *actionMute = new QAction("Mute", menu);
    actionMute->setCheckable(true);
    actionMute->setChecked(settingsUser.value("mute", false).toBool());
    connect(actionMute, SIGNAL(triggered(bool)), this, SLOT(emitMute(bool)));
    menu->addAction(actionMute);
    actionsINI.insert("mute", actionMute);

    QAction *actionBackgroundTask = new QAction("Minimize on startup", menu);
    actionBackgroundTask->setCheckable(true);
    actionBackgroundTask->setChecked(settingsUser.value("minimizeOnStartup", false).toBool());
    connect(actionBackgroundTask, SIGNAL(triggered()), this, SLOT(saveOptions()));
    menu->addAction(actionBackgroundTask);
    actionsINI.insert("minimizeOnStartup", actionBackgroundTask);

    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    QAction *actionStartUp = new QAction("Start-up", menu);
    actionStartUp->setCheckable(true);
    actionStartUp->setChecked(settings.contains("PloucsMessenger"));
    connect(actionStartUp, SIGNAL(triggered()), this, SLOT(setStartup()));
    menu->addAction(actionStartUp);

    setMenu(menu);
    setPopupMode(QToolButton::InstantPopup);
//    QSizeGrip *sizeGrip = new QSizeGrip(this);
//    sizeGrip->setFixedHeight(5);
//    this->addWidget(sizeGrip);
    setFixedSize(22, 17);
    setStyleSheet(  "QToolButton {"
                    "   border: 0px solid black;"
                    "   background: url(:/icons/setting_normal) center no-repeat;"
                    "}"
                    "QToolButton:menu-indicator {"
                    "   image: none;"
                    "}"
                    "QToolButton:pressed {"
                    "   background: url(:/icons/setting_pressed) center no-repeat;"
                    "}"
                    "QToolButton:hover {"
                    "   background: url(:/icons/setting_hover) center no-repeat;"
                    "}");
}

OptionButton::~OptionButton()
{

}

void OptionButton::initOptions()
{
    QSettings settingsUser(QCoreApplication::applicationDirPath() + "/resources/user_config.ini", QSettings::IniFormat);
    emit mute(settingsUser.value("mute", false).toBool());

    if (settingsUser.value("minimizeOnStartup").toBool())
        QTimer::singleShot(500, this, SLOT(emitMinimize()));
}

void OptionButton::emitMinimize()
{
    emit minimize();
}

void OptionButton::emitMute(bool b)
{
    emit mute(b);
    QSettings settings(QCoreApplication::applicationDirPath() + "/resources/user_config.ini", QSettings::IniFormat);
    settings.setValue("mute", b);
}

void OptionButton::saveOptions()
{
    QSettings settings(QCoreApplication::applicationDirPath() + "/resources/user_config.ini", QSettings::IniFormat);

    for (QMap<QString, QAction*>::iterator it = actionsINI.begin(); it != actionsINI.end(); ++it) {
        settings.setValue(it.key(), it.value()->isChecked());
    }
}

void OptionButton::error(QProcess::ProcessError error)
{

    qDebug() << "ERROR" << error;
}

void OptionButton::addUserToMenu(QAction *a)
{
    a->setParent(usersMenu);
    usersMenu->addAction(a);
}

void OptionButton::removeUserToMenu(QAction *a)
{
    usersMenu->removeAction(a);
}

void OptionButton::setStartup()
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
