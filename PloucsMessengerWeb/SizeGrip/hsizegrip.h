#ifndef HSIZEGRIP_H
#define HSIZEGRIP_H

#include <QWidget>
#include <QSizeGrip>

class HSizeGrip : public QSizeGrip
{
    Q_OBJECT
public:
    explicit HSizeGrip(QWidget *parent = 0);
    ~HSizeGrip();

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void enterEvent(QEvent *ev);

private:
    int oldY = 0;
};

#endif // HSIZEGRIP_H
