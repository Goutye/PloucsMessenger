#ifndef INPUTTEXTEDIT_H
#define INPUTTEXTEDIT_H

#include <QObject>
#include <QTextEdit>
#include <QKeyEvent>

class InputTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit InputTextEdit(QWidget *parent = 0);
    ~InputTextEdit();

protected:
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

signals:
    void returnPressed();

public slots:

private:
    bool canSend = true;
};

#endif // INPUTTEXTEDIT_H
