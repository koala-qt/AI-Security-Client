#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QWebEngineView)

class ReportWebBridge : public QObject
{
    Q_OBJECT
public:
    ReportWebBridge(QObject *parent = nullptr);
    void setHostName(QString);

signals:
    void  sigHostNameChanged(QString);

public slots:
    void onInitsized();

private:
    QString hostName_;
};

class ReportPage : public WidgetI
{
    Q_OBJECT
public:
    ReportPage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;

private:
    QWebEngineView *webView_{nullptr};
    ReportWebBridge *webBridge_{nullptr};
    QString webHost_;
};

#endif // REPORTPAGE_H
