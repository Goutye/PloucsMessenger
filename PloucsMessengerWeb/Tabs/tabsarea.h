#ifndef TABSAREA_H
#define TABSAREA_H

#include <QWidget>
#include <QButtonGroup>
#include <QStackedWidget>
#include <QHBoxLayout>
#include <QMap>
#include "displaychat.h"

#define TAB_WIDTH 75
#define TAB_PADDING 10

class TabsArea : public QWidget
{
    Q_OBJECT
public:
    explicit TabsArea(QWidget *parent = 0);
    ~TabsArea();
    int currentIdDC();
    int idDC(int i);
    void setCurrentIdDC(int id);
    bool findAndSet(int id, QString pseudo);
    bool findAndSetDisconnect(int id, bool b);
    void addTab(QWidget *widget, const QString &s);
    void setNotify(int i, bool b);
    bool notify(int i);
    bool isDisconnected(int i);
    int count();
    int currentIndex();

signals:

public slots:
    void currentChanged(int index);
    void prevChat();
    void nextChat();
    void removeTab(int i);

protected:
    void mousePressEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);

private slots:
    void refreshAlert();

private:
    void setIdDC(int i, int id);
    void changeProperty(QWidget *w, QString name, bool b);

    QMap<int, bool> isDisconnectUser;
    QMap<int, bool> notifyTabs;
    QButtonGroup *tabbar;
    QHBoxLayout *buttonsLayout;
    QStackedWidget *tabs;
    int _currentIndex = 0;
};

#endif // TABSAREA_H
