#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include "rectsimage.h"

RectsImage::RectsImage(QWidget *parent):
    QWidget(parent)
{
    setRectLineColor();
    setMouseTracking(true);
}

QSize RectsImage::sizeHint() const
{
    return QSize(960,540);
}

void RectsImage::setRectLineColor(QColor c)
{
    rectLineColor_ = c;
}

void RectsImage::setHightlightRect(QRect rc)
{
    hightlightRect_ = rc;
    update();
}

void RectsImage::clearImages()
{
    seletedImages_.clear();
}

QVector<QImage> RectsImage::selectedImages()
{
    return seletedImages_;
}

void RectsImage::setInfos(const QImage backImg, const QVector<QPair<QRect, QImage> > &rectsVec)
{
    backImg_ = backImg;
    rectsImgVec_ = rectsVec;
}

void RectsImage::mousePressEvent(QMouseEvent *event)
{
    for(QPair<QRect,QImage> &pairV : rectsImgVec_){
        QRect wRect;
        wRect.setWidth(pairV.first.width() * xPer_);
        wRect.setHeight(pairV.first.height() * yPer_);
        wRect.moveTo(pairV.first.x() * xPer_,pairV.first.y() * yPer_);
        if(wRect.contains(event->pos())){
            isDown_ = true;
            break;
        }
    }
}

void RectsImage::mouseReleaseEvent(QMouseEvent *event)
{
    if(!isDown_)return;
    for(QPair<QRect,QImage> &pairV : rectsImgVec_){
        QRect wRect;
        wRect.setWidth(pairV.first.width() * xPer_);
        wRect.setHeight(pairV.first.height() * yPer_);
        wRect.moveTo(pairV.first.x() * xPer_,pairV.first.y() * yPer_);
        if(wRect.contains(event->pos())){
            seletedImages_ << pairV.second;
            emit sigClickedImage(pairV.second);
            break;
        }
    }
}

void RectsImage::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    bool inArea = false;
    for(QPair<QRect,QImage> &pairV : rectsImgVec_){
        QRect wRect;
        wRect.setWidth(pairV.first.width() * xPer_);
        wRect.setHeight(pairV.first.height() * yPer_);
        wRect.moveTo(pairV.first.x() * xPer_,pairV.first.y() * yPer_);
        if(wRect.contains(event->pos())){
            QCursor curSor = cursor();
            curSor.setShape(Qt::PointingHandCursor);
            setCursor(curSor);
            inArea = true;
            break;
        }
    }

    if(!inArea){
        QCursor curSor = cursor();
        curSor.setShape(Qt::ArrowCursor);
        setCursor(curSor);
    }
}

void RectsImage::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    xPer_ = (qreal)width() / backImg_.width();
    yPer_ = (qreal)height() / backImg_.height();
}

void RectsImage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setWindow(0,0,backImg_.width(),backImg_.height());
    painter.drawImage(painter.window(),backImg_);
    painter.setPen(rectLineColor_);
    for(QPair<QRect,QImage> &pairV : rectsImgVec_){
        painter.drawRect(pairV.first);
    }
}
