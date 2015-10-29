#include "windowbuttons.h"
#include <QHBoxLayout>

WindowButtons::WindowButtons(QWidget *parent) : QWidget(parent)
{
    QPushButton *close = new QPushButton(this);
    close->setFixedSize(12, 12);
    close->setStyleSheet(   "QPushButton {"
                            "   border: 0px solid black;"
                            "   border-radius:6px;"
                            "   background-color: #424242;"
                            "}"
                            "QPushButton:hover {"
                            "   background-color: #594242;"
                            "}"
                            "QPushButton:pressed {"
                            "   background-color: #704242;"
                            "}");
    connect(close, SIGNAL(clicked()), this, SLOT(emitClose()));
    QPushButton *minimize = new QPushButton(this);
    minimize->setFixedSize(12, 12);
    minimize->setStyleSheet("QPushButton {"
                            "   border: 0px solid black;"
                            "   border-radius:6px;"
                            "   background-color: #424242;"
                            "}"
                            "QPushButton:hover {"
                            "   background-color: #425942;"
                            "}"
                            "QPushButton:pressed {"
                            "   background-color: #427042;"
                            "}");
    connect(minimize, SIGNAL(clicked()), this, SLOT(emitMinimize()));

    QHBoxLayout *layout = new QHBoxLayout();

    layout->addWidget(minimize);
    layout->addWidget(close);
    layout->addStretch();
    setLayout(layout);
    layout->setSpacing(6);
    layout->setContentsMargins(0,0,0,0);
}

WindowButtons::~WindowButtons()
{

}

void WindowButtons::emitClose() {
    emit close();
}

void WindowButtons::emitMinimize() {
    emit minimize();
}
