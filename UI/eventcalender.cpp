#include <QPainter>
#include "eventcalender.h"

EventCalender::EventCalender(QWidget *parent):
    QCalendarWidget(parent)
{

}

void EventCalender::setMaxValue(const int v)
{
    maxValue_ = v;
}

int EventCalender::maxValue() const
{
    return maxValue_;
}

void EventCalender::setEventBackColor(const QColor &c)
{
    c_ = c;
}

QColor EventCalender::eventBackColor() const
{
    return c_;
}
#if 1
void EventCalender::paintCell(QPainter *painter, const QRect &rect, const QDate &date) const
{
    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    qreal radius = qreal(qMin(rect.width(),rect.height())) / (2 * maxValue_);
    qreal curRadiu = 300 * radius;
    painter->setBrush(c_);
    painter->drawEllipse(QPointF(rect.center()),curRadiu,curRadiu);
    painter->restore();
}
#endif
