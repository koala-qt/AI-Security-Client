#ifndef TREECHARTS_H
#define TREECHARTS_H

#include <QWebEngineView>
#include <QJsonObject>

class TreeBridge : public QObject
{
    Q_OBJECT
public:
    TreeBridge(QObject *parent = nullptr): QObject(parent){}
    void updateData(QJsonObject &);
    void startWaiting();
    void stopWaiting();
    bool isInited();

signals:
    void sigDataChanged(QJsonObject);
    void sigStartWaiting();
    void sigStopWaiting();

public slots:
    void onInitsized();

private:
    bool isInitsized_ = false;
    QJsonObject curJsobj_;
};

class TreeCharts : public QWebEngineView
{
    Q_OBJECT
public:
    TreeCharts(QWidget *parent = Q_NULLPTR);
    void updateData(QJsonObject &jsObj);
    void startWaiting();
    void stopWaiting();

private:
    TreeBridge *webBridge_{nullptr};
};

#endif // TREECHARTS_H
