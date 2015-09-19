#include "useraction.h"

UserAction::UserAction(QString pseudo, int id) : QAction(pseudo, 0)
{
    this->pseudo = pseudo;
    this->id = id;
    connect(this, SIGNAL(triggered()), this, SLOT(isTriggered()));
}

UserAction::~UserAction()
{

}

void UserAction::isTriggered()
{
    emit triggered(id, pseudo);
}
