#ifndef SERVICEFACETORY_H
#define SERVICEFACETORY_H

#include "servicei.h"
#include "restserviceconcureent.h"
#include "notifypersonbymqtt.h"
#include "notifyeventbywebsocket.h"
#include "notifybyeventperson.h"
class ServiceFactory : public ServiceFactoryI
{
public:
    ServiceFactory() = default;
    inline NotifyServiceI* makeNotifyServiceI(NotifyInterfaceType s){
        switch (s) {
        case WebsocketEvent_MqttPerson:
            return new NotifyByEventPerson(makeNotifyEventServiceI(WebSocket),makeNotifyPersonServiceI(Mqtt));
            break;
        default:
            break;
        }
        return nullptr;
    }
    inline RestServiceI* makeRestServiceI(RestInterfaceType s){
        switch (s) {
        case HTTPREST:
            return new RestConcurrent;
            break;
        case THRIFTREST:

            break;
        default:
            break;
        }
        return nullptr;
    }
    inline NotifyPersonI* makeNotifyPersonServiceI(NotifyInterfaceType s){
        switch (s) {
        case Mqtt:
            return new NotifyPersonByMqtt;
            break;
        case Kafka:

            break;
        case WebSocket:

            break;
        default:
            break;
        }
        return nullptr;
    }
    inline NotifyEventI* makeNotifyEventServiceI(NotifyInterfaceType s){
        switch (s) {
        case Mqtt:

            break;
        case Kafka:

            break;
        case WebSocket:
            return new NotifyEventByWebSocket;
            break;
        default:
            break;
        }
        return nullptr;
    }
};
#endif // SERVICEFACETORY_H
