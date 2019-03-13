#include <random>
#include <QLabel>
#include <QPainter>
#include <QVBoxLayout>
#include <QApplication>
#include "glviewmapwidget.h"
#include "waitinglabel.h"
#include "informationdialog.h"
#include "movielabel.h"

GlViewMapWidget::GlViewMapWidget(WidgetI *parent):
    WidgetI(parent)
{
    init();
    setUserStyle(userStyle());
    notifyServiceI_ = reinterpret_cast<NotifyServiceI*>(qApp->property("NotifyServiceI").toULongLong());
    connect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
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
        QImage tmpImg("C:/Users/Administrator/Desktop/images/1.png");
        info.sceneImg = tmpImg;
        slotOnIntruderEvent(info);
    }
#endif

//        queryTopStatistics();
        return true;
    }
    return WidgetI::event(event);
}

void GlViewMapWidget::slotOnIntruderEvent(NotifyEventI::IntruderEventData info)
{
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(-150, 100);
    std::uniform_int_distribution<int> dis2(-300, 100);

    MovieLabel *ml = new MovieLabel(info, this); // 更改父级轻松搞定
    ml->setFixedSize(180, 100);
    QRect cr = ml->geometry();

    cr.moveCenter(this->rect().center());
    ml->move(cr.topLeft() + QPoint(dis2(gen), dis(gen)) + QPoint(250 / 2, 360 / 2));
    ml->setInfo(info.deviceName);
    QPalette pal = ml->palette();
    pal.setColor(QPalette::Foreground, Qt::white);
    ml->setPalette(pal);
    ml->show();
    ml->startWaring();
}

void GlViewMapWidget::init()
{
    m_backgroundImg.load("images/glview/map.png");
    setFixedSize(m_backgroundImg.size());
#if 1
    QHBoxLayout *mainHlay = new QHBoxLayout;
    this->setLayout(mainHlay);
    mainHlay->addSpacing(this->width() - 200);
    QVBoxLayout *pMidVlay = new QVBoxLayout;
    pMidVlay->addSpacing(300);
    pMidVlay->setSpacing(10);
    mainHlay->addLayout(pMidVlay);
    QString strTitleStyle = "font-size:16px;color:white;";
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

#endif 0
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
