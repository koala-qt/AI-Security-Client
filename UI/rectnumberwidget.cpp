#include <QPainter>
#include "rectnumberwidget.h"

RectNumberWidget::RectNumberWidget(QWidget *parent)
{

}

void RectNumberWidget::setValue(const QString v)
{
    value_ = v;
}

void RectNumberWidget::setTitle(const QString title, const Qt::AlignmentFlag f)
{
    title_ = title;
    titleAlign_ = f;
}

void RectNumberWidget::setAllPen(const QPen &borderPen, const QPen &pointPen, const QPen &valuePen, const QPen &titlePen)
{
    borderPen_ = borderPen;
    pointPen_ = pointPen;
    valuePen_ = valuePen;
    titletPen_ = titlePen;
}

void RectNumberWidget::setAllFont(const QFont &valueFont, const QFont &titleFont)
{
    valueFont_ = valueFont;
    titleFont_ = titleFont;
}

QPen RectNumberWidget::borderPen() const
{
    return borderPen_;
}

QPen RectNumberWidget::pointPen() const
{
    return pointPen_;
}

QPen RectNumberWidget::valuePen() const
{
    return valuePen_;
}

QPen RectNumberWidget::titlePen() const
{
    return titletPen_;
}

QFont RectNumberWidget::valueFont() const
{
    return valueFont_;
}

QFont RectNumberWidget::titleFont() const
{
    return titleFont_;
}

QString RectNumberWidget::title() const
{
    return title_;
}

Qt::AlignmentFlag RectNumberWidget::titleAlign() const
{
    return titleAlign_;
}

QString RectNumberWidget::value() const
{
    return value_;
}

void RectNumberWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QFontMetrics titleMetrics(titleFont());
    int halfTitleH = static_cast<int>(::ceil(titleMetrics.height() / 2));
    QRect borderRect;
    borderRect.setTop(VSPACE + halfTitleH);
    borderRect.setBottom(height() - titlePen().width() - VSPACE - halfTitleH);
    borderRect.setLeft(0);
    borderRect.setRight(width() - borderPen().width());

    QPolygon polygon;
    int leftXP = borderRect.x() + static_cast<int>(::ceil(borderRect.width() * g_shortborderper));
    polygon << QPoint(leftXP,borderRect.y())
            << QPoint(leftXP,borderRect.bottom())
            << QPoint(borderRect.right() - leftXP,borderRect.y())
            << QPoint(borderRect.right() - leftXP,borderRect.bottom());
    QVector<QLine> lines;
    lines << QLine(borderRect.topLeft(),polygon.first())
          << QLine(borderRect.topLeft(),borderRect.bottomLeft())
          << QLine(borderRect.bottomLeft(),polygon.at(1))
          << QLine(borderRect.topRight(),polygon.at(2))
          << QLine(borderRect.topRight(),borderRect.bottomRight())
          << QLine(borderRect.bottomRight(),polygon.last());
    painter.setPen(pointPen());
    painter.drawPoints(polygon);
    painter.setPen(borderPen());
    painter.drawLines(lines);

    QRect textBoundingRect;
    switch (titleAlign()) {
    case Qt::AlignTop:
        textBoundingRect = QRect(QPoint(leftXP,borderRect.y() - halfTitleH),QPoint(borderRect.right() - leftXP,borderRect.y() + halfTitleH));
        break;
    case Qt::AlignBottom:
        textBoundingRect = QRect(QPoint(leftXP,borderRect.bottom() - halfTitleH),QPoint(borderRect.right() - leftXP,borderRect.bottom() + halfTitleH));
        break;
    default:
        break;
    }
    painter.setFont(titleFont());
    painter.setPen(titlePen());
    painter.drawText(textBoundingRect,Qt::AlignCenter,title());

    painter.setFont(valueFont());
    painter.setPen(valuePen());
    painter.drawText(borderRect,Qt::AlignCenter,value());
    return QWidget::paintEvent(event);
}
