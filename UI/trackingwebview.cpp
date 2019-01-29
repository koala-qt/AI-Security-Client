#include <random>
#include <QWebChannel>
#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QWebEnginePage>
#include <QApplication>
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
}

QSize TrackingWebView::sizeHint() const
{
    return QSize(1695,927);
}

void TrackingWebView::updateTracking(QVector<TrackingWebView::TrackingPoint> &data)
{
    QJsonArray jsArray;
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> disx(0,1139 - 71);
    std::uniform_int_distribution<int> disy(0,640 - 35);
    for(const TrackingWebView::TrackingPoint &value : data){
        QJsonObject jsObj;
        jsObj["pos"] = QJsonObject{{"x",disx(gen)},{"y",disy(gen)}};
        jsObj["name"] = value.name;
        jsObj["holdTime"] = value.holdTime;
        jsObj["grabTime"] = value.grabTime;
        jsObj["personImg"] = value.personImgUr;
        jsArray << jsObj;
    }
    webBridge_->updateData(jsArray);
}

void TrackingBridge::updateData(QJsonArray &jsArray)
{
    if(isInitsized_){
        emit sigTrackingDataChanged(jsArray);
    }else{
        curJsonArray_ = jsArray;
    }
}

void TrackingBridge::onInitsized()
{
//    QJsonArray jsArray;
//    std::random_device device;
//    std::mt19937 gen(device());
//    std::uniform_int_distribution<int> disx(0,1139 - 71);
//    std::uniform_int_distribution<int> disy(0,640 - 35);
//    for(int i = 0; i < 5; i++){
//        QJsonObject jsObj;
//        jsObj["pos"] = QJsonObject{{"x",disx(gen)},{"y",disy(gen)}};
//        jsObj["name"] = tr("camera%1").arg(i);
//        jsObj["holdTime"] = "10:30:10";
//        jsObj["grabTime"] = "2019/01/22 11:33";
//        jsObj["personImg"] = "https://ss2.bdstatic.com/70cFvnSh_Q1YnxGkpoWK1HF6hhy/it/u=4001431513,4128677135&fm=27&gp=0.jpg";
//        jsArray << jsObj;
//    }
//    emit sigTrackingDataChanged(jsArray);
    if(!curJsonArray_.isEmpty()){
        emit sigTrackingDataChanged(curJsonArray_);
    }
    isInitsized_ = true;
}
