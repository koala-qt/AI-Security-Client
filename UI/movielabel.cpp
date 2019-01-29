#include <QPainter>
#include <QMouseEvent>
#include "movielabel.h"

MovieLabel::MovieLabel(QWidget *parent) : QWidget(parent)
{
    connect(&tm_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    setAttribute(Qt::WA_TranslucentBackground);
    backColor_ = Qt::green;
}

void MovieLabel::startWaring()
{
    tm_.start(300);
    QTimer::singleShot(2000,this,[&]{
        tm_.stop();
        backColor_ = Qt::green;
        update();
    });
}

void MovieLabel::setInfo(QString s)
{
    infoStr_ = s;
}

void MovieLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int infoH = painter.fontMetrics().height();
    QRect textR(0,rect().bottom() - painter.pen().width() - infoH,width() - painter.pen().width(),infoH);
    QRect cr(0,0,textR.width(),textR.top());
    int radius = cr.width() > cr.height() ? cr.height() : cr.width();
    QRect ellpiseR;
    ellpiseR.setSize(QSize(radius,radius));
    ellpiseR.moveCenter(cr.center());
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(backColor_);
    painter.drawEllipse(ellpiseR);
    painter.setBrush(palette().color(QPalette::Foreground));
    painter.drawText(textR,Qt::AlignCenter,infoStr_);
    QWidget::paintEvent(event);
}

void MovieLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        startPoint_ = event->globalPos();
    }
}

void MovieLabel::mouseMoveEvent(QMouseEvent *event)
{
    move(pos() + event->globalPos() - startPoint_);
    startPoint_ = event->globalPos();
    QWidget::mouseMoveEvent(event);
}

void MovieLabel::slotTimeout()
{
    if(backColor_ == Qt::red){
        backColor_ = Qt::green;
    }else{
        backColor_ = Qt::red;
    }
    update();
}
