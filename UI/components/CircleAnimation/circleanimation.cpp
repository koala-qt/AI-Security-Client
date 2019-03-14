#include <QPainter>
#include <QPropertyAnimation>
#include <QEvent>
#include "circleanimation.h"

CircleAnimation::CircleAnimation(QWidget *parent)
{
    animation_ = new QPropertyAnimation(this,"value");
    animation_->setDuration(800);
    QPalette pal = palette();
    pal.setColor(QPalette::Foreground,QColor("#292966"));
    pal.setColor(QPalette::Text,QColor(255,255,255,0.75));
    setPalette(pal);
}

void CircleAnimation::setStartValue(const qreal v)
{
    animation_->setStartValue(v);
}

void CircleAnimation::setEndValue(const qreal v)
{
    animation_->setEndValue(v);
}

void CircleAnimation::setValue(const qreal v)
{
    value_ = v;
    update();
}

qreal CircleAnimation::value() const
{
    return value_;
}

void CircleAnimation::paintEvent(QPaintEvent *event)
{
    if(value_ == 0)return;
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    int radius = std::min(rect().width() - p.pen().width(),rect().height() - p.pen().width());
    QRect rt;
    rt.setSize(QSize(radius,radius));
    rt.moveCenter(rect().center());

    QRadialGradient rg(rt.center(),radius >> 1,rt.center());
    rg.setColorAt(0,Qt::transparent);
    rg.setColorAt(0.8,Qt::transparent);
    QColor c("#292966");
    rg.setColorAt(0.81,c);
    rg.setColorAt(1,c);
    p.setBrush(rg);
    p.drawEllipse(rt.center(),radius >> 1,radius >> 1);

    c.setNamedColor("#4741F2");
    rg.setColorAt(0.81,c);
    rg.setColorAt(1,c);
    p.setBrush(rg);
    p.drawPie(rt, 30 * 16 - 30 * 16, -value_ * 360 * 16 - 30 * 16);

    p.setPen(QColor(255,255,255,191));
    QString str;
    str.setNum(value_ * 100,'g',4);
    p.drawText(rt,Qt::AlignCenter,str + '%');
}

bool CircleAnimation::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        animation_->start();
    }

    return QWidget::event(event);
}
