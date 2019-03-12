#include <QUrl>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QWebSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QJsonArray>
#include "notifyeventbywebsocket.h"

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
    quit();
    wait();
}

void NotifyEventByWebSocket::slotTimeout()
{
    QMetaObject::invokeMethod(websocket_,"open",Q_ARG(QUrl, curUrl_));
}

void NotifyEventByWebSocket::onConnected()
{
    connect(websocket_, SIGNAL(textMessageReceived(QString)),this, SLOT(onTextMessageReceived(QString)),Qt::UniqueConnection);
    connect(websocket_, SIGNAL(disconnected()),this,SLOT(onDisconnected()),Qt::UniqueConnection);
    if(timer_->isActive()){
        timer_->stop();
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
    if(jsObj.value("type") != "snap-alarm-type")return;
    jsObj = jsObj.value("data").toObject();
    QString eventType = jsObj.value("eventType").toString();
    if(eventType == "smsr_alarm_intruder"){
        IntruderEventData evData;
        evData.bodyId = jsObj.value("bodyId").toString();
        evData.deviceName = jsObj.value("deviceName").toString();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.eventType = eventType;
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        QJsonArray jsArray = jsObj.value("warnZone").toArray();
        QVector<int> pointVec;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(pointVec),[](QJsonValue jsVal){
            return jsVal.toInt();
        });
        if(!pointVec.isEmpty() && !(pointVec.count() % 2))
        {
            for(int i = 0; i < pointVec.count(); i = i + 2){
                evData.warnZone << QPointF(qreal(pointVec.at(i)) / 2, qreal(pointVec.at(i + 1)) / 2);
            }
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
        evData.deviceName = jsObj.value("deviceName").toString();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.eventType = eventType;
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        QJsonArray jsArray = jsObj.value("warnZone").toArray();
        QVector<int> pointVec;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(pointVec),[](QJsonValue jsVal){
            return jsVal.toInt();
        });
        if(!pointVec.isEmpty() && !(pointVec.count() % 2))
        {
            for(int i = 0; i < pointVec.count(); i = i + 2){
                evData.warnZone << QPointF(qreal(pointVec.at(i)) / 2, qreal(pointVec.at(i + 1)) / 2);
            }
        }
        RestServiceI *serviceI = serFactory_->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigDownloadImage,this,[this,evData](QImage img){
            ABDoorEventData newData = evData;
            newData.sceneImg = img;
            emit sigABDoorEventData(newData);
        });
        connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
            qDebug() << str;
        });
        serviceI->getImageByUrl(jsObj.value("url").toString());
    }else if(eventType == tr("smsr_alarm_climb")){
        ClimbEventData evData;
        evData.bodyId = jsObj.value("bodyId").toString();
        evData.deviceName = jsObj.value("deviceName").toString();
        evData.eventType = eventType;
        evData.lat = jsObj.value("lat").toDouble();
        evData.lng = jsObj.value("lng").toDouble();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.sourceId = jsObj.value("sourceId").toString();
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        QJsonArray jsArray = jsObj.value("warnZone").toArray();
        QVector<int> pointVec;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(pointVec),[](QJsonValue jsVal){
            return jsVal.toInt();
        });
        if(!pointVec.isEmpty() && !(pointVec.count() % 2))
        {
            for(int i = 0; i < pointVec.count(); i = i + 2){
                evData.warnZone << QPointF(qreal(pointVec.at(i)) / 2, qreal(pointVec.at(i + 1)) / 2);
            }
        }
        RestServiceI *serviceI = serFactory_->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigDownloadImage,this,[this,evData](QImage img){
            ClimbEventData newData = evData;
            newData.sceneImg = img;
            emit sigClimbEventData(newData);
        });
        connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
            qDebug() << str;
        });
        serviceI->getImageByUrl(jsObj.value("url").toString());
    }else if(eventType == tr("smsr_alarm_gather")){
        GatherEventData evData;
        evData.deviceName = jsObj.value("deviceName").toString();
        evData.eventType = eventType;
        evData.lat = jsObj.value("lat").toDouble();
        evData.lng = jsObj.value("lng").toDouble();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.sourceId = jsObj.value("sourceId").toString();
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        RestServiceI *serviceI = serFactory_->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigDownloadImage,this,[this,evData](QImage img){
            GatherEventData newData = evData;
            newData.sceneImg = img;
            emit sigGatherEventData(newData);
        });
        connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
            qDebug() << str;
        });
        serviceI->getImageByUrl(jsObj.value("url").toString());
    }else if(eventType == "smsr_alarm_face"){
        PersonEventData evData;
        evData.faceId = jsObj.value("faceId").toString();
        evData.faceSimilarity = jsObj.value("faceSimilarity").toDouble();
        evData.lat = jsObj.value("lat").toDouble();
        evData.lng = jsObj.value("lng").toDouble();
        evData.personId = jsObj.value("personId").toInt();
        evData.personType = jsObj.value("personType").toString();
        evData.personTypenName = jsObj.value("personTypeName").toString();
        evData.sceneId = jsObj.value("sceneId").toString();
        evData.sourceId = jsObj.value("sourceId").toString();
        evData.deviceName = jsObj.value("deviceName").toString();
        evData.timeStamp = QDateTime::fromMSecsSinceEpoch(jsObj.value("timeStamp").toVariant().toULongLong());
        QString faceUrl = jsObj.value("url").toString();
        QJsonArray jsArray = jsObj.value("pictures").toArray();
        QStringList faceUrlList;
        std::transform(jsArray.begin(),jsArray.end(),std::back_inserter(faceUrlList),[](QJsonValue jsVal){
            return jsVal.toString();
        });

        RestServiceI *serviceI = serFactory_->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigDownloadImages,this,[this,evData,faceUrl](QVector<QImage> imgs){
            PersonEventData newData = evData;
            newData.faceImages = imgs;
            RestServiceI *serI = serFactory_->makeRestServiceI();
            connect(serI,&RestServiceI::sigDownloadImage,this,[this,newData](QImage img){
                PersonEventData fdata = newData;
                fdata.image = img;
                emit sigPersonEventData(fdata);
            });
            connect(serI,&RestServiceI::sigError,this,[this](QString str){
                qDebug() << str;
            });
            QString url = faceUrl;
            serI->getImageByUrl(url);
        });
        serviceI->getImagesByUrlList(faceUrlList);
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
