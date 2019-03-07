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

    ResourceManagePage *resouPage = new ResourceManagePage;
    m_centerW->addWidget(new GolbalViewWidget());
    m_centerW->addWidget(new HomPage());
    m_centerW->addWidget(new RealtimeMonitoring());
    m_centerW->addWidget(new EventSearch());
    m_centerW->addWidget(new TargetSearch());
    m_centerW->addWidget(new ReportPage());
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
        item->setData(Qt::UserRole,QPixmap::fromImage(iconDefaultImage));
        item->setData(Qt::UserRole + 1,QPixmap::fromImage(iconSelectedImage));
        item->setIcon(QPixmap::fromImage(iconDefaultImage));
        item->setTextAlignment(Qt::AlignLeft + Qt::AlignVCenter);
        item->setSizeHint(QSize(topListfs.width(item->text()) + 40 + m_topList->iconSize().width(),80));
        viewW += item->sizeHint().width();
        m_topList->addItem(item);
    }
    resourceXialaMenu_->setMinimumWidth(m_topList->item(m_topList->count() - 1)->sizeHint().width());
    m_topList->setFixedWidth(viewW + (m_topList->count() + 1) * m_topList->spacing() + 2 * m_topList->frameWidth());
    QAction *ac = resourceXialaMenu_->addAction(tr("Devices"),[this,resouPage,ac]{
        m_centerW->setCurrentIndex(5);
        resouPage->loadWebPage(0);
    });
    ac->setData(m_centerW->count() - 1);
    ac = resourceXialaMenu_->addAction(tr("Persons"),[this,resouPage,ac]{
        m_centerW->setCurrentIndex(5);
        resouPage->loadWebPage(1);
    });
    ac->setData(m_centerW->count() - 1);

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(logoLabel_);
    hlay->addWidget(appNameL_);
    hlay->addStretch();
    hlay->addWidget(m_topList);
    hlay->setContentsMargins(15, 0, 0, 0);
    topWgt_->setLayout(hlay);
    topWgt_->setMaximumHeight(80);
    mainLay->addWidget(topWgt_,1);
    mainLay->addWidget(topBorderLine_);
    mainLay->addWidget(m_centerW,16);
    mainLay->setMargin(0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    connect(m_topList,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotItemClicked(QListWidgetItem*)));
    connect(m_topList,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(slotCurentItemChanged(QListWidgetItem*,QListWidgetItem*)));
    m_topList->itemClicked(m_topList->item(0));
    m_topList->setCurrentRow(0);
    m_topList->currentItemChanged(m_topList->item(0),nullptr);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUserStyle(int s)
{
    QPalette pal;
    QFont f = font();
    f.setFamily("Arial"); //DINCond-Bold、PingFang SC Regular、微软雅黑 Microsoft YaHei UI
    setFont(f);

    f = m_topList->font();
    f.setFamily(font().family());
    f.setPixelSize(25);
    m_topList->setFont(f);

    f = appNameL_->font();
    f.setBold(true);
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

        pal = topWgt_->palette();
        pal.setColor(QPalette::Background,QColor(48,54,68));
        topWgt_->setPalette(pal);
        topWgt_->setAutoFillBackground(true);

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
                                 "color: rgb(126,140,177);"
                                 "}"
                                 "QListWidget::item:selected{"
                                 "color: white;"
                                 "background-color: rgb(71,65,242);"
                                 "}");
        resourceXialaMenu_->setStyleSheet("QMenu{"
                                          "color: rgb(126,140,177);"
                                          "background-color: rgb(43,49,61);"
                                          "}"
                                          "QMenu::item{"
                                          "height: 60px;"
                                          "padding-left: 15px;"
                                          "}"
                                          "QMenu::item:selected{"
                                          "color: white;"
                                          "background-color: rgb(71,65,242);"
                                          "}");
    }
}

#ifdef USERESIZE
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
#ifdef FULLTOP
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        item->setSizeHint(QSize((m_topList->width() - m_topList->frameWidth() * 2) / m_topList->count(),m_topList->height() - m_topList->frameWidth() * 2));
    }
#else

    int itemW = m_topList->width() * 0.75 / m_topList->count();
    resourceXialaMenu_->setMinimumWidth(itemW);
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        item->setSizeHint(QSize(itemW,m_topList->height() - 2 * m_topList->frameWidth() - 2 * m_topList->spacing()));
        qDebug() << item->sizeHint().height() << "ttttttttttttttttttttttttttttttttttt";
    }
    qDebug() << m_topList->height() << "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee";
#endif
}
#endif

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    QWidgetList wlist = QApplication::allWidgets();
    for(QWidget *wid : wlist){
        QDynamicPropertyChangeEvent *ev = new QDynamicPropertyChangeEvent("danyahei");
        QApplication::postEvent(dynamic_cast<QObject*>(wid),ev);
    }
}

void MainWindow::slotItemClicked(QListWidgetItem *item)
{
    int index = m_topList->row(item);
    if(index == 5){
        resourceXialaMenu_->move(m_topList->mapToGlobal(QPoint(m_topList->width() - m_topList->item(m_topList->count() - 1)->sizeHint().width(),m_topList->height())));
        resourceXialaMenu_->show();
    }else{
        m_centerW->setCurrentIndex(index);
    }
}

void MainWindow::slotCurentItemChanged(QListWidgetItem *cur, QListWidgetItem *pre)
{
    cur->setIcon(cur->data(Qt::UserRole + 1).value<QPixmap>());
    if(pre){
        pre->setIcon(pre->data(Qt::UserRole).value<QPixmap>());
    }
}
