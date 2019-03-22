#include <random>
#include <QWebChannel>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QWebEnginePage>
#include <QApplication>
#include <QNetworkProxyFactory>
#include <QBuffer>
#include <QSettings>
#include "trackingwebview.h"

TrackingWebView::TrackingWebView(QWidget *parent):
    QWebEngineView(parent)
{
    QWebChannel *channel = new QWebChannel(this);
    webBridge_ = new TrackingBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(webBridge_));
    page()->setWebChannel(channel);
#ifndef Test
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/tracking.html"));
#else
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/calendar-charts.html"));
#endif
    page()->setBackgroundColor(Qt::transparent);
    setContextMenuPolicy(Qt::NoContextMenu);
    connect(webBridge_,SIGNAL(sigCameraClicked(QString)),this,SIGNAL(sigCameraClicked(QString)));
    connect(webBridge_,SIGNAL(sigFaceClicked(QString)),this,SIGNAL(sigFaceClicked(QString)));
    connect(webBridge_,SIGNAL(sigWebError(QString)),this,SIGNAL(sigWebError(QString)));
    QNetworkProxyFactory::setUseSystemConfiguration(false);
    QSettings configSetting("config.ini",QSettings::IniFormat);
    QString webHost_ = configSetting.value("Http/Javahost").toString();
    webBridge_->setHostName(webHost_);
}

QSize TrackingWebView::sizeHint() const
{
    return QSize(1695,927);
}

void TrackingWebView::updateTracking(QVector<TrackingWebView::TrackingPoint> &data)
{
    QHash<QString, int> groupStatistics;
    QJsonArray jsArray;
#if 0
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
        if (!groupStatistics.contains(value.strGroupName))
        {
            groupStatistics.insert(value.strGroupName, 1);
        }
        else
        {
            groupStatistics[value.strGroupName] = groupStatistics.value(value.strGroupName) + 1;
        }
    }
#else
    QVector<TrackingWebView::TrackingPoint>::const_iterator value = data.constEnd() - 1;
    while (value != data.constBegin() - 1)
    {
        QJsonObject jsObj;
        jsObj["pos"] = QJsonObject{{"lat",value->lat},{"lng",value->lng}};
        jsObj["name"] = value->name;
        jsObj["cameraId"] = value->cameraId;
        jsObj["holdTime"] = value->holdTime;
        jsObj["grabTime"] = value->grabTime;
        jsObj["personImg"] = value->personImgUr;
        jsObj["sceneId"] = value->sceneId;
        jsArray << jsObj;
        if (!groupStatistics.contains(value->strGroupName))
        {
            groupStatistics.insert(value->strGroupName, 1);
        }
        else
        {
            groupStatistics[value->strGroupName] = groupStatistics.value(value->strGroupName) + 1;
        }
        --value;
    }
#endif
    webBridge_->updateData(jsArray);

    QJsonArray groupArray;
    QHash<QString, int>::const_iterator iter1 = groupStatistics.constBegin();
    while (iter1 != groupStatistics.constEnd())
    {
        QJsonObject jsObj;
        jsObj["GroupName"] = iter1.key();
        jsObj["GroupCount"] = iter1.value();
        groupArray << jsObj;
        ++iter1;
    }
    webBridge_->updateGroupStatistics(groupArray);
}

void TrackingWebView::updatePersonInfo(QVector<RestServiceI::PortraitLibCompItem> values)
{
    QJsonObject jsObj;
    if (values.count() > 0)
    {
        RestServiceI::PortraitLibCompItem value = values[0];
        QByteArray imgStr;
        QBuffer imgBuf(&imgStr);
        imgBuf.open(QIODevice::WriteOnly);
        value.faceImg.save(&imgBuf,"jpg");
        QString base64Str(imgStr.toBase64()); //QByteArray::Base64UrlEncoding)
        jsObj["personId"] = value.nPersonId;
        jsObj["personName"] = value.strPersonName;
        jsObj["personFace"] = base64Str;
        jsObj["personType"] = value.strSubType;
        jsObj["similarity"] = QString::number(value.dSimilarity * 100, 'g', 2) + "%";
    }
    webBridge_->updatePersonData(jsObj);
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

void TrackingBridge::updatePersonData(QJsonObject &jsArray)
{
    if (isInitsized_)
    {
        emit sigPersonInfo(jsArray);
    }
}

void TrackingBridge::updateGroupStatistics(QJsonArray jsArray)
{
    if (isInitsized_)
    {
        emit sigGroupStatistics(jsArray);
    }
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

void TrackingBridge::onFaceClicked(QString strFaceId)
{
    emit sigFaceClicked(strFaceId);
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
