#ifndef CAMERAMAPSCENE_H
#define CAMERAMAPSCENE_H

#include <QGraphicsScene>
#include "service/servicei.h"
#include "widgetinterface.h"
class CameraMapScene : public QGraphicsScene
{
    Q_OBJECT
public:
    typedef struct _pointInfo
    {
        QString cameraId;
        QDateTime stayTime;
    }PointInfo;
    CameraMapScene(QObject *parent = nullptr);
    void setPersonTracking(QVector<PointInfo>);

private:
};

#endif // CAMERAMAPSCENE_H
