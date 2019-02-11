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
#include "UI/mainpage.h"
#include "UI/targetsearch.h"
#include "UI/videoplayback.h"
#include "UI/realtimemonitoring.h"
#include "UI/eventsearch.h"
#include "UI/multiplesearch.h"
#include "UI/dateanalysis.h"
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
    appNameL_ = new QLabel(qApp->applicationName());
    appNameL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    topBorderLine_->hide();
    topBorderLine_->setFixedHeight(2);
    topWgt_ = new QWidget;
    m_centerW = new QStackedWidget;
    m_topList = new QListWidget;
    m_topList->setFocusPolicy(Qt::NoFocus);
    m_topList->setFlow(QListWidget::LeftToRight);

//    m_centerW->addWidget(new MainPage(wm));
    m_centerW->addWidget(new RealtimeMonitoring(wm));
    m_centerW->addWidget(new EventSearch(wm));
    m_centerW->addWidget(new TargetSearch(wm));
    m_centerW->addWidget(new DateAnalysis(wm));
//    m_centerW->addWidget(new VideoPlayback(wm));

    for(int i = 0; i < m_centerW->count(); i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(m_centerW->widget(i)->objectName());
        item->setTextAlignment(Qt::AlignCenter);
        m_topList->addItem(item);
    }

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(logoLabel_,1);
    hlay->addWidget(appNameL_,1);
    hlay->addStretch(1);
    hlay->addWidget(m_topList,10);
    hlay->setSpacing(15);
    hlay->setContentsMargins(15, 0, 0, 0);
    topWgt_->setLayout(hlay);
    topWgt_->setMaximumHeight(80);
    mainLay->addWidget(topWgt_,1);
    mainLay->addWidget(topBorderLine_);
    mainLay->addWidget(m_centerW,16);
    mainLay->setMargin(0);
    mainLay->setSpacing(0);
    setLayout(mainLay);
    m_topList->setCurrentRow(0);
    connect(m_topList,SIGNAL(currentRowChanged(int)),m_centerW,SLOT(setCurrentIndex(int)));


    QFont f = font();
    f.setFamily("Arial"); //DINCond-Bold、PingFang SC Regular、微软雅黑 Microsoft YaHei UI
    setFont(f);

    f = m_topList->font();
    f.setFamily(font().family());
    f.setPixelSize(25);
    m_topList->setFont(f);

    f = appNameL_->font();
    f.setBold(true);
    f.setPixelSize(28);
    appNameL_->setFont(f);
}

MainWindow::~MainWindow()
{

}

void MainWindow::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(s == WidgetManagerI::Danyahei){
        pal = topWgt_->palette();
        pal.setColor(QPalette::Background,QColor(59,71,79));
        topWgt_->setPalette(pal);
        topWgt_->setAutoFillBackground(true);

        pal = topBorderLine_->palette();
        topBorderLine_->setLineWidth(topBorderLine_->height());
        topBorderLine_->setFrameStyle(QFrame::HLine | QFrame::Plain);
        pal.setColor(QPalette::Foreground,QColor(8,66,131));
        topBorderLine_->setPalette(pal);

        pal = appNameL_->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255,150));
        setPalette(pal);

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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
#ifdef FULLTOP
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        item->setSizeHint(QSize((m_topList->width() - m_topList->frameWidth() * 2) / m_topList->count(),m_topList->height() - m_topList->frameWidth() * 2));
    }
#else
    QFontMetrics fs(m_topList->font());
    int maxTabWidth = 0;
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        int itemWidth = fs.width(item->text());
        if(itemWidth > maxTabWidth){
            maxTabWidth = itemWidth;
        }
    }
    for(int i = 0; i < m_topList->count(); i++){
        QListWidgetItem *item = m_topList->item(i);
        item->setSizeHint(QSize(maxTabWidth + 10,m_topList->height() - 2 * m_topList->frameWidth() - 2 * m_topList->spacing()));
    }
#endif
}
