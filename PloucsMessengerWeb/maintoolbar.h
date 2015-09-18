#ifndef MAINTOOLBAR_H
#define MAINTOOLBAR_H

#include <QObject>
#include <QToolBar>
#include <QProcess>

class MainToolBar : public QToolBar
{
    Q_OBJECT
public:
    explicit MainToolBar(QWidget *parent = 0);
    ~MainToolBar();

signals:

public slots:

private slots:
    void setStartup();
    void error(QProcess::ProcessError error);
};

#endif // MAINTOOLBAR_H
