#ifndef NOTIFYSERVICEI_H
#define NOTIFYSERVICEI_H

#include <QObject>
#include <QImage>
#include <QMap>
#include <QVariant>
#include "dao/face.pb.h"

class NotifyServiceI : public QObject
{
    Q_OBJECT
public:
    NotifyServiceI(QObject *parent = nullptr):QObject(parent){
        qRegisterMetaType<QVector<QPointF>>("QVector<QPointF>");
        qRegisterMetaType<QVector<QVector<double> >>("QVector<QVector<double> >");
        qRegisterMetaType<QVector<kf::PieCharData>>("QVector<kf::PieCharData>");
    }
    virtual void initsize() = 0;

signals:
    void sigNetWorkError(QString);
    void sigInitsized();
    void sigAreaGarphics(QVector<QPointF>);
    void sigAlarmData(int,int,int);
    void sigFaceGrab(int,int,int,int);
    void sigEventStatics(int,int);
    void sigTotalEvent(int,int);
    void sigEventSpider(QVector<QVector<double>>);
    void sigBlackListAlarmScene(QStringList,QImage);
    void sigBlackListAlarmFace(QStringList,QImage);
    void sigIntruderAlarmScene(QStringList,QImage);
    void sigIntruderAlarmFace(QStringList,QImage);
    void sigTimeCost(QMap<QString,QVariant>);
    void sigGrabedPerson(QStringList,QImage);
    void sigPieChart(QVector<kf::PieCharData>);
    void sigABDoorAlarmScene(QStringList,QImage);
    void sigABDoorAlarmFace(QStringList,QImage);
    void sigFaceLinkDataFinished(QString);
};
#endif // NOTIFYSERVICEI_H
