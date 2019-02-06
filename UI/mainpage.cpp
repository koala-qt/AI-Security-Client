#include <random>
#include <QChartView>
#include <QPieSeries>
#include <QPieSlice>
#include <QAreaSeries>
#include <QLineSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QLabel>
#include <QComboBox>
#include <QListWidget>
#include <QLinearGradient>
#include <QResizeEvent>
#include <QScrollBar>
#include <QLegendMarker>
#include <QValueAxis>
#include <QDebug>
#include <QDesktopWidget>
#include <QGraphicsBlurEffect>
#include <QMenu>
#include <QCursor>
#include <QMessageBox>
#include "mainpage.h"
#include "rectnumberwidget.h"
#include "radarchart.h"
#include "eventcalender.h"
#include "platback.h"
#include "movielabel.h"
#include "waitinglabel.h"
#include "calendarecharts.h"
#include "service/restservice.h"
#include "sceneimagedialog.h"
#include "facesearch.h"

#pragma execution_character_set("utf-8")
MainPage::MainPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("首页"));
    backImage_.load("images/app-background.png");
    QHBoxLayout *mainLay = new QHBoxLayout;
    QVBoxLayout *mainVLay = new QVBoxLayout;
    //创建事件统计情况
    eventStatisticsBackW_ = new QWidget;
    QVBoxLayout *vlay = new QVBoxLayout;
    eventStaticsticsTitleLabel_ = new QLabel(tr("事件统计情况"));
    eventStaticsticsTitleLabel_->setFixedHeight(35);
    eventStaticsticsTitleLabel_->setAlignment(Qt::AlignHCenter);
    vlay->addWidget(eventStaticsticsTitleLabel_);
    QHBoxLayout *hlay = new QHBoxLayout;
    eventStaticsLabel_ = new RectNumberWidget;
    eventStaticsLabel_->setTitle(tr("事件统计"),Qt::AlignBottom);
    eventStaticsLabel_->setValue("0");
    hlay->addWidget(eventStaticsLabel_);
    importEventLabel_ = new RectNumberWidget;
    importEventLabel_->setTitle(tr("布控事件"),Qt::AlignBottom);
    importEventLabel_->setValue("0");
    hlay->addWidget(importEventLabel_);
    hlay->setMargin(0);
    hlay->setSpacing(25);
    vlay->addLayout(hlay);
    vlay->setMargin(30);
    eventStatisticsBackW_->setLayout(vlay);
    eventStatisticsBackW_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    mainVLay->addWidget(eventStatisticsBackW_,258);
    //创建事件分布
    eventDistribution_ = new RadarChart(RadarChart::Spider);
    eventDistribution_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    eventDistribution_->setTitle(tr("事件分布情况"),Qt::AlignHCenter);
    mainVLay->addWidget(eventDistribution_,318);
    //创建事件日历
//    barChartView_ = new QChartView;
//    barChartView_->setRenderHint(QPainter::Antialiasing);
//    QBarSeries *barSeries = new QBarSeries;
//    barSeries->append(new QBarSet(tr("攀爬报警")));
//    barSeries->append(new QBarSet(tr("遮挡报警")));
//    barSeries->append(new QBarSet(tr()))
    eventCalenderBack_ = new QWidget;
    vlay = new QVBoxLayout;
    eventCalenderLabel_ = new QLabel(tr("事件日历"));
    eventCalenderLabel_->setFixedHeight(35);
    eventCalenderLabel_->setAlignment(Qt::AlignHCenter);
#ifdef USEQCALENDAR
    eventCalender_ = new QCalendarWidget;
    eventCalender_->setFocusPolicy(Qt::NoFocus);
//    eventCalender_->setMaxValue(500);
    eventCalender_->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
#else
    eventCalender_ = new CalendarEcharts(this);
#endif
    vlay->addWidget(eventCalenderLabel_);
    vlay->addWidget(eventCalender_);
    vlay->setMargin(30);
    eventCalenderBack_->setLayout(vlay);
    mainVLay->addWidget(eventCalenderBack_,363);
    mainVLay->setSpacing(21);
    mainVLay->setMargin(0);
    mainLay->addLayout(mainVLay,459);
    //创建中心地图
    QHBoxLayout *centerMainLay = new QHBoxLayout;
    mapCenterBack_ = new PlatBack;
//    mapCenterBack_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    mapCenterBack_->loadImage(QImage("images/plat.png"),QImage("images/shiedld.png"),QImage("images/shadow.png"));
    //mapCenterBack_->setStyleSheet("background-color:red;");
    centerTitleL_ = new QLabel;
    centerTitleL_->setAlignment(Qt::AlignCenter);
    centerTitleL_->setPixmap(QPixmap("images/title.png"));
    abDoorAlarmLabel_ = new QLabel(tr("AB门闯入告警"));
    abDoorAlarmValueL_ = new QLabel("0");
    intrdeAlarmLabel_ = new QLabel(tr("重点闯入告警"));
    intrudeAlarmValueL_ = new QLabel("0");
    blackAlarmLabel = new QLabel(tr("黑名单告警"));
    blackAlarmValueL_ = new QLabel("0");
    hlay = new QHBoxLayout;
    vlay = new QVBoxLayout;
    vlay->addWidget(abDoorAlarmLabel_);
    vlay->addWidget(abDoorAlarmValueL_);
    gaojingBackW_ = new QWidget;
    gaojingBackW_->setLayout(vlay);
    hlay->addWidget(gaojingBackW_);
    vlay = new QVBoxLayout;
    vlay->addWidget(intrdeAlarmLabel_);
    vlay->addWidget(intrudeAlarmValueL_);
    handedBackW_ = new QWidget;
    handedBackW_->setLayout(vlay);
    hlay->addWidget(handedBackW_);
    vlay = new QVBoxLayout;
    vlay->addWidget(blackAlarmLabel);
    vlay->addWidget(blackAlarmValueL_);
    nonHandedBackW_ = new QWidget;
    //nonHandedBackW_->setStyleSheet("background-color:green;");
    nonHandedBackW_->setLayout(vlay);
    hlay->addWidget(nonHandedBackW_);
    QMargins m = hlay->contentsMargins();
    m.setLeft(20);
    m.setRight(35);
    hlay->setContentsMargins(m);
    hlay->setSpacing(7);
    vlay = new QVBoxLayout;
    //vlay->addWidget(centerTitleL_,111);
    vlay->addStretch(674);
    vlay->setMargin(0);
    vlay->setSpacing(0);
    vlay->addLayout(hlay,144);
    centerMainLay->addLayout(vlay,720);
    //创建事件列表
    eventCobox_ = new QComboBox;
    eventCobox_->setIconSize(QSize(1,30));
    QPixmap pix(1,eventCobox_->iconSize().height());
    pix.fill(Qt::transparent);
    eventCobox_->addItem(pix,tr("显示所有事件"));
    eventCobox_->addItem(pix,tr("显示黑名单事件"));
    eventCobox_->addItem(pix,tr("显示闯入事件"));
    eventCobox_->addItem(pix,tr("显示AB门事件"));
    hlay = new QHBoxLayout;
    hlay->addWidget(eventCobox_);
    hlay->setContentsMargins(10,10,10,0);
    eventList_ = new QListWidget;
    // 加载英文翻译后,大小不对
    QDesktopWidget *desktopWidget = QApplication::desktop();
    //获取可用桌面大小
    QRect deskRect = desktopWidget->availableGeometry();
    eventList_->setFixedWidth(deskRect.width() / 1920 * m_nListWgtWidth);
    eventList_->setFixedHeight(deskRect.height() / 1040 * m_nListWgtHeight);

    eventList_->setViewMode(QListWidget::IconMode);
    eventList_->horizontalScrollBar()->setVisible(false);
    eventList_->verticalScrollBar()->setVisible(false);
    eventList_->setSpacing(10);
    eventList_->setMovement(QListView::Static);
    vlay = new QVBoxLayout;
    vlay->addLayout(hlay);
    vlay->addWidget(eventList_);
    vlay->addStretch();
    vlay->setMargin(0);
    vlay->setSpacing(0);
    eventListBack_ = new QWidget;
    eventListBack_->setLayout(vlay);
    vlay = new QVBoxLayout;
    //vlay->addStretch(1);
    vlay->addWidget(eventListBack_,9);
    vlay->setMargin(0);
    vlay->setSpacing(0);
    centerMainLay->addLayout(vlay,340); // 245
    centerMainLay->setSpacing(0);
    m = centerMainLay->contentsMargins();
    m.setRight(10);
    centerMainLay->setContentsMargins(m);
    mapCenterBack_->setLayout(centerMainLay);
    mainLay->addWidget(mapCenterBack_,944);
    //创建饼图
//    QStringList pieAxies_;
//    pieAxies_ << tr("窗口一") << tr("窗口二") << tr("窗口三") << tr("窗口四") << tr("窗口五");
//    QVector<double> values{234,23,345,209,199};
    mainVLay = new QVBoxLayout;
    pieChartView_ = new QChartView;
    QPieSeries *pieSeries = new QPieSeries;
    pieSeries->setHoleSize(0.35);
//    int i = 0;
//    foreach (const QString &str, pieAxies_) {
//        QPieSlice *slice = pieSeries->append(str,values.at(i));
//        slice->setLabelVisible();
//        i++;
//    }
    pieChartView_->setRenderHint(QPainter::Antialiasing);
    pieChartView_->chart()->addSeries(pieSeries);
    pieChartView_->chart()->setTitle(tr("最近5分钟服务时间占比分析"));
    pieChartView_->chart()->setTitleBrush(Qt::white);
    pieChartView_->chart()->setBackgroundVisible(false);
    pieChartView_->chart()->setAnimationOptions(QChart::AllAnimations);
    mainVLay->addWidget(pieChartView_,258);
    //创建抓拍人员统计
    vlay = new QVBoxLayout;
    grabPersonStatisticsBack_ = new QWidget;
    grabPersonStaticsLabel_ = new QLabel(tr("抓拍人员统计"));
    grabPersonStaticsLabel_->setAlignment(Qt::AlignCenter);
    vlay->addWidget(grabPersonStaticsLabel_,38);
    QGridLayout *gridLay = new QGridLayout;
    grabePersonSumLabel_ = new QLabel(tr("抓拍总人数"));
    grabPersonSumValueLabel_ = new QLabel;
    grabPersonSumValueLabel_->setNum(0);
    registerPersonLabel_ = new QLabel(tr("已受理人数"));
    registerPersonValueLabel_ = new QLabel;
    registerPersonValueLabel_->setNum(0);
    accessPersonLabel_ = new QLabel(tr("布控总人数"));
    accessPersonValueLabel_ = new QLabel;
    accessPersonValueLabel_->setNum(0);
    controlPersonLabel_ = new QLabel(tr("布控人员"));
    controlPersonValueLabel_ = new QLabel;
    controlPersonValueLabel_->setNum(0);
    gridLay->addWidget(grabePersonSumLabel_,0,0,1,1,Qt::AlignHCenter);
    gridLay->addWidget(registerPersonLabel_,0,1,1,1,Qt::AlignHCenter);
    gridLay->addWidget(grabPersonSumValueLabel_,1,0,1,1,Qt::AlignHCenter);
    gridLay->addWidget(registerPersonValueLabel_,1,1,1,1,Qt::AlignHCenter);
    gridLay->addWidget(accessPersonLabel_,2,0,1,1,Qt::AlignHCenter);
    gridLay->addWidget(controlPersonLabel_,2,1,1,1,Qt::AlignHCenter);
    gridLay->addWidget(accessPersonValueLabel_,3,0,1,1,Qt::AlignHCenter);
    gridLay->addWidget(controlPersonValueLabel_,3,1,1,1,Qt::AlignHCenter);
    vlay->setAlignment(Qt::AlignVCenter);
    vlay->addLayout(gridLay,130);
    grabPersonStatisticsBack_->setLayout(vlay);
    grabPersonStatisticsBack_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Expanding);
    mainVLay->addWidget(grabPersonStatisticsBack_,318);
    //创建区域图
    areaChartView_ = new QChartView;
    areaChartView_->setRenderHint(QPainter::Antialiasing);
    QLineSeries *lineSeries = new QLineSeries;
    for(int i = 0; i < 1440; i++){
        lineSeries->append(i,0);
    }
    QAreaSeries *areaSeries = new QAreaSeries;
    areaSeries->setUpperSeries(lineSeries);
    areaChartView_->chart()->addSeries(areaSeries);
    areaChartView_->chart()->createDefaultAxes();
    QBarCategoryAxis *axies = new QBarCategoryAxis;
    axies->append(QStringList() << "00" << "02" << "04" << "06" << "08" << "10"
                                << "12" << "14" << "16" << "18" << "20" << "22" << "24");
    areaChartView_->chart()->setAxisX(axies);
    areaChartView_->chart()->legend()->setVisible(false);
    areaChartView_->chart()->setTitle(tr("人流高峰统计"));
    areaChartView_->chart()->setTitleBrush(Qt::white);
    //QFont tempFont;
    //tempFont.setFamily(QString(tr("Arial"))); //TFArrowLight无效果 设置字体效果不明显
    //tempFont.setBold(false);
    //tempFont.setPixelSize(54);
    //areaChartView_->chart()->setFont(tempFont);
    //tempFont.setBold(false);
    QFont tempFont2;
    tempFont2.setFamily(QString(tr("TFArrowLight")));
    tempFont2.setPixelSize(27);
    areaChartView_->chart()->setTitleFont(tempFont2);
    areaChartView_->chart()->axisY()->setMin(0);
    areaChartView_->chart()->axisY()->setMax(100);
    areaChartView_->chart()->setBackgroundVisible(false);
    areaChartView_->chart()->axisX()->setGridLineVisible(false);
    areaChartView_->chart()->axisY()->setGridLineVisible(false);
    areaChartView_->chart()->setAnimationOptions(QChart::AllAnimations);
    mainVLay->addWidget(areaChartView_,363);
    mainVLay->setSpacing(10);
    mainLay->setSpacing(0);
    m = mainLay->contentsMargins();
    m.setLeft(10);
    m.setRight(10);
    m.setTop(30);
    m.setBottom(21);
    mainLay->setContentsMargins(m);
    mainLay->addLayout(mainVLay,459);
    setLayout(mainLay);

    listMenu_ = new QMenu(eventList_);
    listMenu_->addAction(tr("Scene analysis"),[this]{
            SceneImageDialog dialog;
            dialog.setUserStyle(widgetManger()->currentStyle());
            dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            dialog.setImage(eventList_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
            dialog.setRectLinePen(Qt::yellow);
            connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
                if(!images.count()){
                    return;
                }
                FaceSearch *faceDialog = new FaceSearch(widgetManger());
                faceDialog->setAttribute(Qt::WA_DeleteOnClose);
                faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
                faceDialog->setWindowModality(Qt::ApplicationModal);
                QPalette pal = faceDialog->palette();
                pal.setColor(QPalette::Background,QColor(112,110,119));
                faceDialog->setPalette(pal);
                faceDialog->setAutoFillBackground(true);
                faceDialog->setUserStyle(widgetManger()->currentStyle());
                faceDialog->layout()->setMargin(10);
                faceDialog->setFaceImage(images.first());
                faceDialog->setMinimumHeight(700);
                faceDialog->show();
            });
            dialog.exec();
    });
    eventList_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(eventList_,&QListWidget::customContextMenuRequested,this,[this](const QPoint& p){
        if(!eventList_->itemAt(p))return;
        listMenu_->move(QCursor::pos());
        listMenu_->show();
    });

    notifyService_ = dynamic_cast<NotifyServiceI*>(getWoker("NotifyService"));
    connect(notifyService_,SIGNAL(sigAreaGarphics(QVector<QPointF>)),this,SLOT(slotAreaGraphics(QVector<QPointF>)));
    connect(notifyService_,SIGNAL(sigAlarmData(int,int,int)),this,SLOT(slotAlarmData(int,int,int)));
    connect(notifyService_,SIGNAL(sigFaceGrab(int,int,int,int)),this,SLOT(slotFaceGrab(int,int,int,int)));
    connect(notifyService_,SIGNAL(sigTotalEvent(int,int)),this,SLOT(slotTotalEventStatics(int,int)));
    connect(notifyService_,SIGNAL(sigPieChart(QVector<kf::PieCharData>)),this,SLOT(slotPieChart(QVector<kf::PieCharData>)));
    connect(notifyService_,SIGNAL(sigEventSpider(QVector<QVector<double> >)),this,SLOT(slotSpiderData(QVector<QVector<double> >)));
    connect(eventCobox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotEventComboxClicked(int)));
    slotEventComboxClicked(0);
    getCameraInfo();
}

void MainPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    QFont f;
    if(s == WidgetManagerI::Danyahei){
        f = eventStaticsticsTitleLabel_->font();
        f.setPixelSize(27);
        //f.setFamily("TFArrowLight");
        f.setFamily("TFArrowLight");
        eventStaticsticsTitleLabel_->setFont(f);
        eventStaticsLabel_->setAllPen(QPen(QColor(191,192,194),1),QPen(QColor(191,192,194),3),QPen(QColor(191,192,194)),QPen(QColor(206,206,206)));
        f.setPixelSize(14);
        f.setFamily(QString(tr("Arial")));
        QFont valueF = f;
        valueF.setPixelSize(76.8);
        valueF.setFamily("DINCond-Bold"); //DINCond-Bold、Arial
        eventStaticsLabel_->setAllFont(valueF,f);
        importEventLabel_->setAllPen(eventStaticsLabel_->borderPen(),eventStaticsLabel_->pointPen(),eventStaticsLabel_->valuePen(),eventStaticsLabel_->titlePen());
        importEventLabel_->setAllFont(eventStaticsLabel_->valueFont(),eventStaticsLabel_->titleFont());
        eventStatisticsBackW_->setStyleSheet("QWidget{"
                                             "border-image:url(images/Event statistics.png);"
                                             "border-radius: 10px;"
                                             "}");
        eventStaticsticsTitleLabel_->setStyleSheet("QLabel{"
                                                   "background-color: transparent;"
                                                   "border-image: none;"
                                                   "color: white;"
                                                   "}");

        pal = eventList_->palette();
        pal.setColor(QPalette::Base,Qt::transparent);
        eventList_->setPalette(pal);
        eventList_->setFrameStyle(QFrame::NoFrame);
        QFont axiesXFont = f,axiesYFont = f, valueLabelFont = f,stateLabelFont = f;
        f.setPixelSize(27);
        f.setFamily("TFArrowLight");
        axiesXFont.setPixelSize(10);
        axiesYFont.setPixelSize(10);
        valueLabelFont.setPixelSize(16.8);
        stateLabelFont.setPixelSize(18);
//        eventDistribution_->setLayoutDirection(Qt::RightToLeft);
        eventDistribution_->setAllFont(f,axiesXFont,axiesYFont,valueLabelFont,stateLabelFont);
        eventDistribution_->setAllPen(QPen(Qt::white),QPen(QColor(191,192,194)),QPen(QColor(255,255,255)),QPen(QColor(191,192,194)),QPen(QColor(206,206,206)),QPen(QColor(206,206,206)));
        eventDistribution_->setBackground(QImage("images/Event distribution.png"),10);
        eventDistribution_->setYxiesRange(0,10000,2000);
        eventDistribution_->setState(tr(""));
        eventDistribution_->setXAxiesLabelVisible(false);

        f = eventCalenderLabel_->font();
        f.setPixelSize(27);
        //f.setFamily("TFArrowLight");
        f.setFamily("TFArrowLight");
        eventCalenderLabel_->setFont(f);
        eventCalenderBack_->setStyleSheet("QWidget{"
                                             "border-image:url(images/Event calendar.png);"
                                             "border-radius: 10px;"
                                             "}");
        eventCalenderLabel_->setStyleSheet("QLabel{"
                                           "color: white;"
                                           "border-image: none;"
                                           "background-color:transparent;"
                                           "}");

#ifdef USEQCALENDAR
        eventCalender_->setStyleSheet("QCalendarWidget QWidget#qt_calendar_navigationbar{"
                                      "background-color: transparent; "
                                      "min-height:26px;"
                                      "max-height:26px;"
                                      "}"
                                      "QCalendarWidget QTableView{"
                                      "selection-background-color: rgb(37,80,100);"
                                      "alternate-background-color: rgb(128, 128, 128);"
                                      "background-color: #2F2F3E;"
                                      "}"
                                      "QToolButton#qt_calendar_monthbutton,#qt_calendar_yearbutton{"
                                      "color: #9ea5a9;"
                                      "border-radius: none;"
                                      "}");
//        eventCalender_->setEventBackColor(QColor(206,206,206));
        eventCalender_->setGridVisible(true);
#else
#endif

        pieChartView_->setStyleSheet("QWidget{"
                                     "border-image:url(images/Check-in time length.png);"
                                     "border-radius: 10px;"
                                     "}");
        pieChartView_->chart()->legend()->setAlignment(Qt::AlignLeft);
        pieChartView_->chart()->legend()->setLabelColor(QColor(206,206,206));
        f = font();
        f.setPixelSize(27);
        f.setFamily("TFArrowLight");
        pieChartView_->chart()->setTitleFont(f);

        grabPersonStatisticsBack_->setStyleSheet(".QWidget{"
                                             "border-image:url(images/Capture statistics.png);"
                                             "border-radius: 10px;"
                                             "}");
        grabPersonStaticsLabel_->setStyleSheet("QLabel{"
                                               "font-family: TFArrowLight;"
                                               "color: white;"
                                               "border-image: none;"
                                               "background-color:transparent;"
                                               "font-size: 27px;"
                                               "}");
        grabePersonSumLabel_->setStyleSheet("QLabel{"
                                            "color: rgb(206,206,206);"
                                            "border-image: none;"
                                            "font-family: Arial;"
                                            "font-size: 12px;"
                                            "}");
        registerPersonLabel_->setStyleSheet("QLabel{"
                                            "color: rgb(206,206,206);"
                                            "border-image: none;"
                                            "font-family: Arial;"
                                            "font-size: 12px;"
                                            "}");
        accessPersonLabel_->setStyleSheet("QLabel{"
                                          "color: rgb(206,206,206);"
                                          "border-image: none;"
                                          "font-family: Arial;"
                                          "font-size: 12px;"
                                          "}");
        controlPersonLabel_->setStyleSheet("QLabel{"
                                           "color: rgb(206,206,206);"
                                           "border-image: none;"
                                           "font-family: Arial;"
                                           "font-size: 12px;"
                                           "}");
        grabPersonSumValueLabel_->setStyleSheet("QLabel{"
                                                "color: rgb(206,206,206);"
                                                "border-image: none;"
                                                "font-family: DINCond-Bold;"
                                                "font-size: 63px;"
                                                "}");
        registerPersonValueLabel_->setStyleSheet("QLabel{"
                                                 "color: rgb(206,206,206);"
                                                 "border-image: none;"
                                                 "font-family: DINCond-Bold;"
                                                 "font-size: 63px;"
                                                 "}");
        accessPersonValueLabel_->setStyleSheet("QLabel{"
                                               "color: rgb(206,206,206);"
                                               "border-image: none;"
                                               "font-family: DINCond-Bold;"
                                               "font-size: 63px;"
                                               "}");
        controlPersonValueLabel_->setStyleSheet("QLabel{"
                                                "color: rgb(180,160,108);"
                                                "border-image: none;"
                                                "font-family: DINCond-Bold;"
                                                "font-size: 63px;"
                                                "}");

        areaChartView_->setStyleSheet("QWidget{"
                                      "border-image:url(images/Human traffic statistics.png);"
                                      "font-family:TFArrowLight"
                                      "}");
        areaChartView_->chart()->axisX()->setLabelsColor(QColor(206,206,206));
        areaChartView_->chart()->axisY()->setLabelsColor(QColor(206,206,206));
        f = font();
        f.setPixelSize(27);
        f.setFamily("TFArrowLight");
        //areaChartView_->chart()->setTitleFont(f);
        QAreaSeries *areaSeries = reinterpret_cast<QAreaSeries*>(areaChartView_->chart()->series().first());
        areaSeries->setPen(QPen(QColor(30,112,142),1));
        QLinearGradient gradient;
        gradient.setStart(0,0);
        gradient.setFinalStop(480,0);
        gradient.setColorAt(0,Qt::transparent);
        gradient.setColorAt(1,QColor(48,236,213));
        areaSeries->setBrush(gradient);

        eventListBack_->setStyleSheet("QWidget{"
                                             "border-image:url(images/All events.png);"
                                             "border-radius: 10px;"
                                             "}");
        eventCobox_->setStyleSheet("QComboBox QAbstractItemView{"
                                   "background-color: rgb(100,100,100);"
                                   "selection-color: white;"
                                   "outline: 0px;"
                                   "border-radius: 0px;"
                                   "selection-background-color: rgb(142,142,142,50);"
                                   "}"
                                   "QComboBox::drop-down{"
                                   "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                                   "}"
                                   "QComboBoxListView{"
                                   "color: white;"
                                   "background-color: transparent;"
                                   "border-image:none;"
                                   "}"
                                   "QComboBox{"
                                   "color: white;"
                                   "font-family:Arial;"
                                   "font-size: 14pt;"
                                   "background-color: rgba(206,206,206,0.15);"
                                   "border-radius: 0px;"
                                   "border-image:none;"
                                   "}");

        gaojingBackW_->setStyleSheet("QWidget{"
                                     "border-image:url(images/Work area intrusion alert.png);"
                                     "border: 1px solid #599AFF;"
                                     "border-radius: 5px;"
                                     "}");
        handedBackW_->setStyleSheet("QWidget{"
                                    "border-image:url(images/VIP zone intrusion alert.png);"
                                    "border: 1px solid #599AFF;"
                                    "border-radius: 5px;"
                                    "}");
        nonHandedBackW_->setStyleSheet("QWidget{"
                                       "border-image:url(images/Blacklist event.png);"
                                       "border: 1px solid #FFC760;"
                                       "border-radius: 5px;"
                                       "}");
        abDoorAlarmLabel_->setStyleSheet("QLabel{"
                                         "color: #CECECE;"
                                         "font-size: 14px;"
                                         "font-family: Arial;"
                                         "border: none;"
                                         "border-image: none;"
                                         "}");
        intrdeAlarmLabel_->setStyleSheet("QLabel{"
                                         "color: #CECECE;"
                                         "font-size: 14px;"
                                         "font-family: Arial;"
                                         "border: none;"
                                         "border-image: none;"
                                         "}");
        blackAlarmLabel->setStyleSheet("QLabel{"
                                       "color:  #B6A26D;"
                                       "font-size: 14px;"
                                       "font-family: Arial;"
                                       "border: none;"
                                       "border-image: none;"
                                       "}");
        abDoorAlarmValueL_->setStyleSheet("QLabel{"
                                          "color: #CECECE;"
                                          "font-size:90px;"
                                          "font-family:DINCond-Bold;"
                                          "border: none;"
                                          "border-image: none;"
                                          "}");
        intrudeAlarmValueL_->setStyleSheet("QLabel{"
                                           "color: #CECECE;"
                                           "font-size:90px;"
                                           "font-family:DINCond-Bold;"
                                           "border: none;"
                                           "border-image: none;"
                                           "}");
        blackAlarmValueL_->setStyleSheet("QLabel{"
                                         "color:  #B6A26D;"
                                         "font-size:90px;"
                                         "font-family:DINCond-Bold;"
                                         "border: none;"
                                         "border-image: none;"
                                         "}");
        listMenu_->setStyleSheet("QMenu{"
                                 "color: white;"
                                 "border-image:none;"
                                 "background-color: rgb(75,75,75);"
                                 "}"
                                 "QMenu::item:selected{"
                                 "background-color: rgba(255,255,255,0.4);"
                                 "}");
    }
}

void MainPage::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    int imgItemH = (eventList_->height() - (MAINPAGEEVENTITEMCOUNT + 1) * eventList_->spacing() - eventList_->frameWidth() * 2) / MAINPAGEEVENTITEMCOUNT;
    int imgItemW = eventList_->width() - 2 * eventList_->spacing() - 2 * eventList_->frameWidth();
    imgItemSize_.setHeight(imgItemH);
    imgItemSize_.setWidth(imgItemW);
    eventList_->setIconSize(imgItemSize_);
    for(int i = 0; i < eventList_->count(); i++){
        QListWidgetItem *item = eventList_->item(i);
        item->setSizeHint(imgItemSize_);
        item->setIcon(QPixmap::fromImage(item->data(Qt::UserRole + 1).value<QImage>().scaled(imgItemSize_)));
    }
}

void MainPage::mousePressEvent(QMouseEvent *event)
{
    QMap<QDate,int> mapValue;
    QDate curDate = QDate::currentDate();
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(300,800);
    for(int i = 0; i < curDate.day(); i++){
        mapValue.insert(QDate(curDate.year(),curDate.month(),i + 1),dis(gen));
    }
    eventCalender_->updateData(curDate,mapValue);
}

void MainPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawImage(rect(),backImage_);
}

void MainPage::slotAddImgItem(QStringList infoList, QImage img)
{
    if(eventList_->count() >= MAINPAGEEVENTITEMCOUNT){
        delete eventList_->takeItem(MAINPAGEEVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap::fromImage(img).scaled(imgItemSize_));
    item->setData(Qt::UserRole + 1,QVariant::fromValue(img));
    item->setSizeHint(imgItemSize_);
    eventList_->insertItem(0,item);

    if(infoList.count() < 1){
        return;
    }

    if(movieLabelMap_.value(infoList.first())){
        movieLabelMap_.value(infoList.first())->startWaring();
    }
}

void MainPage::slotEventComboxClicked(int index)
{
    if(index == 0){
        connect(notifyService_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
        connect(notifyService_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
        connect(notifyService_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 1){
        notifyService_->disconnect(SIGNAL(sigABDoorAlarmScene(QStringList,QImage)));
        notifyService_->disconnect(SIGNAL(sigIntruderAlarmScene(QStringList,QImage)));
        connect(notifyService_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 2){
        notifyService_->disconnect(SIGNAL(sigABDoorAlarmScene(QStringList,QImage)));
        notifyService_->disconnect(SIGNAL(sigBlackListAlarmScene(QStringList,QImage)));
        connect(notifyService_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 3){
        notifyService_->disconnect(SIGNAL(sigIntruderAlarmScene(QStringList,QImage)));
        notifyService_->disconnect(SIGNAL(sigBlackListAlarmScene(QStringList,QImage)));
        connect(notifyService_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotAddImgItem(QStringList,QImage)),Qt::UniqueConnection);
    }
}

void MainPage::slotAreaGraphics(QVector<QPointF> data)
{
    QLineSeries *series = reinterpret_cast<QLineSeries*>(reinterpret_cast<QAreaSeries*>(areaChartView_->chart()->series().first())->upperSeries());
    qreal maxY = qobject_cast<QValueAxis*>(areaChartView_->chart()->axisY())->max();
    if((*std::max_element(data.begin(),data.end(),[](QPointF &p1,QPointF &p2){return p1.y() < p2.y();})).y() > maxY){
        areaChartView_->chart()->axisY()->setMax(maxY + 100);
    }
    series->replace(data);
}

void MainPage::slotAlarmData(int abWaring, int intrstion, int blackList)
{
    QString abStr,intrStr,blackStr;
    abStr.setNum(abWaring);
    intrStr.setNum(intrstion);
    blackStr.setNum(blackList);
    if(abWaring >= 999999){
        abStr.setNum(abWaring/1000);
        abStr += "...";
    }
    if(intrstion >= 999999){
        intrStr.setNum(abWaring/1000);
        intrStr += "...";
    }
    if(blackList >= 999999){
        blackStr.setNum(abWaring/1000);
        blackStr += "...";
    }
    abDoorAlarmValueL_->setText(abStr);
    intrudeAlarmValueL_->setText(intrStr);
    blackAlarmValueL_->setText(blackStr);
}

void MainPage::slotFaceGrab(int sum, int black, int served, int blackAlarm)
{
    grabPersonSumValueLabel_->setNum(sum);
    registerPersonValueLabel_->setNum(served);
    accessPersonValueLabel_->setNum(black);
    controlPersonValueLabel_->setNum(blackAlarm);
}

void MainPage::slotTotalEventStatics(int sum, int control)
{
    QString sumStr,conStr;
    sumStr.setNum(sum);
    conStr.setNum(control);
    if(sum >= 999999){
        sumStr.setNum(sum / 1000);
        sumStr += "...";
    }
    if(sum >= 999999){
        conStr.setNum(sum / 1000);
        conStr += "...";
    }
    eventStaticsLabel_->setValue(sumStr);
    importEventLabel_->setValue(conStr);
}

void MainPage::slotSpiderData(QVector<QVector<double> > datas)
{
    eventDistribution_->clear();
    QDate curDate = QDate::currentDate();
    double maxValue = 0.0;
    for(int i = 0; i < datas.count(); i++){
        QString dayStr = curDate.addDays(-(datas.count() -1 - i)).toString("MMMM d yy");
        QColor c = spiderColor_.value(dayStr);
        if(!c.isValid()){
            std::random_device device;
            std::mt19937 gen(device());
            std::uniform_int_distribution<int> dis(0,255);
            c = QColor(dis(gen),dis(gen),dis(gen));
            spiderColor_[dayStr] = c;
        }
        c.setAlpha(150);
        eventDistribution_->append(dayStr,c,datas.at(i));
        double v = *std::max_element(datas.at(i).begin(),datas.at(i).end());
        if(v > maxValue){
            maxValue = v;
        }
    }

    int step = ((static_cast<int>(::ceil(maxValue / 5)) / 100) + 1) * 100;
    eventDistribution_->setYxiesRange(0,5 * step,step);
    eventDistribution_->update();
}

void MainPage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> cameraDatas)
{
    QVector<QString> axies;
    std::transform(cameraDatas.begin(),cameraDatas.end(),std::back_inserter(axies),[](RestServiceI::CameraInfo &info){return info.cameraPos;});
    eventDistribution_->setAxiesX(axies);
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<int> dis(0,300);
    for(auto &info : cameraDatas){
        MovieLabel *ml = new MovieLabel(this);
        movieLabelMap_.insert(info.cameraPos,ml);
        ml->setFixedSize(60,80);
        QRect cr = ml->geometry();
        cr.moveCenter(rect().center());
        ml->move(cr.topLeft() + QPoint(dis(gen),dis(gen)));
        ml->setInfo(info.cameraPos);
        QPalette pal = ml->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        ml->setPalette(pal);
//        ml->show();
    }
//    QPieSeries *pieSeries = static_cast<QPieSeries*>(pieChartView_->chart()->series().first());
//    std::random_device device;
//    std::mt19937 gen(device());
//    std::uniform_int_distribution<int> dis(0,255);
//    foreach (const QString &info, axies) {
//        QPieSlice *slice = pieSeries->append(info,dis(gen));
//        QColor c(dis(gen),dis(gen),dis(gen));
//        slice->setBorderColor(c);
//        slice->setColor(c);
//        slice->setLabelColor(c);
//        slice->setLabelVisible();
//    }
}

void MainPage::slotPieChart(QVector<kf::PieCharData> pieData)
{
    QPieSeries *pieSeries = static_cast<QPieSeries*>(pieChartView_->chart()->series().first());
    pieSeries->clear();
    for(const kf::PieCharData &sliceData : pieData){
        QPieSlice *slice = pieSeries->append(QString::fromStdString(sliceData.camera_pos()),sliceData.cost_time());
        QColor c = pieChartColor_.value(slice->label());
        if(!c.isValid()){
            std::random_device device;
            std::mt19937 gen(device());
            std::uniform_int_distribution<int> dis(0,255);
            c = QColor(dis(gen),dis(gen),dis(gen));
            pieChartColor_[slice->label()] = c;
        }
        slice->setBorderColor(c);
        slice->setColor(c);
        slice->setLabelColor(c);
        slice->setLabelVisible();
    }
}

void MainPage::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}
