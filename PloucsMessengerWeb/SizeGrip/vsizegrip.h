#ifndef VSIZEGRIP_H
#define VSIZEGRIP_H

#include <QWidget>
#include <QSizeGrip>

class VSizeGrip : public QSizeGrip
{
    Q_OBJECT
public:
    explicit VSizeGrip(QWidget *parent = 0);
    ~VSizeGrip();

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);
    void enterEvent(QEvent *ev);

private:
    int oldX = 0;
};

#endif // VSIZEGRIP_H
