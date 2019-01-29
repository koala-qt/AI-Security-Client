#include <QPaintEvent>
#include <QPainter>
#include <QPainterPath>
#include "paintarea.h"

PaintArea::PaintArea(QWidget *parent) : QWidget(parent)
{
}

void PaintArea::setCanvasBackground(QPixmap image)
{
    backPix_ = image;
    _imgWidth = image.width();
    _imgHeight = image.height();
    update();
}

QVector<QPolygonF> PaintArea::polygons() const
{
    return polygons_;
}

QSize PaintArea::sizeHint() const
{
    return QSize(960,540);
}

void PaintArea::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.drawPixmap(rect(),backPix_);
    if(polygons_.isEmpty() && curPolygon_.isEmpty())
        return;

    p.setPen(palette().color(QPalette::Foreground));
    if(!polygons_.isEmpty()){
        foreach (const QPolygonF &poly, polygons_) {
            p.drawPolygon(poly);
        }
    }
    if(curPolygon_.isEmpty())return;
    QPolygonF curPolygon(curPolygon_);
    curPolygon << QPointF(_endPoint);
    if(_closePathFlag){
        _closePathFlag = false;
        _startFlag = false;
        setMouseTracking(false);
        p.drawPolygon(curPolygon);
        polygons_ << curPolygon;
        qSwap(QPolygonF(),curPolygon_);
    }else{
        p.drawPolyline(curPolygon);
    }

    return QWidget::paintEvent(event);
}

void PaintArea::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        setMouseTracking(true);
        _startFlag = true;
        _pressed = true;
    }else if(event->button() == Qt::RightButton){
        setMouseTracking(false);
        curPolygon_.clear();
        polygons_.clear();
        update();
    }

    return QWidget::mousePressEvent(event);
}

void PaintArea::mouseMoveEvent(QMouseEvent *event)
{
    _endPoint = event->pos();
    update();
}

void PaintArea::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        if(_pressed && _startFlag){
            curPolygon_ << QPointF(event->pos());
        }
        _pressed = false;
    }
    return QWidget::mouseReleaseEvent(event);
}

void PaintArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    _closePathFlag = true;
    update();
    return QWidget::mouseDoubleClickEvent(event);
}
