#ifndef NOTIFYBYWEBSOCKET_H
#define NOTIFYBYWEBSOCKET_H

#include <QUrl>
#include <QSslError>
#include <QAbstractSocket>
QT_FORWARD_DECLARE_CLASS(QTimer)
#include "servicei.h"
QT_FORWARD_DECLARE_CLASS(QWebSocket)
class NotifyByWebSocket : public NotifyServiceI
{
    Q_OBJECT
public:
    NotifyByWebSocket(QObject *parent = nullptr);
    ~NotifyByWebSocket();

private:
    QUrl curUrl_;
    QWebSocket *websocket_;
    QTimer *timer_{nullptr};

private slots:
    void slotTimeout();
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);
    void onSslErrors(const QList<QSslError> &errors);
    void slotSocketError(QAbstractSocket::SocketError);
};

#endif // NOTIFYBYWEBSOCKET_H
