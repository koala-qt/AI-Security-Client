#include <QPainter>
#include <QCursor>
#include <QMouseEvent>
#include <QBitmap>
#include "rectsimage.h"

RectsImage::RectsImage(QWidget *parent):
    QWidget(parent)
{
    setRectLineColor();
    setShowRect();
    setMouseTracking(true);
}

QSize RectsImage::sizeHint() const
{
    return QSize(960,540);
}

void RectsImage::setShowRect(bool face, bool body)
{
    showFaceRect_ = face;
    showBodyRect_ = body;
}

void RectsImage::setRectLineColor(QColor face, QColor body)
{
    rectLineColor_ = face;
    bodyRectLineColor_ = body;
}

void RectsImage::setClickedHight(bool s)
{
    clickedHightlight_ = s;
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

void RectsImage::setInfos(const QImage backImg, const QVector<QPair<QRect, QImage> > &rectsVec, const QVector<QPair<QRect, QImage> > &bodyRectImgVec)
{
    backImg_ = backImg;
    rectsImgVec_ = rectsVec;
    bodyRectImgVec_ = bodyRectImgVec;
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
            hightlightRect_ = pairV.first;
            clickedHightlight_ = true;
            update();
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
    if(showFaceRect_){
        painter.setPen(rectLineColor_);
        for(QPair<QRect,QImage> &pairV : rectsImgVec_){
            painter.drawRect(pairV.first);
        }
    }
    if(showBodyRect_){
        painter.setPen(bodyRectLineColor_);
        for(QPair<QRect,QImage> &pairV : bodyRectImgVec_){
            painter.drawRect(pairV.first);
        }
    }
    painter.end();

    if(clickedHightlight_){
        QPixmap menban(backImg_.size());
        menban.fill(QColor(0,0,0,100));
        QPixmap mask(backImg_.size());
        mask.fill(Qt::transparent);
        painter.begin(&mask);
        painter.setPen(Qt::blue);
        painter.fillRect(hightlightRect_.adjusted(-painter.pen().width(),-painter.pen().width(),painter.pen().width(),painter.pen().width()),Qt::blue);
        menban.setMask(mask.createMaskFromColor(Qt::blue));
        painter.end();
        painter.begin(this);
        painter.setWindow(0,0,backImg_.width(),backImg_.height());
        painter.drawPixmap(painter.window(),menban);
    }
}
