#ifndef NOTIFYBYEVENTPERSON_H
#define NOTIFYBYEVENTPERSON_H

#include "servicei.h"
QT_FORWARD_DECLARE_CLASS(NotifyPersonI)
QT_FORWARD_DECLARE_CLASS(NotifyEventI)
class NotifyByEventPerson : public NotifyServiceI
{
    Q_OBJECT
public:
    NotifyByEventPerson(NotifyEventI *notifyEvI,NotifyPersonI *notifyPerI,QObject *parent = nullptr);
    ~NotifyByEventPerson();
    void start() override;
    void stop() override;

private:
    NotifyEventI *notifyEventI_{nullptr};
    NotifyPersonI *notifyPersonI_{nullptr};
};

#endif // NOTIFYBYEVENTPERSON_H
