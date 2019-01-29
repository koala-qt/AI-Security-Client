#ifndef NOTIFYSERVICE_H
#define NOTIFYSERVICE_H

#include "core/baseworker.h"
#include "notifyservicei.h"
#include "mosquittopp.h"

namespace BLL{
class NotifyService : public NotifyServiceI,public mosqpp::mosquittopp, public BaseWorker
{
    Q_OBJECT
    enum{
        ConnectMqtt,
        ReconnectMqtt
    };
public:
    NotifyService(WorkerManager *wm,QObject *parent = nullptr);
    WorkerType workerType() const override;
    void initsize() override;

protected:
    void run() override;

private:
    QStringList topticList_;

    void reconnectMqtt();
    void on_connect(int rc) override;
    void on_disconnect(int rc) override;
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
    void on_message(const struct mosquitto_message *message) override;
};
}

#endif // NOTIFYSERVICE_H
