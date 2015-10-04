#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ChatWidget(QWidget *parent = 0);
    ~ChatWidget();
    void paintEvent(QPaintEvent *);

signals:

public slots:
};

#endif // CHATWIDGET_H
