#include <QSettings>
#include <QTimer>
#include <QUuid>
#include <QDebug>
#include "notifybymqtt.h"
#include "dao/face.pb.h"

#pragma execution_character_set("utf-8")
NotifyByMqtt::NotifyByMqtt(QObject *parent):
    NotifyServiceI(parent)
{
    timer_ = new QTimer(this);
    QThread::connect(timer_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
}

void NotifyByMqtt::run()
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

void NotifyByMqtt::on_connect(int rc)
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

void NotifyByMqtt::on_disconnect(int rc)
{
    qDebug() << "mqtt disconnect";
    if(!timer_->isActive()){
        QMetaObject::invokeMethod(timer_,"start",Q_ARG(int,5000));
    }
}

void NotifyByMqtt::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
    qDebug() << "Subscription successed";
    emit sigInitsized();
}

void NotifyByMqtt::on_message(const mosquitto_message *message)
{
    if(!::qstrcmp(message->topic,"topic/test")){
        kf::FaceInfoPT face_info_pt;
        std::string info(reinterpret_cast<char*>(message->payload),message->payloadlen);
        face_info_pt.ParseFromString(info);
        QImage img;
        img.loadFromData(QByteArray::fromStdString(face_info_pt.img_data()));
        emit sigGrabedPerson(QStringList() << QString::fromStdString(face_info_pt.id())
                             << QString::fromStdString(face_info_pt.camera_pos())
                             << QDateTime::fromMSecsSinceEpoch(face_info_pt.timestamp()).toString("yyyy-MM-dd HH:mm:ss")
                             << QString::fromStdString(face_info_pt.scene_id()),
                             img);
    }else if(!::qstrcmp(message->topic,"face/statis")){
        kf::FaceStatis resInfo;
        std::string info(reinterpret_cast<char*>(message->payload),message->payloadlen);
        resInfo.ParseFromString(info);

        QImage beforImg,afterImg;
        beforImg.loadFromData(QByteArray::fromStdString(resInfo.before_face().img_data()));
        afterImg.loadFromData(QByteArray::fromStdString(resInfo.after_face().img_data()));
        QMap<QString,QVariant> data;
        data.insert("startImg",beforImg);
        data.insert("startLocation",QString::fromStdString(resInfo.before_face().camera_id()));
        data.insert("startTime",QDateTime::fromMSecsSinceEpoch(resInfo.before_face().timestamp()).toString("yyyy-MM-dd HH:mm:ss"));
        data.insert("endImg",afterImg);
        data.insert("endLocation",QString::fromStdString(resInfo.after_face().camera_id()));
        data.insert("endTime",QDateTime::fromMSecsSinceEpoch(resInfo.after_face().timestamp()).toString("yyyy-MM-dd HH:mm:ss"));
        data.insert("timeCost",resInfo.time_cost() / 1000);
        emit sigTimeCost(data);
    }else if(!::strcmp(message->topic,"alarm/blacklistface")){
        kf::BlackListFaceAlarm alarmInfo;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        alarmInfo.ParseFromString(infoStr);

        QImage sceneImg,faceImg;
        faceImg.loadFromData(QByteArray::fromStdString(alarmInfo.face_img()));
        sceneImg.loadFromData(QByteArray::fromStdString(alarmInfo.thumbnail_scene()));
        QStringList infoList;
        infoList << QString::fromStdString(alarmInfo.camera_pos());
        emit sigBlackListAlarmScene(infoList,sceneImg);
        infoList << tr("黑名单报警") << QString::fromStdString(alarmInfo.oid());
        emit sigBlackListAlarmFace(infoList,faceImg);
    }else if(!::strcmp(message->topic,"alarm/intruder")){
        kf::Intruder intruderInfo;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        intruderInfo.ParseFromString(infoStr);

        QImage sceneImg,personImg;
        personImg.loadFromData(QByteArray::fromStdString(intruderInfo.pseron_imgs(0)));
        sceneImg.loadFromData(QByteArray::fromStdString(intruderInfo.thumbnail_scene()));
        QStringList infoList;
        infoList << QString::fromStdString(intruderInfo.camera_pos());
        emit sigIntruderAlarmScene(infoList,sceneImg);
        infoList << tr("闯入报警") << QString::fromStdString(intruderInfo.oid());
        emit sigIntruderAlarmFace(infoList,personImg);
    }else if(!::strcmp(message->topic,"mainpage/snapareagiagram")){
        kf::SnapAreaGiagram grabpicsData;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        grabpicsData.ParseFromString(infoStr);

        QVector<QPointF> dataVec;
        int pSize = grabpicsData.snap_count_point_size();
        for(int i = 0; i < pSize; i++){
            dataVec << QPointF(i,grabpicsData.snap_count_point(i));
        }
        sigAreaGarphics(dataVec);
    }else if(!::strcmp(message->topic,"mainpage/todayfacecount")){
        kf::TodayFaceCount faceGrabData;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        faceGrabData.ParseFromString(infoStr);
        sigFaceGrab(faceGrabData.snap_count(),faceGrabData.blacklist_count(),faceGrabData.served_count(),faceGrabData.blacklist_alarm_count());
    }else if(!::strcmp(message->topic,"mainpage/todayalarmcount")){
        kf::TodayAlarmEventCount alarmingEventCount;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        alarmingEventCount.ParseFromString(infoStr);
        sigAlarmData(alarmingEventCount.ab_count(),alarmingEventCount.intrude_count(),alarmingEventCount.blacklist_count());
    }else if(!::strcmp(message->topic,"mainpage/todaytotal")){
        kf::TodayTotalEventCount totalEvent;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        totalEvent.ParseFromString(infoStr);
        sigTotalEvent(totalEvent.total_count(),totalEvent.blacklist_count());
    }else if(!::strcmp(message->topic,"mainpage/spider")){
        kf::EventSpiderGiagram spiderGiagram;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        spiderGiagram.ParseFromString(infoStr);
        QVector<QVector<double>> spiderData;
        int daySize = spiderGiagram.someday_count_size();
        for(int i = 0; i < daySize; i++){
            QVector<double> oneDayData;
            const kf::OneDayEventCount &oneEvent = spiderGiagram.someday_count(i);
            int oneDaySize = oneEvent.event_count_size();
            for(int j = 0; j < oneDaySize; j++){
                oneDayData << oneEvent.event_count(j);
            }
            spiderData << oneDayData;
        }
        emit sigEventSpider(spiderData);
    }else if(!::strcmp(message->topic,"mainpage/piechart")){
        kf::PieChart pieChart;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        pieChart.ParseFromString(infoStr);
        QVector<kf::PieCharData> qpiedata;
        for(int i = 0; i < pieChart.pie_chart_data_size(); i++){
            qpiedata << pieChart.pie_chart_data(i);
        }
        emit sigPieChart(qpiedata);
    }else if(!::strcmp(message->topic,"alarm/abdoor")){
        kf::ABDoorAlarm abdoorAlarm;
        std::string infoStr(reinterpret_cast<char*>(message->payload),message->payloadlen);
        abdoorAlarm.ParseFromString(infoStr);

        QImage sceneImg,personImg;
        personImg.loadFromData(QByteArray::fromStdString(abdoorAlarm.pseron_img()));
        sceneImg.loadFromData(QByteArray::fromStdString(abdoorAlarm.thumbnail_scene()));
        QStringList infoList;
        infoList << QString::fromStdString(abdoorAlarm.camera_pos());
        emit sigABDoorAlarmScene(infoList,sceneImg);
        infoList << tr("AB门报警") << QString::fromStdString(abdoorAlarm.oid());
        emit sigABDoorAlarmFace(infoList,personImg);
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

void NotifyByMqtt::slotTimeout()
{
    reconnect_async();
}
