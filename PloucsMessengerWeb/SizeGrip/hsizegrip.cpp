#include <QMouseEvent>
#include "hsizegrip.h"
#include <QDebug>

HSizeGrip::HSizeGrip(QWidget *parent) : QSizeGrip(parent)
{
    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

HSizeGrip::~HSizeGrip()
{

}

void HSizeGrip::mousePressEvent(QMouseEvent *ev)
{
    oldY = ev->globalY();
    QSizeGrip::mousePressEvent(ev);
}

void HSizeGrip::mouseMoveEvent(QMouseEvent *ev)
{
    QMouseEvent *nEv = new QMouseEvent(ev->type(), QPoint(ev->globalX(), oldY), QPoint(ev->globalX(), oldY), ev->button(), ev->buttons(), ev->modifiers());
    QSizeGrip::mouseMoveEvent(nEv);
}

void HSizeGrip::enterEvent(QEvent *ev)
{
    QSizeGrip::enterEvent(ev);
    setCursor(QCursor(Qt::SizeHorCursor));
}
