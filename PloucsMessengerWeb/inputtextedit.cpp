#include "inputtextedit.h"
#include "Tabs/tabsarea.h"

InputTextEdit::InputTextEdit(QWidget *tabs, QWidget *parent) : QTextEdit(parent), tabs(tabs)
{

}

InputTextEdit::~InputTextEdit()
{

}

void InputTextEdit::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_Shift:
        QTextEdit::keyPressEvent(e);
        canSend = false;
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (canSend) {
            emit returnPressed();
        } else {
            QTextEdit::keyPressEvent(e);
        }
        break;
    default:
        QTextEdit::keyPressEvent(e);
        break;
    }
}

void InputTextEdit::keyReleaseEvent(QKeyEvent *e)
{
    QTextEdit::keyReleaseEvent(e);

    switch (e->key()) {
    case Qt::Key_Shift:
        canSend = true;
        break;
    default:
        break;
    }
}

void InputTextEdit::mousePressEvent(QMouseEvent *e)
{
    QTextEdit::mousePressEvent(e);

    TabsArea *ta = ((TabsArea*) tabs);
    ta->setNotify(ta->currentIndex(), false);
}
