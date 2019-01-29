#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QQueue>
#include "service/core/workermanagerapi.h"
#include "service/restservicei.h"
#include "klvideowidget.h"
QT_FORWARD_DECLARE_CLASS(CanvasWidget)
QT_FORWARD_DECLARE_CLASS(QMenu)
class VideoPlayer : public Klvideowidget
{
    Q_OBJECT
public:
    explicit VideoPlayer(QWidget *parent = nullptr);
    void setWorkerManager(BLL::WorkerManager *wm);
    void play(QString url, QString decoderFactoryName, QString id, QString name = QString());
    CanvasWidget* canvasWidget() const;
    QString cameraId() const;

public slots:
    void slotSetPolygons(QVector<QPair<int, QPolygonF> > points);
    void slotSetRects(QVector<QRect> rs);

private:
    QString m_deviceId,oldRtsp_;
    int reconnectCount_ = 0;
    QMenu *menu_{nullptr};
    CanvasWidget *m_canvas{nullptr};
    QQueue<std::tuple<QString,QString,int>> m_playQueue;

    BLL::WorkerManager *workerM_{nullptr};

private slots:
    void slotOnStarted(int w,int h);
};

#endif // VIDEOPLAYER_H
