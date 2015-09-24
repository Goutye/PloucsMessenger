#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (QFontDatabase::addApplicationFont(QCoreApplication::applicationFilePath() + "/resources/Roboto-Light.ttf"))
        qDebug() << "ok";
    if (QFontDatabase::addApplicationFont(QCoreApplication::applicationFilePath() + "/resources/Roboto-Bold.ttf"))
        qDebug() << "ok";

    QFont f("Roboto-Light");
    QFont f2("Roboto-Bold");

    QApplication::setFont(f);

    MainWindow w;
    w.show();


    return a.exec();
}
