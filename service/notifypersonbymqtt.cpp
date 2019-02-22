#include <QSettings>
#include <QTimer>
#include <QUuid>
#include <QDebug>
#include "notifypersonbymqtt.h"
#include "dao/face.pb.h"

#pragma execution_character_set("utf-8")
NotifyPersonByMqtt::NotifyPersonByMqtt(QObject *parent):
    NotifyPersonI(parent)
{
    timer_ = new QTimer(this);
    QThread::connect(timer_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
}

void NotifyPersonByMqtt::run()
{
    QSettings settins("config.ini",QSettings::IniFormat);
    int num = settins.beginReadArray("Mqtt/topic");
    for(int i = 0; i < num; i++){
        settins.setArrayIndex(i);
        topticList_ << settins.value("topic").toString();
    }
    settins.endArray();
    QString clientId = QUuid::createUuid().toString();
    mosqpp::mosquittopp::reinitialise(clientId.toStdString().data(),true);
    mosqpp::lib_init();
    int keepalive = 60;
    mosqpp::mosquittopp::username_pw_set(settins.value("Mqtt/username").toString().toStdString().data(),settins.value("Mqtt/password").toString().toStdString().data());
    bool resCode = mosqpp::mosquittopp::connect(settins.value("Mqtt/ip").toString().toStdString().data(),settins.value("Mqtt/port").toInt(),keepalive);
    if(resCode == MOSQ_ERR_SUCCESS){
        qDebug() << tr("连接成功") << QThread::currentThreadId();
    }else{
        qDebug() << tr("连接错误") << QThread::currentThreadId();
        emit sigNetWorkError(tr("连接错误"));
        if(!timer_->isActive()){
            QMetaObject::invokeMethod(timer_,"start",Q_ARG(int,5000));
        }
    }

    while (!isInterruptionRequested()) {
        mosqpp::mosquittopp::loop();
        QThread::msleep(1);
    }
    mosqpp::lib_cleanup();
}

void NotifyPersonByMqtt::on_connect(int rc)
{
    qDebug() << "mqtt connect" << QThread::currentThreadId();
    if(timer_->isActive()){
        QMetaObject::invokeMethod(timer_,"stop");
    }
    foreach (const QString str, topticList_) {
        int resCode = subscribe(nullptr,str.toStdString().data());
        qDebug() << "mqtt subscribe" << str << "return" << resCode;
    }
}

void NotifyPersonByMqtt::on_disconnect(int rc)
{
    qDebug() << "mqtt disconnect";
    if(!timer_->isActive()){
        QMetaObject::invokeMethod(timer_,"start",Q_ARG(int,5000));
    }
}

void NotifyPersonByMqtt::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    qDebug() << "Subscription successed";
}

void NotifyPersonByMqtt::on_message(const mosquitto_message *message)
{
    if(!::qstrcmp(message->topic,"topic/test")){
        kf::FaceInfoPT face_info_pt;
        std::string info(reinterpret_cast<char*>(message->payload),message->payloadlen);
        face_info_pt.ParseFromString(info);

        FaceSnapEventData evData;
        evData.cameraId = QString::fromStdString(face_info_pt.id());
        evData.cameraPos = QString::fromStdString(face_info_pt.camera_pos());
        evData.sceneId = QString::fromStdString(face_info_pt.scene_id());
        evData.snapTime = QDateTime::fromMSecsSinceEpoch(face_info_pt.timestamp());
        evData.faceImg.loadFromData(QByteArray::fromStdString(face_info_pt.img_data()));
        emit sigFaceSnap(evData);
    }else if(!::strcmp(message->topic,"facelinkpage/tree")){
        kf::Tree faceLinkPoint;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        faceLinkPoint.ParseFromString(infoStr);

        int status = faceLinkPoint.status();
        QString oid = QString::fromStdString(faceLinkPoint.oid());
        QString msg = QString::fromStdString(faceLinkPoint.msg());
        emit sigFaceLinkDataFinished(oid);
    }
}

void NotifyPersonByMqtt::slotTimeout()
{
    reconnect_async();
}
