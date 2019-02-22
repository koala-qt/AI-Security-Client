#include <QUrl>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QJsonArray>
#include "notifybywebsocket.h"

NotifyEventByWebSocket::NotifyEventByWebSocket(QObject *parent):
    NotifyEventI(parent)
{
    QSettings config("config.ini",QSettings::IniFormat);
    curUrl_ = QUrl(config.value("WebSocket/url").toString());
    timer_ = new QTimer(this);
    websocket_ = new QWebSocket();
    serFactory_ = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    connect(timer_,SIGNAL(timeout()),this,SLOT(slotTimeout()));

    connect(websocket_, SIGNAL(connected()), this, SLOT(onConnected()));
    qRegisterMetaType<QAbstractSocket::SocketError>("QAbstractSocket::SocketError");
    connect(websocket_,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(slotSocketError(QAbstractSocket::SocketError)));
    connect(websocket_,SIGNAL(sslErrors(QList<QSslError>)),this,SLOT(onSslErrors(QList<QSslError>)));
    websocket_->open(curUrl_);

    moveToThread(this);
}

NotifyEventByWebSocket::~NotifyEventByWebSocket()
{
    websocket_->close();
    websocket_->deleteLater();
}

void NotifyEventByWebSocket::slotTimeout()
{
    websocket_->open(curUrl_);
}

void NotifyEventByWebSocket::onConnected()
{
    connect(websocket_, SIGNAL(textMessageReceived(QString)),this, SLOT(onTextMessageReceived(QString)),Qt::UniqueConnection);
    if(timer_->isActive()){
        timer_->start(5000);
    }
}

void NotifyEventByWebSocket::onDisconnected()
{
    websocket_->close();
    if(!timer_->isActive()){
        timer_->start(5000);
    }
}

void NotifyEventByWebSocket::onTextMessageReceived(QString message)
{
    QJsonParseError jsError;
    QJsonDocument jsDoc = QJsonDocument::fromJson(message.toLatin1(),&jsError);
    if(jsError.error != QJsonParseError::NoError){
        qDebug() << jsError.errorString();
        return;
    }
    QJsonObject jsObj = jsDoc.object();
    QString eventType = jsObj.value("eventType").toString();
    if(eventType == "smsr_alarm_intruder"){
        IntruderEventData evData;
        evData.bodyId = jsObj.value("bodyId").toString();
        evData.deviceId = jsObj.value("deviceId").toInt();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        QJsonArray jsArray = jsObj.value("warnZone").toArray();
        QVector<int> pointVec;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(pointVec),[](QJsonValue jsVal){
            return jsVal.toInt();
        });
        if(!pointVec.isEmpty() && !(pointVec.count() % 2))
        {
            std::transform(pointVec.begin(),pointVec.end() - 1,pointVec.begin() + 1,std::back_inserter(evData.warnZone),[](int x,int y){
                return QPointF(qreal(x)/2,qreal(y)/2);
            });
        }
        RestServiceI *serviceI = serFactory_->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigDownloadImage,this,[this,evData](QImage img){
            IntruderEventData newData = evData;
            newData.sceneImg = img;
            emit sigIntruderEvent(newData);
        });
        connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
            qDebug() << str;
        });
        serviceI->getImageByUrl(jsObj.value("url").toString());
    }else if(eventType == "smsr_alarm_abdoor"){
        ABDoorEventData evData;
        evData.bodyId = jsObj.value("bodyId").toString();
        evData.deviceId = jsObj.value("deviceId").toInt();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
    }else if(eventType == "smsr_alarm_face"){

    }
}

void NotifyEventByWebSocket::onSslErrors(const QList<QSslError> &errors)
{
    for(const QSslError &err : errors){
        qDebug() << metaObject()->className() << err.errorString();
    }
}

void NotifyEventByWebSocket::slotSocketError(QAbstractSocket::SocketError e)
{
    qDebug() << metaObject()->className() << e;
}
