#include <random>
#include <QWebChannel>
#include <QDir>
#include <QResizeEvent>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTimer>
#include <QApplication>
#include <QDebug>
#include <QWebEnginePage>
#include "calendarecharts.h"
//#define Test
CalendarEcharts::CalendarEcharts(QWidget *parent):
    QWebEngineView(parent)
{
    QWebChannel *channel = new QWebChannel(this);
    webBridge_ = new Bridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(webBridge_));
    page()->setWebChannel(channel);
#ifndef Test
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/calendar.html"));
#else
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/calendar-charts.html"));
#endif
    page()->setBackgroundColor(Qt::transparent);
}

void CalendarEcharts::updateData(QDate &date, QMap<QDate,int> &datas)
{
    QJsonArray jsArray;
    QMap<QDate,int>::iterator iter(datas.begin());
    while (iter != datas.end()) {
        jsArray << QJsonArray{iter.key().toString("yyyy-MM-dd"),iter.value()};
        iter++;
    }
#ifndef Test
    webBridge_->updateData(date.year(),date.month(),date.day(),jsArray);
#else
    QString str = QString("getVirtulData(%1, %2, %3);").arg(date.year()).arg(date.month()).arg("\"\"");
    this->page()->runJavaScript(str);
#endif
}

void Bridge::onInitsized()
{
    QJsonArray jsArray;
    QDate curDate = QDate::currentDate();
    int dayCount = curDate.day();
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(300,800);
    for(int i = 0; i < dayCount; i++){
        curDate.setDate(curDate.year(),curDate.month(),i + 1);
        jsArray << QJsonArray{curDate.toString("yyyy-MM-dd"),dis(gen)};
    }
    emit updateData(curDate.year(),curDate.month(),curDate.day(),jsArray);
}
