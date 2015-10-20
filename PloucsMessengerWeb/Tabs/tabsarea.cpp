#include "mainwindow.h"
#include <QTimer>
#include <QToolButton>
#include "tabsarea.h"
#include <qDebug>
#include <QTabBar>
#include <QApplication>

#include "tabbar.h"

TabsArea::TabsArea(QWidget *parent) : QTabWidget(parent)
{
    TabBar *t = new TabBar();
    setTabBar(t);
    //setMovable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));
    QString style = QString("QTabBar::tab {"
                        "") +
                        "background: #202020;"
                        "border-bottom: 2px solid #202020;"
                        "width: "+ QString("%1").arg(TAB_WIDTH) +"px;"
                        "height: 28px;"
                        "padding: 2px;"
                        "padding-left: "+ QString("%1").arg(TAB_PADDING) +"px;"
                        "padding-right: "+ QString("%1").arg(TAB_PADDING) +"px;"
                        "padding-bottom: 4px;"
                        "color: #888888;"
                        "font-size:16px;"
                        "font-family: Roboto;"
                    "}"
                    "QTabBar::tab:selected {"
                        "color: #EEEEEE;"
                        "border-bottom-color: #82B1FF;"
                    "}"
                    "QTabWidget::pane {"
                        "border-top: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                           "stop: 0 #1B1C1B, stop: 1.0 #2E2F2F);"
                    "}"
                    "QTabBar::pane {"
                    "   background-color: #FFFFFF;"
                    "}"
                    "QTabBar::tab:selected, QTabBar::tab:hover {"
                        "color: #EEEEEE;"
                    "}";
    setStyleSheet(style);
}

TabsArea::~TabsArea()
{

}

void TabsArea::currentChanged(int index)
{
    if (index == currentIndex())
        setNotify(index, false);
}

int TabsArea::currentIdDC()
{
    return ((DisplayChat *) currentWidget())->id();
}

void TabsArea::setCurrentIdDC(int id)
{
    for (int i = 0; i < count(); ++i) {
        if (id == idDC(i)) {
            setCurrentIndex(i);
            return;
        }
    }
}

int TabsArea::idDC(int i)
{
    return((DisplayChat *) widget(i))->id();
}

void TabsArea::setIdDC(int i, int id)
{
    ((DisplayChat *) widget(i))->setId(id);
}

bool TabsArea::findAndSet(int id, QString pseudo)
{
    for (int i = 0; i < count(); ++i) {
        if (id == idDC(i)) {
            setTabText(i, pseudo);
            return true;
        }
    }

    return false;
}

bool TabsArea::findAndSetDisconnect(int id, bool b)
{
    for (int i = 0; i < count(); ++i) {
        if (id == idDC(i)) {
            isDisconnectUser.insert(i, b);
            tabBar()->repaint();
            return true;
        }
    }

    return false;
}

void TabsArea::prevChat()
{
    setCurrentIndex(((currentIndex() - 1) + count()) % count());
}

void TabsArea::nextChat()
{
    setCurrentIndex((currentIndex() + 1) % count());
}

void TabsArea::removeTab(int i)
{
    if (idDC(i) != -1)
        QTabWidget::removeTab(i);
}

bool TabsArea::notify(int i)
{
    return notifyTabs.contains(i) && notifyTabs.value(i);
}

bool TabsArea::isDisconnected(int i)
{
    return isDisconnectUser.value(i);
}

void TabsArea::refreshAlert()
{
    for (int i = 0; i < count(); ++i) {
        if (notify(i)) {
            QApplication::alert(this, 1000);
            QTimer::singleShot(1000, this, SLOT(refreshAlert()));
            return;
        }
    }
    return;
}

void TabsArea::setNotify(int i, bool b)
{
    notifyTabs.insert(i, b);
    if (b) {
        tabBar()->update();
        QApplication::alert(this, 1000);
        QTimer::singleShot(1000, this, SLOT(refreshAlert()));
    }
}

void TabsArea::addTab(QWidget *widget, const QString &s)
{
    QTabWidget::addTab(widget, s);
    if (count() == 1) {
        QTabBar *tabBar = findChild<QTabBar *>();
        tabBar->setTabButton(0, QTabBar::RightSide, 0);
        tabBar->setTabButton(0, QTabBar::LeftSide, 0);
    }
    isDisconnectUser.insert(count() - 1, false);
}

void TabsArea::mousePressEvent(QMouseEvent *e)
{
    ((MainWindow*) this->parent()->parent())->mousePressEvent(e);
}
