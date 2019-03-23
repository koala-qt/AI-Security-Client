#include <QWebChannel>
#include <QDir>
#include <QResizeEvent>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <QWebEnginePage>
#include "treecharts.h"

TreeCharts::TreeCharts(QWidget *parent)
{
    QWebChannel *channel = new QWebChannel(this);
    webBridge_ = new TreeBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(webBridge_));
    page()->setWebChannel(channel);
#ifndef Test
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/tree.html"));
#else
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/calendar-charts.html"));
#endif
    page()->setBackgroundColor("#282D38");
    setContextMenuPolicy(Qt::NoContextMenu);
}

void TreeCharts::updateData(QJsonObject &jsObj)
{
    webBridge_->updateData(jsObj);
}

void TreeCharts::startWaiting()
{
    webBridge_->startWaiting();
}

void TreeCharts::stopWaiting()
{
    webBridge_->stopWaiting();
}

void TreeBridge::updateData(QJsonObject &jsObj)
{
    qDebug() << jsObj;
    if(isInitsized_){
        emit sigDataChanged(jsObj);
    }else{
        curJsobj_ = jsObj;
    }
}

void TreeBridge::startWaiting()
{
    emit sigStartWaiting();
}

void TreeBridge::stopWaiting()
{
    emit sigStopWaiting();
}

bool TreeBridge::isInited()
{
    return isInitsized_;
}

void TreeBridge::onInitsized()
{
    if(!curJsobj_.isEmpty()){
        emit sigDataChanged(curJsobj_);
    }
    isInitsized_ = true;
}
