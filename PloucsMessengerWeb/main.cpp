#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug() << QString("ok %1").arg(QFontDatabase::addApplicationFont(QCoreApplication::applicationDirPath() + "/resources/Roboto-Regular.ttf"));

    QFont f("Roboto");

    QApplication::setFont(f);
    QStringList st = QFontDatabase::applicationFontFamilies(0);
    for (int i = 0; i < st.count(); ++i)
        qDebug() << st.at(i);

    MainWindow w;
    w.show();


    return a.exec();
}
