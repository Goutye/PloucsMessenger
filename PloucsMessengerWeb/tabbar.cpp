#include "tabbar.h"
#include <QTabWidget>
#include <QStyle>
#include <qDebug>
#include <QToolButton>
#include <QStylePainter>
#include <QSize>
#include <QStyleOptionTab>
#include <QRect>
#include "tabsarea.h"

TabBar::TabBar(QWidget *parent) : QTabBar(parent)
{

}

TabBar::~TabBar()
{

}

void TabBar::initStyleBaseTB(QStyleOptionTabBarBaseV2 *optTabBase, QSize size)
{
    QStyleOptionTab tabOverlap;
    tabOverlap.shape = shape();
    int overlap = style()->pixelMetric(QStyle::PM_TabBarBaseOverlap, &tabOverlap, this);
    QWidget *theParent = parentWidget();
    optTabBase->init(this);
    optTabBase->shape = shape();
    optTabBase->documentMode = documentMode();

    if (theParent && overlap > 0) {
        QRect rect;
        switch (tabOverlap.shape) {
        case QTabBar::RoundedNorth:
        case QTabBar::TriangularNorth:
            rect.setRect(0, size.height()-overlap, size.width(), overlap);
            break;
        case QTabBar::RoundedSouth:
        case QTabBar::TriangularSouth:
            rect.setRect(0, 0, size.width(), overlap);
            break;
        case QTabBar::RoundedEast:
        case QTabBar::TriangularEast:
            rect.setRect(0, 0, overlap, size.height());
            break;
        case QTabBar::RoundedWest:
        case QTabBar::TriangularWest:
            rect.setRect(size.width() - overlap, 0, overlap, size.height());
            break;
        }
        optTabBase->rect = rect;
    }
}

void TabBar::paintEvent(QPaintEvent *event)
{
    QTabBar::paintEvent(event);
    QStylePainter p(this);
    QStyleOptionTab cutTab;
    QStyleOptionTabBarBaseV2 optTabBar;
    initStyleBaseTB(&optTabBar, size());

    int selected = -1;
    int cut = -1;
    bool rtl = optTabBar.direction == Qt::RightToLeft;
    bool vertical = false;//verticalTabs(shape());
    selected = currentIndex();

    for (int i = 0; i < count(); ++i)
        optTabBar.tabBarRect |= tabRect(i);

    optTabBar.selectedTabRect = tabRect(selected);
    //p.drawPrimitive(QStyle::PE_FrameTabBarBase, optTabBar);

    for (int i = 0; i < count(); ++i) {
        QStyleOptionTabV3 tab;
        initStyleOption(&tab, i);

        if (!(tab.state & QStyle::State_Enabled)) {
            tab.palette.setCurrentColorGroup(QPalette::Disabled);
        }
        // If this tab is partially obscured, make a note of it so that we can pass the information
        // along when we draw the tear.
        if (((!vertical && (!rtl && tab.rect.left() < 0)) || (rtl && tab.rect.right() > width()))
            || (vertical && tab.rect.top() < 0)) {
            cut = i;
            cutTab = tab;
        }
        // Don't bother drawing a tab if the entire tab is outside of the visible tab bar.
        if ((!vertical && (tab.rect.right() < 0 || tab.rect.left() > width()))
            || (vertical && (tab.rect.bottom() < 0 || tab.rect.top() > height())))
            continue;
        optTabBar.tabBarRect |= tab.rect;
        if (i == selected)
            continue;
        if (((TabsArea *) parent())->notify(i)) {
            setStyleSheet("QTabBar::tab{"
                          "border-bottom-color: #CC3300;"
                      "}");
            p.drawControl(QStyle::CE_TabBarTab, tab);
            setStyleSheet("");
            qDebug() << "notify";
        } else {
            //p.drawControl(QStyle::CE_TabBarTab, tab);
        }

        if (selected >= 0) {
            QStyleOptionTabV3 tab;
            initStyleOption(&tab, selected);
//            if (d->paintWithOffsets && d->tabList[selected].dragOffset != 0) {
//                if (vertical)
//                    tab.rect.moveTop(tab.rect.y() + d->tabList[selected].dragOffset);
//                else
//                    tab.rect.moveLeft(tab.rect.x() + d->tabList[selected].dragOffset);
//            }
            //if (true) //!d->dragInProgress)
                //p.drawControl(QStyle::CE_TabBarTab, tab);
//            else {
//                int taboverlap = style()->pixelMetric(QStyle::PM_TabBarTabOverlap, 0, this);
//                d->movingTab->setGeometry(tab.rect.adjusted(-taboverlap, 0, taboverlap, 0));
//            }
        }


        // Only draw the tear indicator if necessary. Most of the time we don't need too.
//        if (d->leftB->isVisible() && cut >= 0) {
//            cutTab.rect = rect();
//            cutTab.rect = style()->subElementRect(QStyle::SE_TabBarTearIndicator, &cutTab, this);
//            p.drawPrimitive(QStyle::PE_IndicatorTabTear, cutTab);
//        }
    }
}
