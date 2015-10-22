#include <QVBoxLayout>
#include "mainwindow.h"
#include <QTimer>
#include <QToolButton>
#include "tabsarea.h"
#include <qDebug>
#include <QTabBar>
#include <QApplication>
#include <QStyle>
#include "pushbutton.h"

TabsArea::TabsArea(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    setLayout(vLayout);
    QWidget *tabbarWidget =  new QWidget(this);
    tabbarWidget->setFixedHeight(41);
    tabbarWidget->setProperty("tabbar", true);
    tabbarWidget->setStyleSheet("QWidget[tabbar=true]{"
                                "padding-bottom: 5px;"
                                "border: none;"
                                "border-bottom: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                                                                       "stop: 0 #1B1C1B, stop: 1.0 #2E2F2F);"
                                "}"
                                "PushButton {"
                                    "background: #202020;"
                                    "border: none;"
                                    "height:23px;"
                                    "border-bottom: 2px solid #202020;"
                                    "width: "+ QString("%1").arg(TAB_WIDTH) +"px;"
                                    "padding: 0px;"
                                    "padding-left: "+ QString("%1").arg(TAB_PADDING) +"px;"
                                    "padding-right: "+ QString("%1").arg(TAB_PADDING) +"px;"
                                    "padding-bottom: 6px;"
                                    "color: #888888;"
                                    "font-size:16px;"
                                    "font-family: Roboto;"
                                "}"
                                "PushButton:selected {"
                                    "color: #EEEEEE;"
                                    "border-bottom-color: #82B1FF;"
                                "}"
                                "PushButton:selected, PushButton:hover {"
                                    "color: #EEEEEE;"
                                "}"
                                "PushButton[selected=true] {"
                                    "color: #EEEEEE;"
                                    "border-bottom-color: #82B1FF;"
                                "}"
                                "PushButton[notify=true] {"
                                    "border-bottom-color: #3E4C64;"
                                "}"
                                "PushButton[disconnect=true] {"
                                    "color:#333333;"
                                "}"
                                "PushButton[disconnect=true]:hover {"
                                    "color:#EEEEEE;"
                                "}");

    vLayout->addWidget(tabbarWidget);
    tabbar = new QButtonGroup;
    buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch();
    tabbarWidget->setLayout(buttonsLayout);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    tabs = new QStackedWidget(this);
    vLayout->addWidget(tabs);
    setContentsMargins(0,0,0,0);
    buttonsLayout->setContentsMargins(0,0,0,0);
    buttonsLayout->setSpacing(0);

    //connect(this, SIGNAL(tabCloseRequested(int)), this, SLOT(removeTab(int)));
    connect(tabbar, SIGNAL(buttonClicked(int)), this, SLOT(currentChanged(int)));
}

TabsArea::~TabsArea()
{

}

void TabsArea::changeProperty(QWidget *w, QString name, bool b)
{
    w->setProperty(name.toStdString().c_str(), b);
    w->style()->unpolish(w);
    w->style()->polish(w);
    w->update();
}

void TabsArea::currentChanged(int index)
{
    if (index < -1)
        index = index * -1 - 2;

    if (index != _currentIndex) {
        changeProperty(tabbar->button((_currentIndex+2)*-1), "selected", false);
        changeProperty(tabbar->button((index+2)*-1), "selected", true);
        _currentIndex = index;
        tabs->setCurrentIndex(index);

    }
    setNotify(index, false);
}

int TabsArea::currentIdDC()
{
    return ((DisplayChat *) tabs->currentWidget())->id();
}

void TabsArea::setCurrentIdDC(int id)
{
    for (int i = 0; i < tabs->count(); ++i) {
        if (id == idDC(i)) {
            currentChanged(i);
            return;
        }
    }
}

int TabsArea::idDC(int i)
{
    return((DisplayChat *) tabs->widget(i))->id();
}

void TabsArea::setIdDC(int i, int id)
{
    ((DisplayChat *) tabs->widget(i))->setId(id);
}

bool TabsArea::findAndSet(int id, QString pseudo)
{
    for (int i = 0; i < count(); ++i) {
        if (id == idDC(i)) {
            tabbar->button((i + 2) * -1)->setText(pseudo);
            return true;
        }
    }

    return false;
}

bool TabsArea::findAndSetDisconnect(int id, bool b)
{
    for (int i = 0; i < tabs->count(); ++i) {
        if (id == idDC(i)) {
            isDisconnectUser.insert(i, b);
            changeProperty(tabbar->button((i+2)*-1), "disconnect", b);
            return true;
        }
    }

    return false;
}

void TabsArea::prevChat()
{
    currentChanged(((_currentIndex - 1) + count()) % count());
}

void TabsArea::nextChat()
{
    currentChanged((_currentIndex + 1) % count());
}

void TabsArea::removeTab(int i)
{
    if (idDC(i) != -1) {
        buttonsLayout->removeWidget(tabbar->button((i + 2) * -1));
        tabbar->removeButton(tabbar->button((i + 2) * -1));
        tabs->removeWidget(tabs->widget(i));
    }
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
    changeProperty(tabbar->button((i + 2) * -1), "notify", b);
    if (b) {
        QApplication::alert(this, 1000);
        QTimer::singleShot(1000, this, SLOT(refreshAlert()));
    }
}

void TabsArea::addTab(QWidget *widget, const QString &s)
{
    tabs->addWidget(widget);
    PushButton *b = new PushButton(s);
    buttonsLayout->insertWidget(tabbar->buttons().count(), b);
    tabbar->addButton(b);
    if (tabs->count() == 1) {
        changeProperty(b, "selected", true);
    }
    isDisconnectUser.insert(count() - 1, false);
}

void TabsArea::mousePressEvent(QMouseEvent *e)
{
    ((MainWindow*) this->parent()->parent())->mousePressEvent(e);
}

int TabsArea::count()
{
    return tabs->count();
}

int TabsArea::currentIndex()
{
    return _currentIndex;
}
