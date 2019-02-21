#ifndef SERVICEFACETORY_H
#define SERVICEFACETORY_H

#include "servicei.h"
#include "restserviceconcureent.h"
#include "notifybymqtt.h"
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
    inline NotifyServiceI* makeNotifyServiceI(NotifyInterfaceType s){
        switch (s) {
        case Mqtt:
            return new NotifyByMqtt;
            break;
        case Kafka:

            break;
        case WebSocket:
            return new NotifyByWebSocket;
            break;
        default:
            break;
        }
        return nullptr;
    }
};
#endif // SERVICEFACETORY_H
