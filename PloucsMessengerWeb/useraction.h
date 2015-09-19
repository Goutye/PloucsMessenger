#ifndef USERACTION_H
#define USERACTION_H

#include <QObject>
#include <QAction>

class UserAction : public QAction
{
    Q_OBJECT
public:
    explicit UserAction(QString pseudo, int id);
    ~UserAction();

signals:
    void triggered(int id, QString pseudo);

public slots:

private slots:
    void isTriggered();

private:
    int id;
    QString pseudo;
};

#endif // USERACTION_H
