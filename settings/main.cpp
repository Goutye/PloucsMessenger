#include <QCoreApplication>
#include <QSettings>
#include <QDebug>
#include <QFile>
#include <QTimer>

#ifdef Q_OS_WIN
    #include <windows.h>
    #include <shellapi.h>
#endif

int nbTry = 0;

void updatePM() {
    QString fileName = QCoreApplication::applicationDirPath() + "/PloucsMessenger.exe";
    qDebug() << fileName;
    if (QFile(fileName).exists() && !QFile::remove(fileName) && nbTry < 20) {
        QTimer::singleShot(500, updatePM);
        nbTry++;
        qDebug() << QString("Echec n°%1").arg(nbTry);
    } else {
        if (nbTry >= 20)
            exit(0);
        else {
            QFile(QCoreApplication::applicationDirPath() + "/PloucsMessenger.exe.update").rename(QCoreApplication::applicationDirPath() + "/PloucsMessenger.exe");
            QString program = QCoreApplication::applicationDirPath() + "/PloucsMessenger.exe";
        #ifdef Q_OS_WIN
            int result = (int)::ShellExecuteA(0, "open", program.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
            if (SE_ERR_ACCESSDENIED == result)
            {
                // Requesting elevation
                result = (int)::ShellExecuteA(0, "runas", program.toUtf8().constData(), 0, 0, SW_SHOWNORMAL);
            }
            if (result <= 32)
            {
                // error handling
            }
        #else
            if (!QProcess::startDetached(program))
            {
                // error handling
            } else {
                QTimer::singleShot(0, this, SLOT(close()));
            }
        #endif
            exit(0);
        }
    }
}

void set_on_startup() {
    QSettings settings("HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);

    if (!settings.contains("PloucsMessenger")) {
        QString value = QCoreApplication::applicationFilePath();
        QString apostroph = "\"";

        value.replace("/","\\");
        value.replace("settings", "PloucsMessenger");
        value = apostroph + value + apostroph + " --argument";
        qDebug() << value;

        settings.setValue("PloucsMessenger", value);
    }
    else {
        settings.remove("PloucsMessenger");
    }

    exit(0);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2)
        return 0;
    if (QString(argv[1]).compare("startup") == 0)
        set_on_startup();
    else if (QString(argv[1]).compare("update") == 0)
        updatePM();

    return a.exec();
}
