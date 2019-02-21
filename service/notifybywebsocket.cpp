#include <QUrl>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QWebSocket>
#include <QUuid>
#include "notifybywebsocket.h"

NotifyByWebSocket::NotifyByWebSocket(QObject *parent):
    NotifyServiceI(parent)
{
    QSettings config("config.ini",QSettings::IniFormat);
    curUrl_ = QUrl(config.value("WebSocket/url").toString());
    timer_ = new QTimer(this);
    websocket_ = new QWebSocket();
    connect(timer_,SIGNAL(timeout()),this,SLOT(slotTimeout()));

    connect(websocket_, SIGNAL(connected()), this, SLOT(onConnected()));
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    connect(websocket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotSocketError(QAbstractSocket::SocketError)));
    connect(websocket_,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(onSslErrors(QList<QSslError>)));
    websocket_->open(curUrl_);

    moveToThread(this);
}

NotifyByWebSocket::~NotifyByWebSocket()
{
    websocket_->close();
    websocket_->deleteLater();
}

void NotifyByWebSocket::slotTimeout()
{
    websocket_->open(curUrl_);
}

void NotifyByWebSocket::onConnected()
{
    connect(websocket_, SIGNAL(textMessageReceived(QString)),this, SLOT(onTextMessageReceived(QString)),Qt::UniqueConnection);
    if(timer_->isActive()){
        timer_->start(5000);
    }
}

void NotifyByWebSocket::onDisconnected()
{
    websocket_->close();
    if(!timer_->isActive()){
        timer_->start(5000);
    }
}

void NotifyByWebSocket::onTextMessageReceived(QString message)
{
    qDebug() << message;
}

void NotifyByWebSocket::onSslErrors(const QList<QSslError> &errors)
{
    for(const QSslError &err : errors){
        qDebug() << metaObject()->className() << err.errorString();
    }
}

void NotifyByWebSocket::slotSocketError(QAbstractSocket::SocketError e)
{
    qDebug() << metaObject()->className() << e;
}
