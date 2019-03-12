#include <random>
#include <QWebChannel>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QWebEnginePage>
#include <QApplication>
#include <QNetworkProxyFactory>
#include "trackingwebview.h"

TrackingWebView::TrackingWebView(QWidget *parent):
    QWebEngineView(parent)
{
    QWebChannel *channel = new QWebChannel(this);
    webBridge_ = new TrackingBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(webBridge_));
    page()->setWebChannel(channel);
#ifndef Test
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/routeline.html"));
#else
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/calendar-charts.html"));
#endif
    page()->setBackgroundColor(Qt::transparent);
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(webBridge_,SIGNAL(sigCameraClicked(QString)),this,SIGNAL(sigCameraClicked(QString)));
    connect(webBridge_,SIGNAL(sigWebError(QString)),this,SIGNAL(sigWebError(QString)));
    QNetworkProxyFactory::setUseSystemConfiguration(false);
}

QSize TrackingWebView::sizeHint() const
{
    return QSize(1695,927);
}

void TrackingWebView::updateTracking(QVector<TrackingWebView::TrackingPoint> &data)
{
    QJsonArray jsArray;
    for(const TrackingWebView::TrackingPoint &value : data){
        QJsonObject jsObj;
        jsObj["pos"] = QJsonObject{{"lat",value.lat},{"lng",value.lng}};
        jsObj["name"] = value.name;
        jsObj["cameraId"] = value.cameraId;
        jsObj["holdTime"] = value.holdTime;
        jsObj["grabTime"] = value.grabTime;
        jsObj["personImg"] = value.personImgUr;
        jsObj["sceneId"] = value.sceneId;
        jsArray << jsObj;
    }
    qDebug() << jsArray;
    webBridge_->updateData(jsArray);
}

void TrackingBridge::setHostName(QString s)
{
    hostName_ = s;
}

void TrackingBridge::updateData(QJsonArray &jsArray)
{
    if(isInitsized_){
        emit sigTrackingDataChanged(jsArray);
    }else{
        curJsonArray_ = jsArray;
    }
}

void TrackingBridge::startWaiting()
{
    emit sigMovieingStart();
}

void TrackingBridge::stopWaiting()
{
    emit sigMovieStop();
}

void TrackingBridge::onInitsized()
{
    if(!curJsonArray_.isEmpty()){
        emit sigTrackingDataChanged(curJsonArray_);
    }
    emit sigHostNameChanged(hostName_);
    isInitsized_ = true;
}

void TrackingBridge::onCameraClicked(QString cameraId)
{
    emit sigCameraClicked(cameraId);
}

void TrackingBridge::alertNoPoint(QString str)
{
    emit sigWebError(str);
}

void TrackingWebView::startWaiting()
{
    webBridge_->startWaiting();
}

void TrackingWebView::stopWaiting()
{
    webBridge_->stopWaiting();
}

void TrackingWebView::setHostName(QString s)
{
    webBridge_->setHostName(s);
}
