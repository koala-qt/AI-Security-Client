#ifndef TRACKINGWEBVIEW_H
#define TRACKINGWEBVIEW_H

#include <QWebEngineView>
#include <QJsonArray>
class TrackingBridge : public QObject
{
    Q_OBJECT
public:
    TrackingBridge(QObject *parent = nullptr): QObject(parent){}
    void updateData(QJsonArray &);

signals:
    void sigTrackingDataChanged(QJsonArray);

public slots:
    void onInitsized();

private:
    bool isInitsized_ = false;
    QJsonArray curJsonArray_;
};

class TrackingWebView : public QWebEngineView
{
    Q_OBJECT
public:
    struct TrackingPoint{
        int xpos;
        int ypos;
        QString name;
        QString holdTime;
        QString grabTime;
        QString personImgUr;
    };
    TrackingWebView(QWidget *parent = Q_NULLPTR);
    QSize sizeHint() const override;
    void updateTracking(QVector<TrackingPoint> &data);

private:
    TrackingBridge *webBridge_{nullptr};
};

#endif // TRACKINGWEBVIEW_H
