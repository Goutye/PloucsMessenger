#include <QToolButton>
#include "tabsarea.h"
#include <qDebug>
#include <QTabBar>

#include "tabbar.h"

TabsArea::TabsArea(QWidget *parent) : QTabWidget(parent)
{
    TabBar *t = new TabBar();
    setTabBar(t);
    //setMovable(true);
    connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(this, SIGNAL(currentChanged(int)), this, SLOT(currentChanged(int)));

    QString style = "QTabBar::tab {"
                        "background: #202020;"
                        "border-bottom: 2px solid #202020;"
                        "min-width: 20ex;"
                        "height: 8ex;"
                        "padding: 2px;"
                        "padding-left:10px;"
                        "padding-right:10px;"
                        "color: #888888;"
                        "font-size:16px;"
                        "font-family: Roboto-Light;"
                    "}"
                    "QTabBar::tab:selected {"
                        "color: #EEEEEE;"
                        "border-bottom-color: #4587F6;"
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
    QTabBar *tabBar = findChild<QTabBar *>();
    if (index == currentIndex())
        setNotify(index, false);
}

int TabsArea::currentIdDC()
{
    return ((DisplayChat *) currentWidget())->id();
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

bool TabsArea::findAndAdd(int id, QString pseudo)
{
    for (int i = 0; i < count(); ++i) {
        if (id == idDC(i)) {
            setTabText(i, tabText(i) + pseudo);
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

void TabsArea::setNotify(int i, bool b)
{
    notifyTabs.insert(i, b);
    if (b)
        tabBar()->update();
}

void TabsArea::addTab(QWidget *widget, const QString &s)
{
    QTabWidget::addTab(widget, s);
    if (count() == 1) {
        QTabBar *tabBar = findChild<QTabBar *>();
        tabBar->setTabButton(0, QTabBar::RightSide, 0);
        tabBar->setTabButton(0, QTabBar::LeftSide, 0);
    }
}
