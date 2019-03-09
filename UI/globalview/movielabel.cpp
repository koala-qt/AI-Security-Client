#include "movielabel.h"

#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QMovie>
#include <QVBoxLayout>
#include <QSettings>
//#include <QElapsedTimer>

MovieLabel::MovieLabel(RestServiceI::CameraInfo info, QWidget *parent) : QWidget(parent)
{
    connect(&tm_,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    setAttribute(Qt::WA_TranslucentBackground);
    backColor_ = QColor("#599AFF");
    m_info = info;
}

void MovieLabel::startWaring()
{
    if (m_fixRadius == 30)
    {
        backColor_ = QColor("#FF4B4B");
    }
    else
    {
        backColor_ = QColor("#FF001B");
    }
    if (!m_isWarning)
    {
        m_isWarning = true;
        tm_.start(5000);
    }
    else
    {
        tm_.start(5000);
    }
}

void MovieLabel::setInfo(QString s)
{
    if (s.length() > 6)
    {
        infoStr_ = s.left(6) + "...";
    }
    else
    {
        infoStr_ = s;
    }
}

int MovieLabel::getCameraId()
{
    return m_info.cameraId;
}

QString &MovieLabel::getDeviceName()
{
    return m_info.deviceName;
}

void MovieLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int infoH = painter.fontMetrics().height();
    QRect textR(0,rect().bottom() - painter.pen().width() - infoH,width() - painter.pen().width(),infoH);
    QRect cr(0,0,textR.width(),textR.top());
    //int radius = cr.width() > cr.height() ? cr.height() : cr.width();
    //int radius = 25;
    QRect ellpiseR; // 内圆
    ellpiseR.setSize(QSize(m_fixRadius, m_fixRadius));
    ellpiseR.moveCenter(cr.center());

#ifdef QT_NO_DEBUG
    m_radius += 0.2;

    if (m_radius >= 40)
    {
        m_radius = 25;
    }
#else
    m_radius += 1;

    if (m_radius >= 40)
    {
        m_radius = 30;
    }
#endif
    painter.setRenderHint(QPainter::Antialiasing);
    QPen pen;
    pen.setColor(backColor_);
    painter.setPen(pen);
    painter.save();
    painter.setBrush(backColor_);
    painter.drawEllipse(ellpiseR);
    painter.setBrush(palette().color(QPalette::Foreground));
    painter.drawText(textR,Qt::AlignCenter,infoStr_);
    painter.restore();
    QRect ellpiseR2;
    if (!m_isWarning)
    {
        painter.setBrush(QColor(89, 153, 255, 100));
    }
    else
    {
        painter.setBrush(QColor(230, 72, 77, 100));
    }
    ellpiseR2.setSize(QSize(m_radius, m_radius));
    ellpiseR2.moveCenter(cr.center());
    painter.drawEllipse(ellpiseR2);
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

void MovieLabel::mouseReleaseEvent(QMouseEvent *event)
{
    QString filePath = qApp->applicationDirPath() + "/position/" + QString::number(m_info.cameraId) + ".ini";
    QSettings fileCfg(filePath, QSettings::IniFormat);
    fileCfg.setValue("point", this->pos());
}

void MovieLabel::enterEvent(QEvent *event)
{
    m_fixRadius = 32;
    if (!m_isWarning)
    {
        backColor_ = QColor("#2566FF");
    }
    else
    {
        backColor_ = QColor("#FF001B");
    }
}

void MovieLabel::leaveEvent(QEvent *event)
{
    m_fixRadius = 30;
    if (!m_isWarning)
    {
        backColor_ = QColor("#599AFF");
    }
    else
    {
        backColor_ = QColor("#FF4B4B");
    }
}

void MovieLabel::slotTimeout()
{
    m_isWarning = false;

    backColor_ = QColor("#599AFF");
}
