#ifndef REPORTPAGE_H
#define REPORTPAGE_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
QT_FORWARD_DECLARE_CLASS(QWebEngineDownloadItem)
class ReportWebBridge : public QObject
{
    Q_OBJECT
public:
    ReportWebBridge(QObject *parent = nullptr);
    void setHostName(QString);

signals:
    void sigHostNameChanged(QString);

public slots:
    void onInitsized();

private:
    QString host_;
};

class ReportPage : public WidgetI
{
    Q_OBJECT
public:
    ReportPage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;

protected:
    bool event(QEvent *event)override;
    void paintEvent(QPaintEvent *event) override;

private:
    QWebEngineView *webView_{nullptr};
    ReportWebBridge *webBridge_{nullptr};
    QString webHost_;

    QImage backImg_;

private slots:
    void slotLoadRequest(QWebEngineDownloadItem*);
};

#endif // REPORTPAGE_H
