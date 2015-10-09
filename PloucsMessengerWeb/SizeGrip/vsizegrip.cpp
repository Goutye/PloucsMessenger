#include <QCursor>
#include <QApplication>
#include <QMouseEvent>
#include "vsizegrip.h"
#include <QDebug>

VSizeGrip::VSizeGrip(QWidget *parent) : QSizeGrip(parent)
{
}

VSizeGrip::~VSizeGrip()
{

}

void VSizeGrip::mousePressEvent(QMouseEvent *ev)
{
    oldX = ev->globalX();
    QSizeGrip::mousePressEvent(ev);
}

void VSizeGrip::mouseMoveEvent(QMouseEvent *ev)
{
    QMouseEvent *nEv = new QMouseEvent(ev->type(), QPoint(oldX, ev->globalY()), QPoint(oldX, ev->globalY()), ev->button(), ev->buttons(), ev->modifiers());
    QSizeGrip::mouseMoveEvent(nEv);
}

void VSizeGrip::enterEvent(QEvent *ev)
{
    QSizeGrip::enterEvent(ev);
    setCursor(QCursor(Qt::SizeVerCursor));
}
