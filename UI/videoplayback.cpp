#include <QCalendarWidget>
#include <QTableWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTextCharFormat>
#include <QDebug>
#include <QToolButton>
#include <QPainter>
#include <QApplication>
#include <QScrollBar>
#include "videoplayback.h"
#include "hkplayback.h"

#pragma execution_character_set("utf-8")
VideoPlayback::VideoPlayback( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("视频回放"));
    backImg_.load("images/Mask.png");
    QHBoxLayout *mainLay = new QHBoxLayout;
    deviceTree_ = new QTreeWidget;
    hkplayback_ = new HKPlayBack;

    QVBoxLayout *vLay = new QVBoxLayout;
    timeLabel_ = new QLabel(tr("时间"));
    calendarWidget = new QCalendarWidget;
    {
        QToolButton *tools = calendarWidget->findChild<QToolButton *>(QLatin1String("qt_calendar_prevmonth"));
        QIcon i("images/PrevButton.png");
        tools->setIcon(i);
    }
    {
        QToolButton *tools = calendarWidget->findChild<QToolButton *>(QLatin1String("qt_calendar_nextmonth"));
        QIcon i("images/NextButton.png");
        tools->setIcon(i);
    }
    calendarWidget->setFocusPolicy(Qt::NoFocus);
    fileLabel_ = new QLabel(tr("录像文件"));
    tableWidget_ = new QTableWidget;
    vLay->addWidget(timeLabel_);
    vLay->addWidget(calendarWidget);
    vLay->addWidget(fileLabel_);
    vLay->addWidget(tableWidget_);
    vLay->setAlignment(Qt::AlignTop);

    mainLay->addWidget(deviceTree_,18);
    mainLay->addWidget(hkplayback_,79);
    mainLay->addLayout(vLay,28);
    setLayout(mainLay);

    tableWidget_->setColumnCount(3);
    tableWidget_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableWidget_->setEditTriggers(QTableWidget::NoEditTriggers);
    tableWidget_->setFocusPolicy(Qt::NoFocus);
    tableWidget_->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableWidget_->verticalHeader()->hide();
    tableWidget_->setHorizontalHeaderLabels(QStringList() << tr("摄像头") << tr("日期") << tr("时间段"));
    deviceTree_->headerItem()->setText(0,tr("设备列表"));
    connect(deviceTree_,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotItemClicked(QTreeWidgetItem*,int)));
    connect(calendarWidget,SIGNAL(clicked(QDate)),this,SLOT(slotUploadTable(QDate)));
    connect(tableWidget_,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(slotTableItemDoubleClicked(QTableWidgetItem*)));

    setUserStyle(userStyle());
    updateCamera();
//    getCameraGroup(nullptr,"1005");
}

void VideoPlayback::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        deviceTree_->header()->setStretchLastSection(true);
        deviceTree_->header()->setIconSize(QSize(50,50));
        QSize s = deviceTree_->headerItem()->sizeHint(0);
        deviceTree_->headerItem()->setSizeHint(0,QSize(s.width(),60));
        deviceTree_->headerItem()->setTextAlignment(0,Qt::AlignCenter);
        deviceTree_->setStyleSheet("QTreeView{"
                                   "border:1px solid #4C5A6B;"
                                   "font-size: 16px;"
                                   "color: #CECECE;"
                                   "border-radius: 10px;"
                                   "background-color: rgba(0,0,0,0.35);}");
        deviceTree_->verticalScrollBar()->setStyleSheet(
                                                    "QScrollBar:vertical{"
                                                    "background: transparent;"
                                                    "border-radius: 10px;"
                                                    "border: none;"
                                                    "width: 13px;"
                                                    "}"
                                                    "QScrollBar::handle:vertical{"
                                                    "background: rgba(255,255,255,0.5);"
                                                    "border-radius: 5px;"
                                                    "}"
                                                    "QScrollBar::add-line:vertical{"
                                                    "background: transparent;"
                                                    "border:0px solid #274168;"
                                                    "border-radius: 5px;"
                                                    "min-height: 10px;"
                                                    "width: 13px;"
                                                    "}"
                                                    "QScrollBar::sub-line:vertical{"
                                                    "background: transparent;"
                                                    "border:0px solid #274168;"
                                                    "min-height: 10px;"
                                                    "width: 13px;"
                                                    "}"
                                                    "QScrollBar::up-arrow:vertical{"
                                                    "subcontrol-origin: margin;"
                                                    "height: 0px;"
                                                    "border:0 0 0 0;"
                                                    "visible:false;"
                                                    "}"
                                                    "QScrollBar::down-arrow:vertical{"
                                                    "subcontrol-origin: margin;"
                                                    "height: 0px;"
                                                    "visible:false;"
                                                    "}"
                                                    "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                                    "background: transparent;"
                                                    "border-radius: 10px;"
                                                    "}");
        deviceTree_->header()->setStyleSheet("QHeaderView{"
                                         "background-color: transparent;"
                                         "border-radius:10px;"
                                         "}"
                                         "QHeaderView::section{"
                                         "color: #CECECE;"
                                         "font-size:16px;"
                                         "border:none;"
                                         "background-color: transparent;"
                                         "}");
        calendarWidget->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        pal = calendarWidget->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        calendarWidget->setPalette(pal);
        f = calendarWidget->font();
        f.setPixelSize(27);
        calendarWidget->setFont(f);
        //".QCalendarWidget QWidget{background-color:#CECECE;border:1px solid green;border-radius:6px;}"
        calendarWidget->setStyleSheet("QCalendarWidget QWidget#qt_calendar_navigationbar{"
                                      "background-color: #435464;"
                                      "min-height:26px;"
                                      "max-height:26px;"
                                      "color:#B6A26D;font-size:15px;font-family:Arial;font:bold;"
                                      "}"
                                      "QCalendarWidget QTableView{"
                                      "alternate-background-color: rgba(206,206,206,40);"
                                      "background-color: #435464;"
                                      "selection-background-color: #B4A06C;"
                                      "font-size:15px;color:#CECECE;"
                                      "}"
                                      "QCalendarWidget QMenu {background-color: #CECECE; color:#B6A26D; }QCalendarWidget QMenu::item:selected {background-color:#808080;}"
                                      "QToolButton#qt_calendar_monthbutton,QToolButton#qt_calendar_yearbutton{color:#B6A26D;font-size:15px;font-family:Arial;}");
        calendarWidget->setGridVisible(true);

        pal = fileLabel_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        fileLabel_->setPalette(pal);
        f = fileLabel_->font();
        f.setPixelSize(16);
        fileLabel_->setFont(f);

        timeLabel_->setPalette(pal);
        timeLabel_->setFont(f);

        tableWidget_->setStyleSheet(
                    "QTableView{"
                    "color: white;"
                    "selection-color: white;"
                    "border:1px solid #CECECE;"
                    "background-color: transparent;;"
                    "border-radius:6px;"
                    "selection-background-color: rgba(206,206,206,40);"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgba(206,206,206,40);"
                    "}"
                    "QHeaderView{"
                    "background-color: rgba(206,206,206,20);"
                    "}"
                    "QHeaderView::section{"
                    "color: white;"
                    "background-color: rgba(206,206,206,40);"
                    "}");
    }
    hkplayback_->setUserStyle(s);
}

void VideoPlayback::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void VideoPlayback::updateCamera()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<CameraInfo>)),this,SLOT(slotAddDevice(QVector<CameraInfo>)));
    serviceI->getCameraInfo();
}

void VideoPlayback::getCameraGroup(QTreeWidgetItem *item, QString groupNo)
{
//    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
//    RestServiceI *serviceI = factoryI->makeRestServiceI();
//    connect(serviceI,&RestServiceI::sigCameraGroup,this,[item,this](QVector<RestServiceI::CameraGoup> groups){
//        foreach (const RestServiceI::CameraGoup &groupV, groups) {
//            QTreeWidgetItem *childItem = nullptr;
//            qDebug() << item << groupV.deviceNumber;
//            if(!item){
//                childItem = new QTreeWidgetItem(deviceTree_,QStringList() << groupV.groupName,0);
//            }else{
//                childItem = new QTreeWidgetItem(item,QStringList() << groupV.groupName,0);
//            }
//            childItem->setData(0,Qt::UserRole,groupV.groupNo);
//            childItem->setData(1,Qt::UserRole + 1,groupV.description);
//            if(groupV.deviceNumber){
//                getCameraDevice(childItem,groupV.groupNo);
//                getCameraGroup(childItem,groupV.groupNo);
//            }
//        }
//    });
//    serviceI->getCameraGroup(groupNo);
}

void VideoPlayback::getCameraDevice(QTreeWidgetItem *item, QString groupNo)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,&RestServiceI::sigCameraInfo,this,[this,item](QVector<RestServiceI::CameraInfo> devices){
        for (auto &info : devices) {
            QTreeWidgetItem *camera = new QTreeWidgetItem(item, QStringList() << info.cameraPos,1);
            camera->setData(0,Qt::UserRole + 1, info.cameraId);
            camera->setData(0,Qt::UserRole + 2, info.rtsp);
            camera->setData(0,Qt::UserRole + 3, info.cameraPos);
            camera->setToolTip(0,tr("position:%1\n"
                                   "cameraId:%2\n"
                                    "%3").arg(info.cameraPos)
                               .arg(info.cameraId)
                               .arg(info.rtsp));
        }
    });
    serviceI->getCameraDevice(groupNo);
}

void VideoPlayback::slotUploadTable(QDate d)
{
    if(!deviceTree_->selectedItems().count() || deviceTree_->selectedItems().first()->data(0,Qt::UserRole + 1).toString().isEmpty())return;
    tableWidget_->model()->removeRows(0,tableWidget_->rowCount());
    QTreeWidgetItem *treeItem = deviceTree_->selectedItems().first();
    for(int i = 0; i < 24; i+=2){
        tableWidget_->insertRow(tableWidget_->rowCount());

        QTableWidgetItem *item = new QTableWidgetItem;
        item->setText(treeItem->text(0));
        item->setData(Qt::UserRole + 1,treeItem->data(0,Qt::UserRole + 1));
        item->setData(Qt::UserRole + 2,treeItem->data(0,Qt::UserRole + 2));
        item->setData(Qt::UserRole + 3,treeItem->data(0,Qt::UserRole + 3));
        item->setData(Qt::UserRole + 4,treeItem->data(0,Qt::UserRole + 4));
        item->setData(Qt::UserRole + 5,treeItem->data(0,Qt::UserRole + 5));
        item->setData(Qt::UserRole + 6,treeItem->data(0,Qt::UserRole + 6));
        item->setData(Qt::UserRole + 7,treeItem->data(0,Qt::UserRole + 7));
        tableWidget_->setItem(tableWidget_->rowCount() - 1,0,item);

        item = new QTableWidgetItem;
        item->setText(d.toString("yyyy-MM-dd"));
        item->setData(Qt::UserRole,d);
        tableWidget_->setItem(tableWidget_->rowCount() - 1,1,item);

        item = new QTableWidgetItem;
        QTime startT(i,0,0),endT(i+2,0,0);
        if(i == 22){
            endT.setHMS(23,59,59);
        }
        item->setText(startT.toString("HH:mm:ss") + " - " + endT.toString("HH:mm:ss"));
        item->setData(Qt::UserRole,startT);
        item->setData(Qt::UserRole + 1,endT);
        tableWidget_->setItem(tableWidget_->rowCount() - 1,2,item);
    }
}

void VideoPlayback::slotItemClicked(QTreeWidgetItem *item, int column)
{
    if(item->childCount() || column)return;

    PlayBackParam param;
    QDateTime dtime;
    dtime.setDate(calendarWidget->selectedDate());
    dtime.setTime(QTime(0,0,0));
    param.startTime = dtime;
    dtime.setTime(QTime(23,59,59));
    param.endTime = dtime;
    param.dvrIp = item->data(0,Qt::UserRole + 3).toString();
    param.dvrPort = item->data(0,Qt::UserRole + 4).toInt();
    param.dvrUser = item->data(0,Qt::UserRole + 5).toString();
    param.dvrPassword = item->data(0,Qt::UserRole + 6).toString();
    param.playBackChannelId = item->data(0, Qt::UserRole + 7).toInt();
    hkplayback_->startPlay(param);

    slotUploadTable(calendarWidget->selectedDate());
}

void VideoPlayback::slotAddDevice(QVector<RestServiceI::CameraInfo> data)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(deviceTree_,QStringList() << tr("Cameras"));
    int txtLen = 0;
    QString devTitle;
    for (auto &info : data) {
        devTitle = info.cameraPos;
        QTreeWidgetItem *camera = new QTreeWidgetItem(item, QStringList() << devTitle);
        if (devTitle.length() > txtLen)
        {
            txtLen = devTitle.length();
        }
        camera->setData(0,Qt::UserRole + 1, info.cameraId);
        camera->setData(0,Qt::UserRole + 2, info.rtsp);
    }
    if(item->childCount()){
        deviceTree_->setItemSelected(item->child(0),true);
        slotUploadTable(calendarWidget->selectedDate());
    }
    if (txtLen > 20)
    {
        deviceTree_->setMinimumWidth(150 + txtLen * 3);
    }
    deviceTree_->expandAll();
}

void VideoPlayback::slotTableItemDoubleClicked(QTableWidgetItem *item)
{
    QTableWidgetItem *cameraItem = tableWidget_->item(item->row(),0);
    QTableWidgetItem *dateItem = tableWidget_->item(item->row(),1);
    QTableWidgetItem *timeItem = tableWidget_->item(item->row(),2);

    PlayBackParam param;
    QDateTime dtime;
    dtime.setDate(dateItem->data(Qt::UserRole).toDate());
    dtime.setTime(timeItem->data(Qt::UserRole).toTime());
    param.startTime = dtime;
    dtime.setTime(timeItem->data(Qt::UserRole + 1).toTime());
    param.endTime = dtime;
    param.dvrIp = cameraItem->data(Qt::UserRole + 3).toString();
    param.dvrPort = cameraItem->data(Qt::UserRole + 4).toInt();
    param.dvrUser = cameraItem->data(Qt::UserRole + 5).toString();
    param.dvrPassword = cameraItem->data(Qt::UserRole + 6).toString();
    param.playBackChannelId = cameraItem->data(Qt::UserRole + 7).toInt();
    hkplayback_->startPlay(param);
}
