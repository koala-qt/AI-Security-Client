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
#include <QDesktopWidget>
#include "UI/mainpage.h"
#include "UI/targetsearch.h"
#include "UI/videoplayback.h"
#include "UI/realtimemonitoring.h"
#include "UI/eventsearch.h"
#include "UI/multiplesearch.h"
#include "UI/facelinkpage.h"
#include "UI/trackingpage.h"
#include <QDebug>

#pragma execution_character_set("utf-8")
MainWindow::MainWindow(WidgetManagerI *wm, WidgetI *parent)
    : WidgetI(wm,parent)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    logoLabel_ = new QLabel;
    logoLabel_->setPixmap(QPixmap("images/appLogo.png"));
//    logoLabel_->setScaledContents(true);
    topBorderLine_ = new QLabel;
    topBorderLine_->hide();
    topBorderLine_->setFixedHeight(2);
    topWgt_ = new QWidget;
    m_centerW = new QStackedWidget;
    m_topList = new QListWidget;
    m_topList->setFixedHeight(60);
    m_topList->setFocusPolicy(Qt::NoFocus);
    m_topList->setFlow(QListWidget::LeftToRight);

//    m_centerW->addWidget(new MainPage(wm));
    m_centerW->addWidget(new RealtimeMonitoring(wm));
    m_centerW->addWidget(new EventSearch(wm));
    m_centerW->addWidget(new TrackingPage(wm));
    m_centerW->addWidget(new FaceLinkPage(wm));
    m_centerW->addWidget(new TargetSearch(wm));
    m_centerW->addWidget(new MultipleSearch(wm));
//    m_centerW->addWidget(new VideoPlayback(wm));

    for(int i = 0; i < m_centerW->count(); i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(m_centerW->widget(i)->objectName());
        item->setTextAlignment(Qt::AlignCenter);
        item->setSizeHint(QSize(220,m_topList->height() - 2 * m_topList->frameWidth()));
        m_topList->addItem(item);
    }

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(logoLabel_);
    hlay->addWidget(m_topList);
    hlay->setSpacing(15);
    hlay->setContentsMargins(15, 0, 0, 0);
    topWgt_->setMaximumHeight(60);
    topWgt_->setLayout(hlay);
    mainLay->addWidget(topWgt_);
    mainLay->addWidget(topBorderLine_);
    mainLay->addWidget(m_centerW);
    mainLay->setMargin(0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    m_topList->setCurrentRow(0);
    connect(m_topList,SIGNAL(currentRowChanged(int)),m_centerW,SLOT(setCurrentIndex(int)));
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QFont f;
    QPalette pal;
    if(s == WidgetManagerI::Danyahei){
        f = font();
        f.setFamily("微软雅黑"); //DINCond-Bold、PingFang SC Regular、微软雅黑
        setFont(f);

        topWgt_->setStyleSheet("QWidget{"
                               "background-color: rgb(59,71,79);"
                               "}");

        pal = topBorderLine_->palette();
        topBorderLine_->setLineWidth(topBorderLine_->height());
        topBorderLine_->setFrameStyle(QFrame::HLine | QFrame::Plain);
        pal.setColor(QPalette::Foreground,QColor(8,66,131));
        topBorderLine_->setPalette(pal);

        f = m_topList->font();
        f.setPixelSize(20);
        m_topList->setFont(f);
        m_topList->setFrameStyle(QFrame::NoFrame);
        m_topList->setStyleSheet("QListWidget{"
                                 "background-color: transparent;"
                                 "}"
                                 "QListWidget::item{"
                                 "color: rgba(206,206,206,0.8);"
                                 "}"
                                 "QListWidget::item:selected{"
                                 "color:rgba(255,255,255,0.8);"
                                 "border-image: url(images/item_selected.png);;"
                                 "}");
    }
}

#ifdef FULLTOP
void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        item->setSizeHint(QSize((m_topList->width() - m_topList->frameWidth() * 2) / m_topList->count(),m_topList->height() - m_topList->frameWidth() * 2));
    }
}
#endif
