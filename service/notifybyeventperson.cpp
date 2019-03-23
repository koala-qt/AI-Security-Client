#include "notifybyeventperson.h"
#include "notifyeventbywebsocket.h"
#include "notifypersonbymqtt.h"

NotifyByEventPerson::NotifyByEventPerson(NotifyEventI *notifyEvI, NotifyPersonI *notifyPerI, QObject *parent):
    notifyEventI_(notifyEvI),
    notifyPersonI_(notifyPerI),
    NotifyServiceI(parent)
{
    connect(notifyEventI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)));
    connect(notifyEventI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)));
    connect(notifyEventI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)));
    connect(notifyEventI_,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)),this,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)));
    connect(notifyEventI_,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)),this,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)));
    connect(notifyPersonI_,SIGNAL(sigFaceLinkDataFinished(QString)),this,SIGNAL(sigFaceLinkDataFinished(QString)));
    connect(notifyPersonI_,SIGNAL(sigFaceSnap(NotifyPersonI::FaceSnapEventData)),this,SIGNAL(sigFaceSnap(NotifyPersonI::FaceSnapEventData)));
    connect(notifyPersonI_,SIGNAL(sigNetWorkError(QString)),this,SIGNAL(sigNetWorkError(QString)));
    connect(notifyPersonI_,SIGNAL(sigVideoFacePicture(QString,QImage)),this,SIGNAL(sigVideoFacePicture(QString,QImage)));
}

NotifyByEventPerson::~NotifyByEventPerson()
{
    delete notifyEventI_;
    delete notifyPersonI_;
}

void NotifyByEventPerson::start()
{
    notifyEventI_->start();
    notifyPersonI_->start();
}

void NotifyByEventPerson::stop()
{
    notifyEventI_->requestInterruption();
    notifyEventI_->quit();
    notifyEventI_->wait();
    notifyPersonI_->requestInterruption();
    notifyPersonI_->quit();
    notifyPersonI_->wait();
}
