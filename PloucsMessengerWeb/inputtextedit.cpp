#include "inputtextedit.h"

InputTextEdit::InputTextEdit(QWidget *parent) : QTextEdit(parent)
{

}

InputTextEdit::~InputTextEdit()
{

}

void InputTextEdit::keyPressEvent(QKeyEvent *e)
{
    QTextEdit::keyPressEvent(e);

    switch (e->key()) {
    case Qt::Key_Shift:
        canSend = false;
        break;
    case Qt::Key_Return:
        if (canSend)
            emit returnPressed();
        break;
    default:
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
