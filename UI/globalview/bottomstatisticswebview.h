#ifndef BOTTOMSTATISTICSWEBVIEW_H
#define BOTTOMSTATISTICSWEBVIEW_H

#include <QWebEngineView>
#include <QJsonObject>

class BottomStatisticsBridge:public QObject
{
    Q_OBJECT
public:
    BottomStatisticsBridge(QObject *parent = Q_NULLPTR):QObject(parent)
    {

    }

signals:

private:

};

class BottomStatisticsWebView : public QWebEngineView
{
public:
    BottomStatisticsWebView(QWidget *parent = Q_NULLPTR);

private:
    BottomStatisticsBridge *m_pWebBridge{nullptr};
};

#endif // BOTTOMSTATISTICSWEBVIEW_H
