#ifndef RESOURCEMANAGEPAGE_H
#define RESOURCEMANAGEPAGE_H

#include <QWebEngineDownloadItem>
#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QWebEngineView)

class ResourceWebBridge : public QObject
{
    Q_OBJECT
public:
    ResourceWebBridge(QObject *parent = nullptr);
    void setHostName(QString);

signals:
    void sigHostNameChanged(QString);

public slots:
    void onInitsized();

private:
    QString host_;
};

class ResourceManagePage : public WidgetI
{
    Q_OBJECT
public:
    ResourceManagePage(WidgetI *parent = nullptr);
    void setUserStyle(int s = 0) override;
    void loadWebPage(int index);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QWebEngineView *webView_{nullptr};
    ResourceWebBridge *webBridge_{nullptr};
    QString webHost_;

    QImage backImg_;

private slots:
    void slotLoadRequest(QWebEngineDownloadItem*download);
};

#endif // RESOURCEMANAGEPAGE_H
