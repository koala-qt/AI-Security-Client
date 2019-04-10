#include <random>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>
#include "glviewmapwidget.h"
#include "waitinglabel.h"
#include "informationdialog.h"
#include "movielabel.h"

//#define SwitchIntruderWarning

GlViewMapWidget::GlViewMapWidget(WidgetI *parent):
    WidgetI(parent)
{
    init();
    setUserStyle(userStyle());
    m_notifyServiceI = reinterpret_cast<NotifyServiceI*>(qApp->property("NotifyServiceI").toULongLong());
    connect(m_notifyServiceI, SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),
            this, SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
    connect(m_notifyServiceI,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),
            this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)),Qt::UniqueConnection);
    connect(m_notifyServiceI,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),
            this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)),Qt::UniqueConnection);
    connect(m_notifyServiceI,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)),
            this,SLOT(slotOnClimbEvent(NotifyEventI::ClimbEventData)));
    connect(m_notifyServiceI,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)),
            this,SLOT(slotOngGatherEvent(NotifyEventI::GatherEventData)));
}

void GlViewMapWidget::setUserStyle(int style)
{
    if (0 == style)
    {
        QString strCommStyle = "font-size:46px;color:white;";
        m_labLocationAccess->setStyleSheet(strCommStyle);
        m_labIDNumbers->setStyleSheet(strCommStyle);
        m_labCameraAccess->setStyleSheet(strCommStyle);
        m_labDataStorage->setStyleSheet(strCommStyle);
    }
}

void GlViewMapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(), m_backgroundImg);
}

bool GlViewMapWidget::event(QEvent *event)
{
    if ((event->type() == QEvent::Show))
    {
#if 0
    for (int i = 0; i < 10; ++i)
    {
        NotifyEventI::IntruderEventData info;
        info.deviceName = "test";
        info.sourceId = "1";
        QImage tmpImg("C:/Users/Administrator/Desktop/images/2.jpg");
        info.sceneImg = tmpImg;

        slotOnIntruderEvent(info);
    }
#endif
        queryTopStatistics();
    }
    return WidgetI::event(event);
}

void GlViewMapWidget::mousePressEvent(QMouseEvent *event)
{
    WidgetI::mousePressEvent(event);
}

void GlViewMapWidget::slotOnIntruderEvent(NotifyEventI::IntruderEventData info)
{
#ifdef SwitchIntruderWarning
    m_mutex.lock();
    if (!m_mapCameras.contains(info.sourceId))
    {
        std::random_device device;
        std::mt19937 gen(device());
        std::uniform_int_distribution<int> dis(-150, 100);
        std::uniform_int_distribution<int> disWidth(-300, 90); // width

        MovieLabel *ml = Q_NULLPTR;
        ml = new MovieLabel(info, this);
        m_mapCameras.insert(info.sourceId, ml);

        //ml->setFixedSize(180, 100);
        ml->setFixedSize(110, 150);
        QRect cr = ml->geometry();

        cr.moveCenter(this->rect().center());
        ml->move(cr.topLeft() + QPoint(disWidth(gen), dis(gen)) + QPoint(250 / 2, 310 / 2));
        ml->setInfo(info.deviceName);
        QPalette pal = ml->palette();
        pal.setColor(QPalette::Foreground, Qt::white);
        ml->setPalette(pal);
        ml->show();
        ml->startWaring();
    }
    else
    {
        auto *ml = m_mapCameras.value(info.sourceId);
        if (Q_NULLPTR != ml)
        {
        ml->appendWarningInfo(info);
        }
    }
    m_mutex.unlock();
#else
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = QObject::tr("Intrusion");
    allEventWarning(glInfo);
#endif
}

void GlViewMapWidget::slotOnAbDoorEvent(NotifyEventI::ABDoorEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = QObject::tr("Trailing");
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOnPersonEvent(NotifyEventI::PersonEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.image;
    if ("100010001008" == info.personType)
    {
        glInfo.strEventType = QObject::tr("BlackList");
    }
    else if ("100010001007" == info.personType)
    {
        glInfo.strEventType = QObject::tr("VIP");
    }
    else if ("100010001005" == info.personType)
    {
        glInfo.strEventType = QObject::tr("Staff");
    }
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOnClimbEvent(NotifyEventI::ClimbEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = QObject::tr("Climbing");
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOngGatherEvent(NotifyEventI::GatherEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = QObject::tr("Gathering");
    allEventWarning(glInfo);
}

void GlViewMapWidget::init()
{
    m_backgroundImg.load("images/glview/chengdu.png");
    setFixedSize(m_backgroundImg.size());
    QHBoxLayout *mainHlay = new QHBoxLayout;
    this->setLayout(mainHlay);
    mainHlay->addSpacing(this->width() - 160);
    QVBoxLayout *pMidVlay = new QVBoxLayout;
    pMidVlay->addSpacing(300);
    pMidVlay->setSpacing(10);
    mainHlay->addLayout(pMidVlay);
    QString strTitleStyle = "font-size:18px;color:white;font:bold;";
    QLabel *labTitle = new QLabel(tr("Location Access"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(labTitle);
    m_labLocationAccess = new QLabel(tr("100"));
    m_labLocationAccess->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(m_labLocationAccess);
    labTitle = new QLabel(tr("Camera Access"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(labTitle);
    m_labCameraAccess = new QLabel(tr("1200"));
    m_labCameraAccess->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(m_labCameraAccess);
    labTitle = new QLabel(tr("Total ID Numbers"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(labTitle);
    m_labIDNumbers = new QLabel(tr("710"));
    m_labIDNumbers->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(m_labIDNumbers);
    labTitle = new QLabel(tr("Data Storage"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(labTitle);
    m_labDataStorage = new QLabel(tr("10GB"));
    m_labDataStorage->setAlignment(Qt::AlignRight);
    pMidVlay->addWidget(m_labDataStorage);
    pMidVlay->addStretch();
    mainHlay->addSpacing(15);
}

void GlViewMapWidget::queryTopStatistics()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI, &RestServiceI::sigError, this, [this, label](const QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
    });
    connect(serviceI, &RestServiceI::sigGLViewTopStatisResult, this, [&, label](const RestServiceI::GLViewTopStatistics value){
        m_labLocationAccess->setText(value.strLocationAccess);
        m_labCameraAccess->setText(value.strCameraAccess);
        m_labIDNumbers->setText(value.strTotalIDNumbers);
        m_labDataStorage->setText(value.strDataStorage);
        label->close();
        delete label;

    });
    serviceI->queryGLViewTopStatistics("2019-03-02");
    label->show(500);
}

void GlViewMapWidget::allEventWarning(NotifyEventI::GlEventData info)
{
    if (info.image.isNull())
    {
        return;
    }
    m_mutex.lock();
    if (!m_mapCameras.contains(info.deviceId))
    {
        std::random_device device;
        std::mt19937 gen(device());
        std::uniform_int_distribution<int> dis(-300, 50); // height
        std::uniform_int_distribution<int> disWidth(-250, 90); // width

        MovieLabel *ml = Q_NULLPTR;
        ml = new MovieLabel(info, this);
        m_mapCameras.insert(info.deviceId, ml);

        //ml->setFixedSize(180, 100);
        ml->setFixedSize(110, 150);
        QRect cr = ml->geometry();

        cr.moveCenter(this->rect().center());
        ml->move(cr.topLeft() + QPoint(disWidth(gen), dis(gen)) + QPoint(220, 175)); // Half of the central area
        ml->setInfo(info.deviceName);
        QPalette pal = ml->palette();
        pal.setColor(QPalette::Foreground, Qt::white);
        ml->setPalette(pal);
        ml->show();
        ml->startWaring();
    }
    else
    {
        auto *ml = m_mapCameras.value(info.deviceId);
        if (Q_NULLPTR != ml)
        {
            ml->appendWarningInfo(info);
        }
    }
    m_mutex.unlock();
}
