#ifndef RIGHTSTATISTICSWEBVIEW_H
#define RIGHTSTATISTICSWEBVIEW_H

#include <QWebEngineView>
#include <QJsonObject>

class RightStatisticsBridge:public QObject
{
    Q_OBJECT
public:
    RightStatisticsBridge(QObject *parent = Q_NULLPTR):QObject(parent)
    {

    }

signals:

private:

};

class RightStatisticsWebView : public QWebEngineView
{
public:
    RightStatisticsWebView(QWidget *parent = Q_NULLPTR);

private:
    RightStatisticsBridge *m_pWebBridge{nullptr};
};

#endif // RIGHTSTATISTICSWEBVIEW_H
