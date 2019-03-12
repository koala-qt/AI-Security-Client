#ifndef TRACKINGWEBVIEW_H
#define TRACKINGWEBVIEW_H

#include <QWebEngineView>
#include <QJsonArray>
#include <QJsonObject>
class TrackingBridge : public QObject
{
    Q_OBJECT
public:
    TrackingBridge(QObject *parent = nullptr): QObject(parent){}
    void setHostName(QString);
    void updateData(QJsonArray &);
    void startWaiting();
    void stopWaiting();

signals:
    void sigHostNameChanged(QString);
    void sigTrackingDataChanged(QJsonArray);
    void sigMovieingStart();
    void sigMovieStop();
    void sigCameraClicked(QString);
    void sigWebError(QString);

public slots:
    void onInitsized();
    void onCameraClicked(QString cameraId);
    void alertNoPoint(QString);

private:
    bool isInitsized_ = false;
    QJsonArray curJsonArray_;
    QString hostName_;
};

class TrackingWebView : public QWebEngineView
{
    Q_OBJECT
public:
    struct TrackingPoint{
        qreal lat;
        qreal lng;
        QString cameraId;
        QString name;
        QString holdTime;
        QString grabTime;
        QString personImgUr;
        QString sceneId;
    };
    TrackingWebView(QWidget *parent = Q_NULLPTR);
    QSize sizeHint() const override;
    void startWaiting();
    void stopWaiting();
    void setHostName(QString);
    void updateTracking(QVector<TrackingPoint> &data);

signals:
    void sigCameraClicked(QString);
    void sigWebError(QString);

private:
    TrackingBridge *webBridge_{nullptr};
};

#endif // TRACKINGWEBVIEW_H
