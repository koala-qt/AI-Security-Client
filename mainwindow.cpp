#include "mainwindow.h"
#include <QResizeEvent>
#include <QListWidget>
#include <QStackedWidget>
#include <QScrollBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QApplication>
#include <QPainter>
#include <QLinearGradient>
#include <QFontMetrics>
#include <QDesktopWidget>
#include <QPushButton>
#include <QMenu>
#include <QLinearGradient>
#include <QDynamicPropertyChangeEvent>
#include "UI/targetsearch.h"
#include "UI/videoplayback.h"
#include "UI/realtimemonitoring.h"
#include "UI/eventsearch.h"
#include "UI/reportpage.h"
#include "UI/resourcemanagepage.h"
#include "UI/golbalviewwidget.h"
#include "UI/hompage.h"
#include <QDebug>

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(WidgetI *parent)
    : WidgetI(parent)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    logoLabel_ = new QLabel;
    logoLabel_->setPixmap(QPixmap("images/appLogo.png"));
//    logoLabel_->setScaledContents(true);
    topBorderLine_ = new QLabel;
    appNameL_ = new QLabel(qApp->applicationName());
    appNameL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    topBorderLine_->setFixedHeight(1);
    topWgt_ = new QWidget;
    m_centerW = new QStackedWidget;
    m_topList = new QListWidget;
    resourceXialaMenu_ = new QMenu(m_topList);
    m_topList->setFocusPolicy(Qt::NoFocus);
    m_topList->setFrameStyle(QFrame::NoFrame);
    m_topList->setFlow(QListWidget::LeftToRight);
    setUserStyle(userStyle());

    QStackedWidget *monitoringStackedW = new QStackedWidget;
    HomPage *homeW = new HomPage;
    RealtimeMonitoring *videoW = new RealtimeMonitoring;
    monitoringStackedW->addWidget(homeW);
    monitoringStackedW->addWidget(videoW);
    connect(homeW,&HomPage::sigSwitchBtnClicked,monitoringStackedW,[monitoringStackedW]{monitoringStackedW->setCurrentIndex(1);});
    connect(videoW,&RealtimeMonitoring::sigSwitchBtnClicked,videoW,[monitoringStackedW]{monitoringStackedW->setCurrentIndex(0);});
    monitoringStackedW->setObjectName(tr("Surveillance"));

    m_centerW->addWidget(new GolbalViewWidget());
    m_centerW->addWidget(monitoringStackedW);
    m_centerW->addWidget(new EventSearch());
    m_centerW->addWidget(new TargetSearch());
    m_centerW->addWidget(new ReportPage());
    ResourceManagePage *resouPage = new ResourceManagePage;
    m_centerW->addWidget(resouPage);

    int viewW = 0;
    m_topList->setIconSize(QSize(36,16));
    QFontMetrics topListfs = m_topList->fontMetrics();
    for(int i = 0; i < m_centerW->count(); i++){
        QImage pixImg(tr("images/%1-%2.png").arg(i).arg(i));
        QImage iconDefaultImage(pixImg.width() + 20,pixImg.height(),QImage::Format_ARGB32);
        iconDefaultImage.fill(Qt::transparent);
        QPainter p(&iconDefaultImage);
        p.drawImage(QRect(iconDefaultImage.width() - pixImg.width(),0,pixImg.width(),pixImg.height()),pixImg);
        p.end();

        pixImg.load(tr("images/%1.png").arg(i));
        QImage iconSelectedImage(iconDefaultImage.size(),QImage::Format_ARGB32);
        iconSelectedImage.fill(Qt::transparent);
        p.begin(&iconSelectedImage);
        p.drawImage(QRect(iconDefaultImage.width() - pixImg.width(),0,pixImg.width(),pixImg.height()),pixImg);
        p.end();

        QListWidgetItem *item = new QListWidgetItem;
        item->setText(tr(" ") + m_centerW->widget(i)->objectName());
        QIcon icon;
        icon.addPixmap(QPixmap::fromImage(iconDefaultImage),QIcon::Normal);
        icon.addPixmap(QPixmap::fromImage(iconSelectedImage),QIcon::Selected);
        item->setIcon(icon);
        item->setTextAlignment(Qt::AlignLeft + Qt::AlignVCenter);
        item->setSizeHint(QSize(topListfs.width(item->text()) + 40 + m_topList->iconSize().width(),-1));
        viewW += item->sizeHint().width();
        m_topList->addItem(item);
    }
    resourceXialaMenu_->setMinimumWidth(m_topList->item(m_topList->count() - 1)->sizeHint().width());
    m_topList->setFixedWidth(viewW + (m_topList->count() + 1) * m_topList->spacing() + 2 * m_topList->frameWidth());
    QAction *ac = new QAction(tr("Devices"),resourceXialaMenu_);
    ac->setData(m_centerW->count() - 1);
    connect(ac,&QAction::triggered,resourceXialaMenu_,[this,ac,resouPage]{
        m_centerW->setCurrentIndex(ac->data().toInt());
        resouPage->loadWebPage(0);
    });
    resourceXialaMenu_->addAction(ac);
    ac = new QAction(tr("ID Database"),resourceXialaMenu_);
    ac->setData(m_centerW->count() - 1);
    connect(ac,&QAction::triggered,resourceXialaMenu_,[this,ac,resouPage]{
        m_centerW->setCurrentIndex(ac->data().toInt());
        resouPage->loadWebPage(1);
    });
    resourceXialaMenu_->addAction(ac);
    ac = new QAction(tr("Registration Database"),resourceXialaMenu_);
    ac->setData(m_centerW->count() - 1);
    connect(ac,&QAction::triggered,resourceXialaMenu_,[this,ac,resouPage]{
        m_centerW->setCurrentIndex(ac->data().toInt());
        resouPage->loadWebPage(2);
    });
    resourceXialaMenu_->addAction(ac);

    topWgt_->setFixedHeight(60);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(logoLabel_);
    hlay->addWidget(appNameL_);
    hlay->addStretch();
    hlay->addWidget(m_topList);
    hlay->setContentsMargins(15, 0, 0, 0);
    topWgt_->setLayout(hlay);
    mainLay->addWidget(topWgt_);//1
    mainLay->addWidget(topBorderLine_);
    mainLay->addWidget(m_centerW);//16
    mainLay->setMargin(0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    connect(m_topList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotItemClicked(QListWidgetItem*)));
    m_topList->itemClicked(m_topList->item(0));
    m_topList->setCurrentRow(0);
    m_topList->currentItemChanged(m_topList->item(0),nullptr);
    appNameL_->hide();
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUserStyle(int s)
{
    QPalette pal;
    QFont f = font();
    f.setFamily("微软雅黑"); //DINCond-Bold、PingFang SC Regular、微软雅黑 Microsoft YaHei UI、Source Sans Pro
    f.setWeight(QFont::Normal);
    qApp->setFont(f);

    f.setWeight(QFont::Bold);
    f.setPixelSize(16);
    m_topList->setFont(f);
    f.setPixelSize(14);
    resourceXialaMenu_->setFont(f);

    f = appNameL_->font();
    f.setWeight(QFont::Bold);
    f.setPixelSize(14);
    appNameL_->setFont(f);
    if(s == 0){
        pal = palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        setPalette(pal);
        setAutoFillBackground(true);

        pal = m_centerW->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        m_centerW->setPalette(pal);
        m_centerW->setAutoFillBackground(true);

        pal = topBorderLine_->palette();
        topBorderLine_->setLineWidth(topBorderLine_->height());
        topBorderLine_->setFrameStyle(QFrame::HLine | QFrame::Plain);
        pal.setColor(QPalette::Foreground,QColor(59,71,79).darker());
        topBorderLine_->setPalette(pal);

        pal = appNameL_->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255));
        setPalette(pal);

        m_topList->setStyleSheet("QListWidget{"
                                 "background-color: transparent;"
                                 "}"
                                 "QListWidget::item{"
                                 "color: rgba(255,255,255,191);"
                                 "}"
                                 "QListWidget::item:selected{"
                                 "color: white;"
                                 "background-color: rgb(71,65,242);"
                                 "}");
        qApp->setStyleSheet("QMenu{"
                            "background-color: #2B313D;"
                            "color: rgba(255,255,255,0.75);"
                            "}"
                            "QMenu::item:selected{"
                            "background-color: #4741F2;"
                            "color: white;"
                            "}"
                            "QMenu::item:disabled{"
                            "color: rgba(255,255,255,0.3);"
                            "}"
                            "QMenu::item:disabled:hover{"
                            "background-color: #2B313D;"
                            "}");
        resourceXialaMenu_->setStyleSheet("QMenu{"
                                          "color: rgb(126,140,177);"
                                          "background-color: rgb(43,49,61);"
                                          "}"
                                          "QMenu::item{"
                                          "height: 30px;"
                                          "padding-left: 15px;"
                                          "}"
                                          "QMenu::item:selected{"
                                          "color: white;"
                                          "background-color: rgb(71,65,242);"
                                          "}");
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    for(int i = 0; i < m_topList->count(); i++){
        m_topList->item(i)->setSizeHint(QSize(m_topList->item(i)->sizeHint().width(),m_topList->height() - 2 * m_topList->frameWidth()));
    }
    if(userStyle() == 0){
        QPalette pal = topWgt_->palette();
        QLinearGradient backGradient;
        backGradient.setColorAt(0.0,QColor(70,95,122));
        backGradient.setColorAt(0.46,QColor(40,55,73));
        backGradient.setColorAt(0.49,QColor(28,40,52));
        backGradient.setColorAt(1.0,QColor(43,62,83));
        backGradient.setStart(topWgt_->rect().topLeft());
        backGradient.setFinalStop(topWgt_->rect().bottomLeft());
        backGradient.setSpread(QGradient::RepeatSpread);
        pal.setBrush(QPalette::Background,backGradient);
        topWgt_->setPalette(pal);
        topWgt_->setAutoFillBackground(true);
    }
    return WidgetI::resizeEvent(event);
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
#if 0
    QWidgetList wlist = QApplication::allWidgets();
    for(QWidget *wid : wlist){
        QDynamicPropertyChangeEvent *ev = new QDynamicPropertyChangeEvent("danyahei");
        QApplication::postEvent(dynamic_cast<QObject*>(wid),ev);
    }
#endif
}

void MainWindow::slotItemClicked(QListWidgetItem *item)
{
    int index = m_topList->row(item);
    if(index == m_topList->count() - 1){
        resourceXialaMenu_->move(m_topList->mapToGlobal(QPoint(m_topList->width() - m_topList->item(m_topList->count() - 1)->sizeHint().width(),m_topList->height())));
        resourceXialaMenu_->show();
    }else{
        m_centerW->setCurrentIndex(index);
    }
}
