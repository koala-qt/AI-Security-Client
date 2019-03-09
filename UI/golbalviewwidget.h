#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"
QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QWebEngineView)
QT_FORWARD_DECLARE_CLASS(GlViewMapWidget)

class GolbalWebBridge : public QObject
{
    Q_OBJECT
public:
    GolbalWebBridge(QObject *parent = nullptr);
    void setHostName(QString);

signals:
    void sigHostNameChanged(QString);

public slots:
    void onInitsized();

private:
    QString host_;
};

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int style) override;

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QString webHost_;
    QImage m_backgroundImg;
    QLabel *menbanL_{nullptr};

    // web
    QWebEngineView *webView_{nullptr};
    GolbalWebBridge *webBridge_{nullptr};

    GlViewMapWidget *m_mapWgt{nullptr};
};

#endif // GOLBALVIEWWIDGET_H
