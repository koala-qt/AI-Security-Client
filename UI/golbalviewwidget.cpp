#include "golbalviewwidget.h"

#include <random>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QWidget>
#include <QApplication>
#include <QSettings>
#include <QFile>

#include "UI/globalview/bottomstatisticswebview.h"
#include "UI/globalview/leftstatisticswebview.h"
#include "UI/globalview/rightstatisticswebview.h"
#include "waitinglabel.h"
#include "informationdialog.h"
#include "UI/movielabel.h"

GolbalViewWidget::GolbalViewWidget(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Golbal View"));
    init();
    setUserStyle(userStyle());
    getCameraInfo();

}

void GolbalViewWidget::setUserStyle(int style)
{
    if (0 == style)
    {
        this->setStyleSheet(".QWidget{background-image:url(images/glview/glviewbg.png);}");

    }
}

void GolbalViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(), m_backgroundImg);
}

bool GolbalViewWidget::event(QEvent *event)
{
    if ((event->type() == QEvent::Show))
    {
        //queryTopStatistics();
        return true;
    }
    return WidgetI::event(event);
}

void GolbalViewWidget::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    for (RestServiceI::CameraInfo &info : data)
    {
        addDevice(info);
    }
}

void GolbalViewWidget::slotOnIntruderEvent(NotifyEventI::IntruderEventData)
{

}

void GolbalViewWidget::init()
{
    m_backgroundImg.load("images/glview/bg1.png");

    m_mapWgt = new GlViewMapWidget(this);
    m_mapWgt->move(450, 0);

    // web
    m_pBottomWeb = new BottomStatisticsWebView(this);
    m_pBottomWeb->move(0, 0);

    //    QHBoxLayout *mainHLay = new QHBoxLayout;
    //    mainHLay->setMargin(46);
    //    this->setLayout(mainHLay);

    //    mainHLay->addSpacing(400);
    //    QVBoxLayout *pMidVlay = new QVBoxLayout;
    //    mainHLay->addLayout(pMidVlay);
    //    m_mapWgt = new GlViewMapWidget;
    //    pMidVlay->addWidget(m_mapWgt);
    //    pMidVlay->addSpacing(228);
    //    mainHLay->addSpacing(400);
    //
#if 0
    QHBoxLayout *hLay = new QHBoxLayout;
    hLay->setMargin(0);
    mainHLay->addLayout(hLay);
    hLay->addWidget(m_pLeftWeb);
    QVBoxLayout *midVLay = new QVBoxLayout;
    hLay->addLayout(midVLay);
    m_pMidWgt = new QWidget;
    // mid part
    QHBoxLayout *pMidHlay = new QHBoxLayout;
    m_pMidWgt->setLayout(pMidHlay);
    pMidHlay->addStretch();
    QVBoxLayout *pMidVlay = new QVBoxLayout;
    pMidHlay->addLayout(pMidVlay);
    pMidVlay->addSpacing(600);
    QString strTitleStyle = "font-size:18px;color:white;font:bold;";

    m_pBottomWeb = new BottomStatisticsWebView;
    m_pBottomWeb->setFixedHeight(228);
    midVLay->addWidget(m_pBottomWeb);
    m_pRightWeb = new RightStatisticsWebView;
    m_pRightWeb->setFixedWidth(400);
    hLay->addWidget(m_pRightWeb);
#endif
}

void GolbalViewWidget::queryTopStatistics()
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

void GolbalViewWidget::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void GolbalViewWidget::addDevice(RestServiceI::CameraInfo &info)
{
#if 0
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(-50, 50);

    MovieLabel *ml = new MovieLabel(info, m_pMidWgt); // 更改父级轻松搞定
    ml->setFixedSize(80, 65);
    QRect cr = ml->geometry();
    QString filePath = qApp->applicationDirPath() + "/position/"
            + info.cameraId + ".ini";
    if (QFile::exists(filePath))
    {
        QSettings fileCfg(filePath, QSettings::IniFormat);
        QPoint point = fileCfg.value("point").toPoint();
        ml->move(point);
    }
    else
    {
        //cr.moveCenter(rect().center());
        cr.moveCenter(m_pMidWgt->rect().center());
        ml->move(cr.topLeft() + QPoint(dis(gen), dis(gen)));
    }
    ml->setInfo(info.cameraPos);
    QPalette pal = ml->palette();
    pal.setColor(QPalette::Foreground, Qt::white);
    ml->setPalette(pal);
    ml->show();
    m_hashDevices.insert(info.cameraPos, ml);
#endif
}
