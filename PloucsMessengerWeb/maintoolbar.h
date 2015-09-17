#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QObject>
#include <QToolBar>

class MainToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MainToolBar(QWidget *parent = 0);
    ~MainToolBar();

signals:

public slots:
};

#endif // MAINTOOLBAR_H
