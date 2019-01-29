#ifndef CALENDARECHARTS_H
#define CALENDARECHARTS_H

#include <QJsonArray>
#include <QWebEngineView>

class Bridge : public QObject
{
    Q_OBJECT
public:
    Bridge(QObject *parent = nullptr): QObject(parent){}

signals:
    void updateData(int,int,int,QJsonArray);

public slots:
    void onInitsized();
};

class CalendarEcharts : public QWebEngineView
{
    Q_OBJECT
public:
    CalendarEcharts(QWidget *parent=nullptr);
    void updateData(QDate &date, QMap<QDate, int> &datas);

private:
    Bridge *webBridge_{nullptr};
    bool isInit = true;
};

#endif // CALENDARECHARTS_H
