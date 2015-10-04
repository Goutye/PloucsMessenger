#include <QPainter >
#include "chatwidget.h"

#include <QStyleOption>


ChatWidget::ChatWidget(QWidget *parent) : QWidget(parent)
{

}

ChatWidget::~ChatWidget()
{

}

void ChatWidget::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
