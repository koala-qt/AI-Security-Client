#include "cornerwidget.h"
#include <QPainter>
#include <QLine>

CornerWidget::CornerWidget(QWidget *parent):
    QWidget(parent)
{

}

void CornerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    int line = 16;
    QPainter p(this);
    p.setPen(QPen(palette().color(QPalette::Foreground),1));
    QVector<QLine> lines;
    lines << QLine(0,0,line,0) << QLine(0,0,0,line) << QLine(width() - line - p.pen().width(),0,width(),0) << QLine(width() - p.pen().width(),0,width() - p.pen().width(),line)
          << QLine(0,height() - line - p.pen().width(),0,height()) << QLine(0,height() - p.pen().width(),line,height() - p.pen().width()) << QLine(width() - line,height() - p.pen().width(),width(),height() - p.pen().width()) << QLine(width() - p.pen().width(),height(),width() - p.pen().width(),height() - line);
    p.drawLines(lines);
}
