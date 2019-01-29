#include <random>
#include <QGraphicsAnchorLayout>
#include <QGraphicsWidget>
#include <QApplication>
#include <QDebug>
#include "cameramapscene.h"
#include "graphicsarrowitem.h"
#include "service/restservice.h"

CameraMapScene::CameraMapScene(QObject *parent)
{
    addPixmap(tr("map.png"));
    workerM_ = reinterpret_cast<BLL::WorkerManager*>(qApp->property("WorkerManager").toULongLong());
    QVector<CameraMapScene::PointInfo> trackingDataVec;
    trackingDataVec << CameraMapScene::PointInfo{"fsrewr",QDateTime(QDate::currentDate(),QTime(10,10,10))}
                    << CameraMapScene::PointInfo{"fsrewr",QDateTime(QDate::currentDate(),QTime(10,10,10))}
                    << CameraMapScene::PointInfo{"fsrewr",QDateTime(QDate::currentDate(),QTime(10,10,10))}
                    << CameraMapScene::PointInfo{"fsrewr",QDateTime(QDate::currentDate(),QTime(10,10,10))}
                    << CameraMapScene::PointInfo{"fsrewr",QDateTime(QDate::currentDate(),QTime(10,10,10))};
    setPersonTracking(trackingDataVec);
}

void CameraMapScene::setPersonTracking(QVector<CameraMapScene::PointInfo> datas)
{
    std::random_device device;
    std::mt19937 gen(device());
    QVector<QPointF> originPfs;
    for(CameraMapScene::PointInfo &pointItem : datas){
        QGraphicsPixmapItem *pixmapItem = addPixmap(tr("security-camera.ico"));
        QGraphicsSimpleTextItem *posTextItem = addSimpleText(tr("camera id:%1").arg(pointItem.cameraId),QFont("Arial",10,1,false));
        QGraphicsSimpleTextItem *timeTextItem = addSimpleText(tr("stayTime:%1").arg(pointItem.stayTime.toString("yyyy/MM/dd HH:mm:ss")),QFont("Arial",10,1,false));
        posTextItem->setPos(pixmapItem->x() + pixmapItem->boundingRect().width(),pixmapItem->y());
        timeTextItem->setPos(posTextItem->x(),posTextItem->y() + posTextItem->boundingRect().height());
        QGraphicsItemGroup *pointInfoItem = createItemGroup(QList<QGraphicsItem*>() << pixmapItem << posTextItem << timeTextItem);
        std::uniform_real_distribution<float> disWidth(0,width() - pointInfoItem->boundingRect().width()),disHeight(0,height() - pointInfoItem->boundingRect().height());
        pointInfoItem->setPos(disWidth(gen),disHeight(gen));
        originPfs << QPointF(pointInfoItem->pos() + pixmapItem->boundingRect().center());
    }

    for(int i = 0; i < originPfs.count() - 1; i++){
        QLineF line(originPfs.at(i),originPfs.at(i+1));
        GraphicsArrowItem *arrow = new GraphicsArrowItem(line.length() - 20,QPen(Qt::yellow),7);
        addItem(arrow);
        arrow->setPos(line.p1());
        arrow->setRotation(-line.angle());
    }
}
