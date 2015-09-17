#include "maintoolbar.h"

#include <QToolButton>
#include <QMenu>
#include <QSize>
#include <QSizeGrip>

MainToolBar::MainToolBar(QWidget *parent) : QToolBar(parent)
{
    QToolButton *button = new QToolButton(this);
    QMenu *menu = new QMenu(button);
    menu->addAction("Update");
    button->setMenu(menu);
    button->setPopupMode(QToolButton::InstantPopup);
    this->setIconSize(QSize(5,10));
    this->addWidget(button);

    QWidget* spacer = new QWidget();
    spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->addWidget(spacer);

    QSizeGrip *sizeGrip = new QSizeGrip(this);
    sizeGrip->setFixedHeight(5);
    this->addWidget(sizeGrip);
}

MainToolBar::~MainToolBar()
{

}

