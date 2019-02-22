#ifndef SERVICEFACETORY_H
#define SERVICEFACETORY_H

#include "servicei.h"
#include "restserviceconcureent.h"
#include "notifypersonbymqtt.h"
#include "notifybywebsocket.h"
class ServiceFactory : public ServiceFactoryI
{
public:
    ServiceFactory() = default;
    inline RestServiceI* makeRestServiceI(RestInterfaceType s = HTTPREST){
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
