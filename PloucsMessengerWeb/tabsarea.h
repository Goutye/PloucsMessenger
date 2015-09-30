#ifndef TABSAREA_H
#define TABSAREA_H

#include <QWidget>
#include <QTabWidget>
#include <QMap>
#include "displaychat.h"

#define TAB_WIDTH 75
#define TAB_PADDING 10

class TabsArea : public QTabWidget
{
    Q_OBJECT
public:
    explicit TabsArea(QWidget *parent = 0);
    ~TabsArea();
    int currentIdDC();
    int idDC(int i);
    bool findAndSet(int id, QString pseudo);
    bool findAndAdd(int id, QString pseudo);
    void addTab(QWidget *widget, const QString &s);
    void setNotify(int i, bool b);
    bool notify(int i);

signals:

public slots:
    void currentChanged(int index);
    void prevChat();
    void nextChat();
    void removeTab(int i);

private slots:
    void refreshAlert();

private:
    void setIdDC(int i, int id);

    QMap<int, bool> notifyTabs;
};

#endif // TABSAREA_H
