#ifndef GRAPHICSARROWITEM_H
#define GRAPHICSARROWITEM_H

#include <QGraphicsItem>
class GraphicsArrowItem : public QGraphicsItem
{
public:
    GraphicsArrowItem(qreal lineLen, QPen &p, qreal arrowWidth = 10,QGraphicsItem *parent = nullptr);
    void setLine(qreal lineLen, qreal arrowWidth, QPen p);
    qreal lineAngle() const;
    QRectF boundingRect() const override;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr);

private:
    QLineF line_,arrow1L_,arrow2L_;
    QVector<QLineF> lines_;
    QPen pen_;
    QRectF curRect_;
};

#endif // GRAPHICSARROWITEM_H
