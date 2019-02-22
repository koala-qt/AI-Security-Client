#ifndef NOTIFYBYWEBSOCKET_H
#define NOTIFYBYWEBSOCKET_H

#include <QUrl>
#include <QSslError>
#include <QAbstractSocket>
#include "servicei.h"
QT_FORWARD_DECLARE_CLASS(QTimer)
QT_FORWARD_DECLARE_CLASS(QWebSocket)
class NotifyEventByWebSocket : public NotifyEventI
{
    Q_OBJECT
public:
    NotifyEventByWebSocket(QObject *parent = nullptr);
    ~NotifyEventByWebSocket();

private:
    QUrl curUrl_;
    QWebSocket *websocket_;
    ServiceFactoryI *serFactory_{nullptr};
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
