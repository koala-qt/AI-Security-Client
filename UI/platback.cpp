#include <QPaintEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include "platback.h"

PlatBack::PlatBack(QWidget *parent):
    QWidget(parent)
{
    animation_ = new QPropertyAnimation(this,"angle");
    animation_->setStartValue(0);
    animation_->setEndValue(-360);
    animation_->setDuration(2000);
    animation_->setLoopCount(-1);
    animation_->start();
}

void PlatBack::loadImage(QImage &plat, QImage &shield, QImage &shadow)
{
    plat_ = plat;
    shiedld_ = shield;
    shadow_ = shadow;
}

void PlatBack::paintEvent(QPaintEvent *event)
{
#if 0
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);
    QRect windowRect(QPoint(0,0),QPoint(950,925));
    painter.setWindow(windowRect);
    QTransform transform;
    int x0 = 355,y0 = 675;
    transform.translate(x0,y0);
    transform.rotate(77,Qt::XAxis);
    transform.rotate(angel_,Qt::ZAxis);
    painter.setTransform(transform);
    painter.drawImage(QRect(QPoint(-300,-300),QPoint(300,300)),plat_);

    transform.reset();
    transform.translate(x0,y0);
    painter.setTransform(transform);
    painter.drawImage(QRect(QPoint(-225,-550),QPoint(225,-15)),shiedld_);
    painter.drawImage(QRect(QPoint(-330,-550),QPoint(340,64)),shadow_);
#endif
}

qreal PlatBack::angle() const
{
    return angel_;
}

void PlatBack::setAngle(const qreal ang)
{
    if(angel_ != ang){
        emit angleChanged();
    }
    angel_ = ang;
    update();
}
