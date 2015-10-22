#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H

#include <QWidget>
#include <QPushButton>

class PushButton : public QPushButton
{
    Q_OBJECT
public:
    explicit PushButton(QWidget *parent = 0);
    PushButton(QString s, QWidget *parent = 0);
    ~PushButton();

signals:

public slots:

protected:
    void mouseMoveEvent(QMouseEvent *e);
};

#endif // PUSHBUTTON_H
