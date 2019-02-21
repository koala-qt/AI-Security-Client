#ifndef NOTIFYBYMQTT_H
#define NOTIFYBYMQTT_H

#include "servicei.h"
#include "mosquittopp.h"
QT_FORWARD_DECLARE_CLASS(QTimer)
class NotifyByMqtt : public NotifyServiceI,public mosqpp::mosquittopp
{
    Q_OBJECT
public:
    NotifyByMqtt(QObject *parent = nullptr);

protected:
    void run() override;

private:
    QTimer *timer_{nullptr};
    QStringList topticList_;

    void on_connect(int rc) override;
    void on_disconnect(int rc) override;
    void on_subscribe(int mid, int qos_count, const int *granted_qos) override;
    void on_message(const struct mosquitto_message *message) override;

private slots:
    void slotTimeout();
};

#endif // NOTIFYBYMQTT_H
