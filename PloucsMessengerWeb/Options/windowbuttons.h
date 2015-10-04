#ifndef WINDOWBUTTONS_H
#define WINDOWBUTTONS_H

#include <QObject>
#include <QWidget>
#include <QPushButton>

class WindowButtons : public QWidget
{
    Q_OBJECT
public:
    explicit WindowButtons(QWidget *parent = 0);
    ~WindowButtons();

public slots:
    void emitClose();
    void emitMinimize();

signals:
    void close();
    void minimize();

public slots:
};

#endif // WINDOWBUTTONS_H
