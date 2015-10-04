#include "mainwindow.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont f("Roboto");

    QApplication::setFont(f);

    MainWindow w;
    w.show();


    return a.exec();
}
