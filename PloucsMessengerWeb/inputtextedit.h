#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

class InputTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputTextEdit(QWidget *tabs, QWidget *parent = 0);
    ~InputTextEdit();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *e);

signals:
    void returnPressed();

public slots:

private:
    bool canSend = true;
    QWidget *tabs;
};

#endif // INPUTTEXTEDIT_H
