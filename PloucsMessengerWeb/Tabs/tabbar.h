#ifndef TABBAR_H
#define TABBAR_H

#include <QWidget>
#include <QTabBar>
#include <QPaintEvent>
#include <QStyleOptionTabBarBaseV2>

class TabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit TabBar(QWidget *parent = 0);
    ~TabBar();

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *ev);

signals:

public slots:


private:
    void initStyleBaseTB(QStyleOptionTabBarBaseV2 *optTabBase, QSize size);
};

#endif // TABBAR_H
