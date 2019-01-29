#include "canvaswidget.h"
#include <QPainter>
#include <QDebug>

CanvasWidget::CanvasWidget(QWidget *parent) : QWidget(parent)
{
    m_tm.setSingleShot(true);
    m_tm.setInterval(100);
    connect(&m_tm,SIGNAL(timeout()),this,SLOT(slotTimeout()));
}

void CanvasWidget::slotSetPainterCoordinate(int w, int h)
{
    m_width = w;
    m_height = h;
    m_errorStr.clear();
    update();
}

void CanvasWidget::slotSetErrorStr(QString s)
{
    m_errorStr = s;
    m_width = -1;
    m_height = -1;
    update();
}

void CanvasWidget::slotSetPolygons(const QVector<QPair<QColor, QPolygonF> > &points)
{
    m_listPoints = points;
    update();
}

void CanvasWidget::slotSetRects(QVector<QRect> &rs)
{
    m_tm.stop();
    m_vecRects = rs;
    m_isBox = true;
    update();
}

void CanvasWidget::slotSetPosition(QString s)
{
    m_posStr = s;
}

void CanvasWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);

    QFont f = painter.font();
    f.setPixelSize(16);
    painter.setPen(Qt::red);
    painter.setFont(f);
    QRect r = painter.boundingRect(rect().adjusted(0,5,-5,0),Qt::AlignRight,m_posStr);
    painter.drawText(r,m_posStr);

    if(m_width != -1 && m_height != -1){
        painter.setWindow(0,0,m_width,m_height);
    }
    if(m_errorStr.isEmpty()){
        painter.setRenderHint(QPainter::Antialiasing);
        for(const QPair<QColor, QPolygonF> &pyargs : m_listPoints) {
            painter.setPen(QPen(pyargs.first, 4, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawPolygon(pyargs.second);
        }

        painter.setPen(QPen(Qt::yellow, 3));
        painter.drawRects(m_vecRects);
//        if(m_isBox){
//            m_tm.start();
//        }
    }else{
        QRect pRecr;
        QFont f = painter.font();
//        painter.fillRect(rect(),Qt::black);
        pRecr = rect();
        f.setPixelSize(16);
        painter.setPen(Qt::yellow);
        painter.setFont(f);
        QRect rc = painter.boundingRect(pRecr,Qt::AlignCenter,m_errorStr);
        painter.drawText(rc,m_errorStr);
    }
}

void CanvasWidget::slotTimeout()
{
    std::swap(m_vecRects,QVector<QRect>());
    m_isBox = false;
    update();
}
