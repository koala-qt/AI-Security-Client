#ifndef GOLBALVIEWWIDGET_H
#define GOLBALVIEWWIDGET_H

#include "widgetinterface.h"

#include <QWebEngineView>

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

class GlobalWebView : public QWebEngineView
{
    Q_OBJECT
public:
    GlobalWebView(QWidget *parent = Q_NULLPTR);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    bool event(QEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *ev) override;

private:
    GolbalWebBridge *m_webBridget{nullptr};
    QWidget *m_child{nullptr};
};

class GolbalViewWidget : public WidgetI
{
    Q_OBJECT
public:
    GolbalViewWidget(WidgetI *parent = nullptr);
    void setUserStyle(int style) override;

    void mousePressEvent(QMouseEvent *event);

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
    GlobalWebView *m_webView{nullptr};

    GlViewMapWidget *m_mapWgt{nullptr};
};

#endif // GOLBALVIEWWIDGET_H
