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

#if 0
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
#endif
}

void GlViewMapWidget::setUserStyle(int style)
{
    if (0 == style)
    {
        QString strCommStyle = "font-size:54px;color:white;";
        m_labLocationAccess->setStyleSheet(strCommStyle);
        m_labIDNumbers->setStyleSheet(strCommStyle);
        m_labCameraAccess->setStyleSheet(strCommStyle);
        m_labDataStorage->setStyleSheet(strCommStyle);
    }
}

void GlViewMapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
#if 0
    QPainter p(this);
    p.drawImage(rect(), m_backgroundImg);
#endif
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
    glInfo.strEventType = "Intrusion";
    allEventWarning(glInfo);
#endif
}

void GlViewMapWidget::slotOnAbDoorEvent(NotifyEventI::ABDoorEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = "Trailing";
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOnPersonEvent(NotifyEventI::PersonEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.image;
    glInfo.strEventType = "BlackList";
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOnClimbEvent(NotifyEventI::ClimbEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = "Climbing";
    allEventWarning(glInfo);
}

void GlViewMapWidget::slotOngGatherEvent(NotifyEventI::GatherEventData info)
{
    NotifyEventI::GlEventData glInfo;
    glInfo.deviceId = info.sourceId;
    glInfo.deviceName = info.deviceName;
    glInfo.image = info.sceneImg;
    glInfo.strEventType = "Gathering";
    allEventWarning(glInfo);
}

void GlViewMapWidget::init()
{
    m_backgroundImg.load("images/glview/map.png");
    setFixedSize(m_backgroundImg.size());
    QHBoxLayout *mainHlay = new QHBoxLayout;
    this->setLayout(mainHlay);
    mainHlay->addSpacing(this->width() - 160);
    QVBoxLayout *pMidVlay = new QVBoxLayout;
    pMidVlay->addSpacing(100);
    pMidVlay->setSpacing(10);
    mainHlay->addLayout(pMidVlay);
    QWidget *parentWgt = new QWidget;
    QString strWgtStyle = ".QWidget{background-color:rgba(53,76,97,0.8);height:140px;"
                          "min-height:140px;border-radius:4px;}";
    parentWgt->setStyleSheet(strWgtStyle);
    QVBoxLayout *vLay = new QVBoxLayout;
    vLay->setContentsMargins(5, 20, 5, 20);
    parentWgt->setLayout(vLay);
    pMidVlay->addWidget(parentWgt);
    QString strTitleStyle = "font-size:18px;color:white;";
    QLabel *labTitle = new QLabel(tr("Location Access"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignLeft);
    vLay->addWidget(labTitle);
    m_labLocationAccess = new QLabel(tr("100"));
    m_labLocationAccess->setAlignment(Qt::AlignLeft);
    vLay->addWidget(m_labLocationAccess);

    parentWgt = new QWidget;
    parentWgt->setStyleSheet(strWgtStyle);
    vLay = new QVBoxLayout;
    vLay->setContentsMargins(5, 20, 5, 20);
    parentWgt->setLayout(vLay);
    pMidVlay->addWidget(parentWgt);
    labTitle = new QLabel(tr("Camera Access"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignLeft);
    vLay->addWidget(labTitle);
    m_labCameraAccess = new QLabel(tr("1200"));
    m_labCameraAccess->setAlignment(Qt::AlignLeft);
    vLay->addWidget(m_labCameraAccess);

    parentWgt = new QWidget;
    parentWgt->setStyleSheet(strWgtStyle);
    vLay = new QVBoxLayout;
    vLay->setContentsMargins(5, 20, 5, 20);
    parentWgt->setLayout(vLay);
    pMidVlay->addWidget(parentWgt);
    labTitle = new QLabel(tr("Total Number of IDs"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignLeft);
    vLay->addWidget(labTitle);
    m_labIDNumbers = new QLabel(tr("710"));
    m_labIDNumbers->setAlignment(Qt::AlignLeft);
    vLay->addWidget(m_labIDNumbers);

    parentWgt = new QWidget;
    parentWgt->setStyleSheet(strWgtStyle);
    vLay = new QVBoxLayout;
    vLay->setContentsMargins(5, 20, 5, 20);
    parentWgt->setLayout(vLay);
    pMidVlay->addWidget(parentWgt);
    labTitle = new QLabel(tr("Data Storage"));
    labTitle->setStyleSheet(strTitleStyle);
    labTitle->setAlignment(Qt::AlignLeft);
    vLay->addWidget(labTitle);
    m_labDataStorage = new QLabel(tr("10GB"));
    m_labDataStorage->setAlignment(Qt::AlignLeft);
    vLay->addWidget(m_labDataStorage);
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
        std::uniform_int_distribution<int> dis(-150, 100);
        std::uniform_int_distribution<int> disWidth(-300, 90); // width

        MovieLabel *ml = Q_NULLPTR;
        ml = new MovieLabel(info, this);
        m_mapCameras.insert(info.deviceId, ml);

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
        auto *ml = m_mapCameras.value(info.deviceId);
        if (Q_NULLPTR != ml)
        {
            ml->appendWarningInfo(info);
        }
    }
    m_mutex.unlock();
}
