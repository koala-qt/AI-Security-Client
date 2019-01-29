#ifndef BAIDUMAP_H
#define BAIDUMAP_H
#include <QWebEngineView>
#include <QPushButton>
#include <QPair>

class bridge : public QObject
{
    Q_OBJECT
public:
    static bridge* instance();

signals:
    void sigYourName(const QString &name);

public slots:
    void showMsgBox();
    void getYourName();

private:
    bridge();
};

class BaiduMap : public QWebEngineView
{
    Q_OBJECT
public:
    BaiduMap(QWidget *parent=0);

public slots:
    void slotGetCityName();
    void slotGetWebWidth();
    void searchLocaltion(double longitude,double latitude);
    void slotDrawTrack(QVector<QPair<float,float>> points);
    void slotClearTrack();
};

#endif // BAIDUMAP_H
