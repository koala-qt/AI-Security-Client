#include "baidumap.h"
#include <QWebChannel>
#include <QEventLoop>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDir>
#include <QTimer>

bridge* bridge::instance()
{
    static bridge obj;
    return &obj;
}

bridge::bridge()
{

}

void bridge::showMsgBox()
{
    QMessageBox::aboutQt(0, tr("Qt"));
}

void bridge::getYourName()
{
    emit sigYourName("hello, world!");
}

BaiduMap::BaiduMap(QWidget* parent):
    QWebEngineView(parent)
{
    QWebChannel *channel = new QWebChannel(this);
    channel->registerObject("bridge", (QObject*)bridge::instance());
    page()->setWebChannel(channel);
    load(QUrl(QString("file:///%1/%2/%3").arg(QDir::currentPath()).arg("jsHtml").arg("BaiduMap.html")));

    //    connect(pbAlert, &QPushButton::clicked, [this]() {
    //        m_view->page()->runJavaScript("setLocation(104.064604,30.663546)");
    //    });连接一个信号执行一个函数，函数中调用javaScript的方法。
}

void BaiduMap::slotGetCityName()
{
    QVariant result;
    QSharedPointer<QEventLoop> loop = QSharedPointer<QEventLoop>(new QEventLoop());
    QTimer::singleShot(3000,loop.data(),&QEventLoop::quit);
    page()->runJavaScript("theCity()",[loop,&result](const QVariant &val){
        if(loop->isRunning()){
            result = val;
            loop->quit();
        }
    });
    loop->exec();

    qDebug() << result.toString();
}

void BaiduMap::slotGetWebWidth()
{
    QVariant result;
    QSharedPointer<QEventLoop> loop = QSharedPointer<QEventLoop>(new QEventLoop()); //新建QEventLoop智能指针
    QTimer::singleShot(3000, loop.data(), &QEventLoop::quit);
    page()->runJavaScript("getWindowWidth()", [loop, &result](const QVariant &val) {
        if (loop->isRunning()) {
            result = val;
            loop->quit();
        }
    });
    loop->exec();

    qDebug() << result.toInt();
}

 void BaiduMap::searchLocaltion(double longitude, double latitude)
{
    QString str;
    str.sprintf("setLocation(%.6f,%.6f)",longitude,latitude);
    page()->runJavaScript(str);
 }

 void BaiduMap::slotDrawTrack(QVector<QPair<float,float>> points)
 {
    QJsonArray jsArray;
    for (QPair<float,float> &value : points) {
        jsArray << QJsonObject{{"lng",value.first},{"lat",value.second}};
    }

    QJsonDocument jsDoc(jsArray);
    page()->runJavaScript(QString("drawTrack(%1)").arg(QString(jsDoc.toJson())));
 }

 void BaiduMap::slotClearTrack()
 {
     page()->runJavaScript("clearTrack()");
 }
