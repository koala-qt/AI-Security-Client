#ifndef LEFTSTATISTICSWEBVIEW_H
#define LEFTSTATISTICSWEBVIEW_H

#include <QWebEngineView>
#include <QJsonObject>

class LeftStatisticsBridge: public QObject
{
    Q_OBJECT
public:
    LeftStatisticsBridge(QObject *parent = Q_NULLPTR):QObject(parent)
    {

    }

signals:

private:

};

class LeftStatisticsWebView : public QWebEngineView
{
public:
    LeftStatisticsWebView(QWidget *parent = Q_NULLPTR);

private:
    LeftStatisticsBridge *m_pWebBridge{nullptr};
};

#endif // LEFTSTATISTICSWEBVIEW_H
