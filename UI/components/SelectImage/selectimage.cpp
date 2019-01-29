#include <QPainter>
#include <QMouseEvent>
#include <QBitmap>
#include <QDebug>
#include "selectimage.h"

SelectImage::SelectImage(QWidget *parent)
{

}

QSize SelectImage::sizeHint() const
{
    return QSize(960,540);
}

void SelectImage::setBackImage(QImage img)
{
    originImg_ = img;
    update();
}

void SelectImage::clearImages()
{
    qSwap(QRectF(),curRect_);
    rects_.clear();
    movingFlag_ = false;
    update();
}

QVector<QImage> SelectImage::selectedImages()
{
    QVector<QImage> images;
    std::transform(rects_.begin(),rects_.end(),std::back_inserter(images),[this](QRectF r){
        qreal perWidth = qreal(originImg_.width()) / width();
        qreal perHeight = qreal(originImg_.height()) / height();
        r.setSize(QSize(r.width() * perWidth,r.height() * perHeight));
        r.moveTo(r.x() * perWidth,r.y() * perHeight);
        return originImg_.copy(r.toRect());
    });
    return images;
}

void SelectImage::paintEvent(QPaintEvent *event)
{
    if(rects_.isEmpty() && curRect_.topLeft().isNull()){
        QPainter p(this);
        p.drawImage(rect(),originImg_);
        return;
    };

    maskImg_ = QImage(size(),originImg_.format());
    maskImg_.fill(Qt::red);
    QPixmap maskPix = QPixmap::fromImage(maskImg_);
    maskPix.fill(QColor(0,0,0,152));

    QPainter p(&maskImg_);
    p.setBrush(Qt::blue);
    p.setPen(QPen(Qt::red,penWidth_));
    p.drawRects(rects_);
    QRectF curRect = curRect_;
    curRect.setBottomRight(_endPoint);
    if(!curRect.topLeft().isNull()){
        p.drawRect(curRect);
    }
    p.end();

    QBitmap mask = QPixmap::fromImage(maskImg_).createMaskFromColor(Qt::blue);
    maskPix.setMask(mask);
    p.begin(&maskPix);
    p.setPen(QPen(palette().color(QPalette::Foreground),penWidth_));
    p.drawRects(rects_);
    if(!curRect.topLeft().isNull()){
        p.drawRect(curRect);
    }
    p.end();

    if(_closePathFlag){
        _closePathFlag = false;
        rects_ << curRect;
        qSwap(QRectF(),curRect_);
        movingFlag_ = false;
    }

    p.begin(this);
    p.drawImage(rect(),originImg_);
    p.drawPixmap(rect(),maskPix);

    return QWidget::paintEvent(event);
}

void SelectImage::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        _pressed = true;
        clearFlag_ = false;
        curRect_.setTopLeft(event->pos());
    }else if(event->button() == Qt::RightButton){
        qSwap(QRectF(),curRect_);
        if(!rects_.isEmpty()){
            rects_.removeLast();
        }
        update();
    }

    return QWidget::mousePressEvent(event);
}

void SelectImage::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton)return;
    _endPoint = event->pos();
    if(_endPoint.x() >= width()){
        _endPoint.setX(width() - penWidth_ * 2);
    }else if(_endPoint.x() < 0){
        _endPoint.setX(penWidth_ * 2);
    }
    if(_endPoint.y() >= height()){
        _endPoint.setY(height() - penWidth_ * 2);
    }else if(_endPoint.y() < 0){
        _endPoint.setY(penWidth_ * 2);
    }
    movingFlag_ = true;
    update();
}

void SelectImage::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(_pressed && movingFlag_){
            _closePathFlag = true;
            update();
        }
        _pressed = false;
    }
    return QWidget::mouseReleaseEvent(event);
}

void SelectImage::mouseDoubleClickEvent(QMouseEvent *event)
{
    return QWidget::mouseDoubleClickEvent(event);
}
