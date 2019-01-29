#include <QPainter>
#include <QDebug>
#include "graphicsarrowitem.h"

GraphicsArrowItem::GraphicsArrowItem(qreal lineLen, QPen &p, qreal arrowWidth, QGraphicsItem *parent):
    pen_(p),
    QGraphicsItem(parent)
{
    setLine(lineLen,arrowWidth,p);
}

void GraphicsArrowItem::setLine(qreal lineLen, qreal arrowWidth, QPen p)
{
    line_ = QLineF(QPointF(0,0),QPointF(0 + lineLen,0));
    pen_ = p;
    arrow1L_.setPoints(line_.p2(),QPointF(line_.p2().x() - arrowWidth,0));
    arrow1L_.setAngle(135);
    arrow2L_.setPoints(line_.p2(),QPointF(line_.p2().x() - arrowWidth,0));
    arrow2L_.setAngle(-135);
    curRect_.setHeight(arrowWidth * 2 + pen_.width() * 3);
    curRect_.setWidth(line_.length() + pen_.width() * 2);
    lines_.clear();
    lines_ << line_ << arrow1L_ << arrow2L_;
}

qreal GraphicsArrowItem::lineAngle() const
{
    return line_.angle();
}

QRectF GraphicsArrowItem::boundingRect() const
{
    return curRect_;
}

void GraphicsArrowItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)
    painter->save();
    painter->setPen(pen_);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawLines(lines_);
    painter->restore();
}
