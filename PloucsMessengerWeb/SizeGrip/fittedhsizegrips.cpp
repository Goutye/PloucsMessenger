#include <QVBoxLayout>
#include "fittedhsizegrips.h"

FittedHSizeGrips::FittedHSizeGrips(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    HSizeGrip *hsg = new HSizeGrip(this);
    hsg->setFixedWidth(4);
    hsg->setFixedHeight(36);
    hsg->setStyleSheet("background-color: #202020;");
    vLayout->addWidget(hsg);
    hsg = new HSizeGrip(this);
    hsg->setFixedWidth(4);
    hsg->setStyleSheet("background-color: #2F2F2F;"
                       "border-top: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                       "stop: 0 #1B1C1B, stop: 1.0 #2E2F2F);"
                       "border-bottom: 5px solid qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
                       "stop: 0 #2E2F2F, stop: 0.5 #272827, stop: 1.0 #222222);");
    vLayout->addWidget(hsg);
    hsg = new HSizeGrip(this);
    hsg->setFixedWidth(4);
    hsg->setFixedHeight(76);
    hsg->setStyleSheet("background-color: #414141;");

    vLayout->addWidget(hsg);
    vLayout->setContentsMargins(0,0,0,0);
    vLayout->setSpacing(0);
    setContentsMargins(0,0,0,0);
    setLayout(vLayout);
    setStyleSheet("background-color:#414141;");
}

FittedHSizeGrips::~FittedHSizeGrips()
{

}

