#include "movielabel.h"

#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QMovie>
#include <QDebug>
#include <QVBoxLayout>
#include <QSettings>
//#include <QElapsedTimer>

//#define IntruderWarning
static int EventMaxCount = 5;

MovieLabel::MovieLabel(NotifyEventI::IntruderEventData info, QWidget *parent) : QWidget(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    setAttribute(Qt::WA_TranslucentBackground);
    m_innerCircleColor = QColor(Qt::red); // "#84070A"
    m_outerCircleColor = QColor(Qt::red);
    backColor_ = m_innerCircleColor;
    m_info = info;
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setStartValue(20);
    m_animation->setEndValue(100);
    m_animation->setDuration(2000);
    m_animation->setLoopCount(-1);
    m_animation->start();
}

MovieLabel::MovieLabel(NotifyEventI::GlEventData info, QWidget *parent) : QWidget(parent)
{
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slotTimeout()));
    setAttribute(Qt::WA_TranslucentBackground);
    m_innerCircleColor = QColor(Qt::red); // "#84070A"
    m_outerCircleColor = QColor(Qt::red);
    backColor_ = m_innerCircleColor;
    m_glInfo = info;
    m_animation = new QPropertyAnimation(this, "geometry");
    m_animation->setStartValue(20);
    m_animation->setEndValue(100);
    m_animation->setDuration(2000);
    m_animation->setLoopCount(-1);
    m_animation->start();
}

void MovieLabel::startWaring()
{
    if (m_fixRadius == 30)
    {// Inner circle
        backColor_ = m_innerCircleColor;
    }
    else
    {// Outer circle
        backColor_ = m_outerCircleColor;
    }
    if (!m_isWarning)
    {
        m_isWarning = true;
        m_timer.start(5000);
    }
    else
    {
        m_timer.start(5000);
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

QString MovieLabel::getCameraId()
{
    return m_info.sourceId;
}

QString &MovieLabel::getDeviceName()
{
    return m_info.deviceName;
}

void MovieLabel::appendWarningInfo(NotifyEventI::IntruderEventData info)
{
    m_mutex.lock();
    if (m_lstInfos.count() > EventMaxCount)
    {
        m_lstInfos.dequeue();
    }
    m_lstInfos.enqueue(info);
    m_mutex.unlock();
}

void MovieLabel::appendWarningInfo(NotifyEventI::GlEventData info)
{
    m_mutex.lock();
    if (m_stackInfos.count() > EventMaxCount)
    {
        m_stackInfos.pop();
    }
    m_stackInfos.push(info);
    m_mutex.unlock();
}

void MovieLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    int infoH = painter.fontMetrics().height();
    QRect textR(0,rect().bottom() - painter.pen().width() - infoH,width() - painter.pen().width(),infoH);
    QRect cr(0, 110, this->width(), 40);
    QRect devNameRect(0, cr.height(), textR.width(), textR.height());
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
    m_radius += 0.2;

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
    //painter.drawText(devNameRect,Qt::AlignCenter,infoStr_);
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

    QPainter p(this);
    QRect imgRect(0, 0, m_geometry, m_geometry);
#ifdef IntruderWarning
    p.drawImage(0, 0, m_info.sceneImg.scaled(100,100), 0, 0, m_geometry, m_geometry);
    p.drawText(imgRect, Qt::AlignLeft | Qt::AlignTop, "Intrusion");
#else
    p.drawImage(0, 0, m_glInfo.image.scaled(100, 100), 0, 0, m_geometry, m_geometry);
    p.drawText(imgRect, Qt::AlignLeft | Qt::AlignTop, m_glInfo.strEventType);
#endif
    p.save();
    pen.setColor(QColor(Qt::yellow));
    p.setPen(pen);
    p.drawRect(imgRect);
    p.restore();
    QWidget::paintEvent(event);
}

void MovieLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton){
        startPoint_ = event->globalPos();
    }
}

void MovieLabel::mouseMoveEvent(QMouseEvent *event)
{
#if 1
    move(pos() + event->globalPos() - startPoint_);
    startPoint_ = event->globalPos();
    QWidget::mouseMoveEvent(event);
#endif
}

void MovieLabel::mouseReleaseEvent(QMouseEvent *event)
{
#if 0
    QString filePath = qApp->applicationDirPath() + "/position/" + QString::number(m_info.sourceId) + ".ini";
    QSettings fileCfg(filePath, QSettings::IniFormat);
    fileCfg.setValue("point", this->pos());
#endif
}

void MovieLabel::enterEvent(QEvent *event)
{
#if 0
    m_fixRadius = 32;
    if (!m_isWarning)
    {
        backColor_ = QColor("#2566FF");
    }
    else
    {
        backColor_ = QColor("#FF001B");
    }
#endif
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

void MovieLabel::setGeometry(const qreal geometry)
{
    if ((m_geometry != geometry) && (m_geometry < 99))
    {
        m_geometry = geometry;
        emit geometryChanged();
    }
    update();
}

qreal MovieLabel::getGeometry()
{
    return m_geometry;
}

void MovieLabel::slotTimeout()
{

    backColor_ = m_innerCircleColor;
#ifdef IntruderWarning
    if (0 == m_lstInfos.count())
    {
        m_isWarning = false;

        this->hide();
        m_animation->stop();
        m_geometry = 20;
    }
    else
    {
        m_isWarning = true;
        auto warningItem = m_lstInfos.dequeue();
        m_geometry = 20;
        this->setInfo(warningItem.deviceName);
        m_info = warningItem;
        update();
        this->show();
        m_animation->start();
    }
#else
    if (0 == m_stackInfos.count())
    {
        m_isWarning = false;

        this->hide();
        m_animation->stop();
        m_geometry = 20;
    }
    else
    {
        m_isWarning = true;
        auto warningItem = m_stackInfos.pop();
        m_animation->stop();
        m_geometry = 20;
        this->setInfo(warningItem.deviceName);
        m_glInfo = warningItem;
        update();
        this->show();
        m_animation->start();
    }
#endif

}
