#include <QTreeWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QResizeEvent>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QDateTime>
#include <QSettings>
#include <QMenu>
#include <QMessageBox>
#include <QTimer>
#include <QPainter>
#include <QDebug>
#include <QScrollBar>
#include "realtimemonitoring.h"
#include "realplaymanager.h"
#include "cornerwidget.h"
#include "service/restservice.h"
#include "realmonitorsetting.h"
#include "facesearch.h"
#include "waitinglabel.h"
#include "videoplayer.h"

#pragma execution_character_set("utf-8")
RealtimeMonitoring::RealtimeMonitoring(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("实时监控"));
    backImg_.load("images/Mask.png");
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hboxLay = new QHBoxLayout;
    m_settingBtn = new QPushButton(tr("设置"));
    hboxLay->addWidget(m_settingBtn);
    hboxLay->setAlignment(Qt::AlignRight);
    mainLay->addLayout(hboxLay);

    QHBoxLayout *centerHboxL = new QHBoxLayout;
    m_treeW = new QTreeWidget;
    // 设置水平滚动条设备字体太长还是显示不完整，自动省略
//    m_treeW->header()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_treeW->headerItem()->setText(0,tr("设备列表"));
//    QScrollArea *m_devArea = new QScrollArea();
//    m_devArea->setWidgetResizable(true);
//    m_devArea->setFrameShape(QFrame::NoFrame);
//    m_devArea->setWidget(m_treeW);
    centerHboxL->addWidget(m_treeW,2);

    m_realPlayM = new RealPlayManager(wm);
    m_realPlayM->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_faceList = new QListWidget;
    m_eventList = new QListWidget;
    m_faceListL = new QLabel(tr(" Face\n capture")); //人\n脸\n抓\n拍
    m_faceListL->setAlignment(Qt::AlignCenter);
    m_faceListL->setFixedWidth(70);
    m_eventListL = new QLabel(tr(" Alarm\n event")); // "报\n警\n事\n件
    m_eventListL->setAlignment(Qt::AlignCenter);
    m_eventListL->setFixedWidth(70);
    QVBoxLayout *vboxLay = new QVBoxLayout;
    vboxLay->addWidget(m_realPlayM,17);
    hboxLay = new QHBoxLayout;
    hboxLay->addWidget(m_faceListL);
    hboxLay->addWidget(m_faceList);
    vboxLay->addLayout(hboxLay,4);
    hboxLay = new QHBoxLayout;
    hboxLay->addWidget(m_eventListL);
    hboxLay->addWidget(m_eventList);
    vboxLay->addLayout(hboxLay,4);
    centerHboxL->addLayout(vboxLay,9);

    m_timeList = new QListWidget;
    m_timeLeftL = new QLabel(tr("起点"));
    m_timeRightL = new QLabel(tr("终点"));
    stayPersonBack_ = new QWidget;
    cameraCombox_ = new QComboBox;
    stayPersonTitleL_ = new QLabel(tr("Number of peoples"));
    stayPersonL_ = new QLabel(tr("0"));
    stayPersonL_->setAlignment(Qt::AlignLeft);
    stayPersonTitleL_->setAlignment(Qt::AlignLeft);
    waitingTimeL_ = new QLabel(tr("0"));
    waitingTimeTitleL_ = new QLabel(tr("Average time"));
    waitingTimeL_->setAlignment(Qt::AlignLeft);
    waitingTimeTitleL_->setAlignment(Qt::AlignLeft);
    vboxLay = new QVBoxLayout;
    vboxLay->addWidget(cameraCombox_);
    QGridLayout *countGridLay = new QGridLayout;
    countGridLay->addWidget(stayPersonTitleL_,0,0,1,1);
    countGridLay->addWidget(waitingTimeTitleL_,0,1,1,1);
    countGridLay->addWidget(stayPersonL_,1,0,1,1);
    countGridLay->addWidget(waitingTimeL_,1,1,1,1);
    vboxLay->addLayout(countGridLay);
    stayPersonBack_->setLayout(vboxLay);
    hboxLay = new QHBoxLayout;
    vboxLay = new QVBoxLayout;
    hboxLay->addWidget(m_timeLeftL);
    hboxLay->addWidget(m_timeRightL);
    vboxLay->addWidget(stayPersonBack_,1);
    vboxLay->addLayout(hboxLay);
    vboxLay->addWidget(m_timeList,5);
    centerHboxL->addLayout(vboxLay,3);

    mainLay->addLayout(centerHboxL);
    setLayout(mainLay);

    numberPersonTimer_ = new QTimer(this);
    faceItemMenu_ = new QMenu(m_faceList);
    faceItemMenu_->addAction(tr("查看最近抓拍记录"),[&]{
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
        faceDialog->setFaceImage(m_faceList->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceDialog->setOid(m_faceList->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    faceItemMenu_->addAction(tr("查看场景图"),[&]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            QMessageBox::information(this,tr("Scene"),str);
            faceItemMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnScenePic(img);
            faceItemMenu_->setEnabled(true);
        });
        label->show(800);
        faceItemMenu_->setEnabled(false);
        serviceI->getScenePic(m_faceList->currentItem()->data(Qt::UserRole + 2).toString());
        startWorker(worker);
    });
    m_faceList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_faceList,&QListWidget::customContextMenuRequested,this,[&](const QPoint &p){
        if(!m_faceList->itemAt(p)){
            return;
        }
        faceItemMenu_->move(m_faceList->mapToGlobal(p));
        faceItemMenu_->show();
    });

    eventItemMenu_ = new QMenu(m_eventList);
    eventItemMenu_->addAction(tr("查看场景图"),[&]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            QMessageBox::information(this,tr("Scene"),str);
            eventItemMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnScenePic(img);
            eventItemMenu_->setEnabled(true);
        });
        label->show(800);
        eventItemMenu_->setEnabled(false);
        serviceI->getAlarmScenePic(m_eventList->currentItem()->data(Qt::UserRole).toString());
        startWorker(worker);
    });
    m_eventList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_eventList,&QListWidget::customContextMenuRequested,this,[&](const QPoint &p){
        if(!m_eventList->itemAt(p)){
            return;
        }
        eventItemMenu_->move(m_eventList->mapToGlobal(p));
        eventItemMenu_->show();
    });

    QSettings setting("config.ini",QSettings::IniFormat);
    QStringList values = setting.value("VideoScreen/default").toString().split(' ');
    if(values.count() == 6){
        m_realPlayM->splitScreen(values.at(0).toInt(),values.at(1).toInt(),values.at(2).toInt(),values.at(3).toInt(),values.at(4).toInt(),values.at(5).toInt());
    }else{
        m_realPlayM->splitScreen(3,3,0,0,2,2);
    }

    //! 增加播放顺序控制
    QStringList order = setting.value("ShowOrder/Order").toString().split('-');
    qDebug() << "order" << order;
    if (2 == order.length())
    {
        m_nBeginIndex = order[0].toInt();
        m_nEndIndex = order[1].toInt();
    }
    //! 增加播放顺序控制

    connect(m_settingBtn,&QPushButton::clicked,this,[&]{
        RealMonitorSetting settingDialog(widgetManger()->workerManager(), this, Qt::Dialog | Qt::WindowCloseButtonHint);
        settingDialog.setMinimumWidth(1200);
        settingDialog.setScreenIndex(m_realPlayM->screenCount());
        settingDialog.setUserStyle(widgetManger()->currentStyle());
        settingDialog.exec();
        int rows = 0, cols = 0, bigRow = 0, bigCol = 0, bigRowSpan = 0, bigColSpan = 0;
        settingDialog.screenSelected(&rows,&cols,&bigRow,&bigCol,&bigRowSpan,&bigColSpan);
        m_realPlayM->splitScreen(rows,cols,bigRow,bigCol,bigRowSpan,bigColSpan);
    });
    notifyServiceI_ = dynamic_cast<NotifyServiceI*>(getWoker("NotifyService"));
    connect(notifyServiceI_,SIGNAL(sigGrabedPerson(QStringList,QImage)),this,SLOT(slotAddFaceitem(QStringList,QImage)));
    connect(notifyServiceI_,SIGNAL(sigBlackListAlarmFace(QStringList,QImage)),this,SLOT(slotAddEventitem(QStringList,QImage)));
    connect(notifyServiceI_,SIGNAL(sigIntruderAlarmFace(QStringList,QImage)),this,SLOT(slotAddEventitem(QStringList,QImage)));
    connect(notifyServiceI_,SIGNAL(sigTimeCost(QMap<QString,QVariant>)),this,SLOT(slotAddTimeitem(QMap<QString,QVariant>)));
    connect(notifyServiceI_,SIGNAL(sigABDoorAlarmFace(QStringList,QImage)),this,SLOT(slotAddEventitem(QStringList,QImage)));
    connect(m_treeW,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeItemDoubleClicked(QTreeWidgetItem*,int)));
    connect(cameraCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotCameraComboxIndexChanged(int)));
    connect(numberPersonTimer_,SIGNAL(timeout()),this,SLOT(slotPersonCountTimer()));
    numberPersonTimer_->start(1000);

    updateCamera();
    getCameraGroup(nullptr,"1005");
}

RealtimeMonitoring::~RealtimeMonitoring()
{
    numberPersonTimer_->stop();
}

void RealtimeMonitoring::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    QFont f;
    if(s == WidgetManagerI::Danyahei){
        m_treeW->header()->setStretchLastSection(true);
        m_treeW->header()->setIconSize(QSize(50,50));
        QSize s = m_treeW->headerItem()->sizeHint(0);
        m_treeW->headerItem()->setSizeHint(0,QSize(s.width(),60));
        m_treeW->headerItem()->setTextAlignment(0,Qt::AlignCenter);
        m_treeW->setStyleSheet("QTreeView{"
                               "border:1px solid #4C5A6B;"
                               "font-size: 16px;"
                               "color: #CECECE;"
                               "border-radius: 10px;"
                               "background-color: rgba(0,0,0,0.35);}");
        m_treeW->verticalScrollBar()->setStyleSheet(
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
        m_treeW->header()->setStyleSheet("QHeaderView{"
                                         "background-color: transparent;"
                                         "border-radius:10px;"
                                         "}"
                                         "QHeaderView::section{"
                                         "color: #CECECE;"
                                         "font-size:16px;"
                                         "border:none;"
                                         "background-color: transparent;"
                                         "}");

        m_timeLeftL->setAlignment(Qt::AlignCenter);
        m_timeRightL->setAlignment(Qt::AlignCenter);

        pal = m_timeLeftL->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        pal.setBrush(QPalette::Background, QColor(83,89,100));
        m_timeLeftL->setPalette(pal);
        m_timeRightL->setPalette(pal);
        f = m_timeLeftL->font();
        f.setPixelSize(16);
        m_timeLeftL->setFont(f);
        m_timeRightL->setFont(f);

        m_faceListL->setFont(f);
        m_faceListL->setWordWrap(true);
        m_faceListL->setAlignment(Qt::AlignVCenter);
        m_faceListL->setStyleSheet("QLabel{"
                                   "color: white;"
                                   "font-size: 16px;"
                                   "background-color:rgba(255,255,255,40);"
                                   "border-radius:6px;}");
        m_faceList->setFlow(QListWidget::LeftToRight);
        m_faceList->setStyleSheet(".QListWidget{background-color:rgba(83,89,100,40); border-radius:6px;border:none;}");
//        m_faceList->setMaximumHeight(137);

        m_eventListL->setWordWrap(true);
        m_eventListL->setAlignment(Qt::AlignVCenter);
        m_eventListL->setStyleSheet("QLabel{"
                                   "color: white;"
                                   "font-size: 16px;"
                                   "background-color:rgba(255,255,255,40);"
                                   "border-radius:6px;}");
        m_eventList->setFlow(QListWidget::LeftToRight);
        m_eventList->setStyleSheet("QListWidget{"
                                   "background-color:rgba(0,0,0,40);"
                                   "border-radius:6px;"
                                   "border:none;"
                                   "}");
//        m_eventList->setMaximumHeight(137);

        cameraCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: transparent;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: #525964;"
                    "border: 1px solid #CECECE;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "background-color: #525964;"
                    "border-radius: 6px;"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: #CECECE;"
                    "}"
                    "QComboBox::drop-down{"
                    "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                    "}"
                    "QScrollBar:vertical{"
                    "background: transparent;"
                    "border: 0px solid gray;"
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
                    "border: none;"
                    "border-radius: 0px;"
                    "}");
        waitingTimeTitleL_->setStyleSheet("QLabel{"
                                         "color: white;"
                                         "background-color: transparent;"
                                         "font-size: 16px;"
                                         "}");
        waitingTimeL_->setStyleSheet("QLabel{"
                                    "color: white;"
                                    "background-color: transparent;"
                                    "font-size: 120px;"
                                    "}");
        stayPersonBack_->setStyleSheet("QWidget{"
                                       "background-color: rgba(0,0,0,40);"
                                       "border-radius: 6px;"
                                       "}");
        stayPersonTitleL_->setStyleSheet("QLabel{"
                                         "color: white;"
                                         "background-color: transparent;"
                                         "font-size: 16px;"
                                         "}");
        stayPersonL_->setStyleSheet("QLabel{"
                                    "color: white;"
                                    "background-color: transparent;"
                                    "font-size: 120px;"
                                    "}");
        pal = m_timeList->palette();
        pal.setColor(QPalette::Foreground,Qt::yellow);
        pal.setColor(QPalette::Text,Qt::white);
        m_timeList->setPalette(pal);
        m_timeList->setStyleSheet("QListWidget{"
                                  "background-color:rgba(0,0,0,40);"
                                  "border-radius:6px;"
                                  "border:none;"
                                  "}");

        m_settingBtn->setFlat(true);
        pal = m_settingBtn->palette();
        pal.setColor(QPalette::ButtonText,Qt::white);
        pal.setColor(QPalette::Button, QColor(83,89,100));
        m_settingBtn->setPalette(pal);
        m_settingBtn->setAutoFillBackground(true);
        m_settingBtn->setStyleSheet("background-color:rgba(206,206,206,40);");

       faceItemStyleSheet_ = "QLabel{color:white;font-size: 9pt;font-family:微软雅黑;}";
       eventItemBlackAlarStyleSheet_ = "QLabel{color:yellow;font-size: 9pt;font-family:微软雅黑;}";
       eventItemIntruderStyleSheet_ = "QLabel{color:white;font-size: 9pt;font-family:微软雅黑;}";
       eventItemABDoor_ = "QLabel{color:red;font-size: 9pt;font-family:微软雅黑;}";
    }
}

void RealtimeMonitoring::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    m_timeItemH = (m_timeList->height() - (TIMEITEMCOUNT + 1) * m_timeList->spacing() - m_timeList->frameWidth() * 2) / TIMEITEMCOUNT;
    for(int i = 0; i < m_timeList->count(); i++){
        QListWidgetItem *item = m_timeList->item(i);
        item->setSizeHint(QSize(item->sizeHint().width(),m_timeItemH));
    }

    int faceItemW = (m_faceList->width() - (FACEITEMCOUNT + 1) * m_faceList->spacing() - m_faceList->frameWidth() * 2) / FACEITEMCOUNT;
    int faceItemH = m_faceList->height() - m_faceList->spacing() * 2 - m_faceList->frameWidth() * 2;
    m_faceItemSize = QSize(faceItemW,faceItemH);
    for(int i = 0; i < m_faceList->count(); i++){
        QListWidgetItem *item = m_faceList->item(i);
        item->setSizeHint(m_faceItemSize);
    }

    int eventItemW = (m_eventList->width() - (EVENTITEMCOUNT + 1) * m_eventList->spacing() - m_eventList->frameWidth() * 2) / EVENTITEMCOUNT;
    int eventItemH = m_eventList->height() - m_eventList->spacing() * 2 - m_eventList->frameWidth() * 2;
    m_eventItemSize = QSize(eventItemW,eventItemH);
    for(int i = 0; i < m_eventList->count(); i++){
        QListWidgetItem *item = m_eventList->item(i);
        item->setSizeHint(m_eventItemSize);
    }
}

void RealtimeMonitoring::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void RealtimeMonitoring::updateCamera()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
#if 0
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<CameraInfo>)),this,SLOT(slotAddDevice(QVector<CameraInfo>)));
#else
    connect(serviceI,SIGNAL(sigCameraMap(QVariantMap)),this,SLOT(slotOnCameraMap(QVariantMap)));
#endif
    serviceI->getCameraInfo();
    startWorker(worker);
}

void RealtimeMonitoring::getCameraGroup(QTreeWidgetItem* item,QString groupNo)
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,&RestServiceI::sigCameraGroup,this,[item,this](QVector<RestServiceI::CameraGoup> groups){
        foreach (const RestServiceI::CameraGoup &groupV, groups) {
            QTreeWidgetItem *childItem = nullptr;
            qDebug() << item << groupV.deviceNumber;
            if(!item){
                childItem = new QTreeWidgetItem(m_treeW,QStringList() << groupV.groupName,0);
            }else{
                childItem = new QTreeWidgetItem(item,QStringList() << groupV.groupName,0);
            }
            childItem->setData(0,Qt::UserRole,groupV.groupNo);
            childItem->setData(1,Qt::UserRole + 1,groupV.description);
            if(groupV.deviceNumber){
                getCameraDevice(childItem,groupV.groupNo);
                getCameraGroup(childItem,groupV.groupNo);
            }
        }
    });
    serviceI->getCameraGroup(groupNo);
    startWorker(worker);
}

void RealtimeMonitoring::getCameraDevice(QTreeWidgetItem *item, QString groupNo)
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,&RestServiceI::sigCameraInfo,this,[this,item](QVector<CameraInfo> devices){
        foreach (const CameraInfo &info, devices) {
            QTreeWidgetItem *camera = new QTreeWidgetItem(item, QStringList() << QString::fromStdString(info.position),1);
            camera->setData(0,Qt::UserRole + 1, QString::fromStdString(info.id));
            camera->setData(0,Qt::UserRole + 2, QString::fromStdString(info.rtsp));
            camera->setData(0,Qt::UserRole + 3, QString::fromStdString(info.position));
            camera->setToolTip(0,tr("position:%1\n"
                                   "cameraId:%2\n"
                                    "%3").arg(QString::fromStdString(info.position))
                               .arg(QString::fromStdString(info.id))
                               .arg(QString::fromStdString(info.rtsp)));
            if(!camera->data(0,Qt::UserRole + 2).toString().isEmpty()){
                m_realPlayM->playByOrder(QString::fromStdString(info.rtsp),QString::fromStdString(info.id),camera->text(0));
            }
        }
    });
    serviceI->getCameraDevice(groupNo);
    startWorker(worker);
}

QString RealtimeMonitoring::findCameraNameById(QString &id)
{
    QTreeWidgetItemIterator it(m_treeW);
    while (*it) {
        if ((*it)->data(0,Qt::UserRole + 1).toString() == id)
            return (*it)->text(0);
        ++it;
    }

    return QString();
}

void RealtimeMonitoring::slotCameraComboxIndexChanged(int index)
{

}

void RealtimeMonitoring::slotTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->type() == 0){
        return;
    }
    if(!m_realPlayM->focusPlayer()){
        QMessageBox::information(this,tr("播放视频"),tr("请先选中一个窗口"));
        return;
    }

    QString cameraId = item->data(column,Qt::UserRole + 1).toString();
    if(cameraId.isEmpty()){
        return;
    }

    m_realPlayM->playByFocus(item->data(column,Qt::UserRole + 2).toString(),item->data(column,Qt::UserRole + 1).toString(),item->data(column,Qt::UserRole + 3).toString());
}

void RealtimeMonitoring::slotAddTimeitem(QMap<QString, QVariant> data)
{
    if(m_timeList->count() >= TIMEITEMCOUNT){
        QListWidgetItem *delItem = m_timeList->takeItem(TIMEITEMCOUNT - 1);
        m_timeList->removeItemWidget(delItem);
        delete delItem;
    }
    QPalette pal = m_timeList->palette();
    QWidget *itemWidget = new QWidget;
    QHBoxLayout *mainLay = new QHBoxLayout;
    QWidget *cornerW = new CornerWidget;
    cornerW->setPalette(pal);
    QVBoxLayout *vboxLay = new QVBoxLayout;
    QLabel *label = new QLabel;
    label->setScaledContents(true);
    label->setPixmap(QPixmap::fromImage(data.value("startImg").value<QImage>()));
    vboxLay->addWidget(label);
    label = new QLabel(tr("位置:") + findCameraNameById(data.value("startLocation").toString()));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setPalette(pal);
    vboxLay->addWidget(label);
    label = new QLabel(tr("时间:") + data.value("startTime").toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    vboxLay->addWidget(label);
    cornerW->setLayout(vboxLay);
    mainLay->addWidget(cornerW);

    label = new QLabel(tr("---%1second--->>").arg(data.value("timeCost").toInt())); // ---%1秒--->>
    label->setAlignment(Qt::AlignCenter);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainLay->addWidget(label);

    cornerW = new CornerWidget;
    cornerW->setPalette(pal);
    vboxLay = new QVBoxLayout;
    label = new QLabel;
    label->setScaledContents(true);
    label->setPixmap(QPixmap::fromImage(data.value("endImg").value<QImage>()));
    vboxLay->addWidget(label);
    label = new QLabel(tr("位置:") + findCameraNameById(data.value("endLocation").toString()));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    vboxLay->addWidget(label);
    label = new QLabel(tr("时间:") + data.value("endTime").toDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    vboxLay->addWidget(label);
    cornerW->setLayout(vboxLay);
    mainLay->addWidget(cornerW);
    itemWidget->setLayout(mainLay);

    QListWidgetItem *item = new QListWidgetItem;
    item->setSizeHint(QSize(item->sizeHint().width(),m_timeItemH));
    m_timeList->insertItem(0,item);
    m_timeList->setItemWidget(item,itemWidget);
#if 1
    totalPerson_++;
    totalTime_ += data.value("timeCost").toInt();
    waitingTimeL_->setNum(totalTime_ / totalPerson_);
#endif
}

void RealtimeMonitoring::slotAddFaceitem(QStringList data, QImage img)
{
    if(m_faceList->count() >= FACEITEMCOUNT){
        QListWidgetItem *delItem = m_faceList->takeItem(FACEITEMCOUNT - 1);
        m_faceList->removeItemWidget(delItem);
        delete delItem;
    }
    QWidget *itemWidget = new QWidget;
    QVBoxLayout *vboxLay = new QVBoxLayout;
    QLabel *label = new QLabel;
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignHCenter);
    label->setPixmap(QPixmap::fromImage(img));
    vboxLay->addWidget(label);

    if(data.count() < 2)return;
    QString oid = data.takeFirst();
    foreach (QString str, data) {
        label = new QLabel(str);
        QPalette pal = label->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        label->setPalette(pal);
        label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        label->setStyleSheet(faceItemStyleSheet_);
        vboxLay->addWidget(label);
    }
    itemWidget->setLayout(vboxLay);

    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::UserRole + 1,img);
    item->setData(Qt::UserRole + 2,oid);
    item->setSizeHint(m_faceItemSize);
    m_faceList->insertItem(0,item);
    m_faceList->setItemWidget(item,itemWidget);
}

void RealtimeMonitoring::slotAddEventitem(QStringList data, QImage img)
{
    if(data.count() < 3){
        return;
    }
    if(m_eventList->count() >= EVENTITEMCOUNT){
        QListWidgetItem *delItem = m_eventList->takeItem(EVENTITEMCOUNT - 1);
        m_eventList->removeItemWidget(delItem);
        delete delItem;
    }
    QWidget *itemWidget = new QWidget;
    QVBoxLayout *vboxLay = new QVBoxLayout;
    QLabel *label = new QLabel;
    label->setScaledContents(true);
    label->setAlignment(Qt::AlignHCenter);
    label->setPixmap(QPixmap::fromImage(img));
    vboxLay->addWidget(label);

    QString oid = data.takeLast();
    foreach (QString str, data) {
        label = new QLabel(str);
        label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
        vboxLay->addWidget(label);
    }
    itemWidget->setLayout(vboxLay);

    QListWidgetItem *item = new QListWidgetItem;
    if(!data.isEmpty()){
        QString waryingType = data.last();
        if(waryingType == tr("闯入报警")){
            for(int i = 1; i < vboxLay->count(); i++){
                vboxLay->itemAt(i)->widget()->setStyleSheet(eventItemIntruderStyleSheet_);
            }
        }else if(waryingType == tr("黑名单报警")){
            for(int i = 1; i < vboxLay->count(); i++){
                vboxLay->itemAt(i)->widget()->setStyleSheet(eventItemBlackAlarStyleSheet_);
            }
        }else if(waryingType == tr("AB门报警")){
            for(int i = 1; i < vboxLay->count(); i++){
                vboxLay->itemAt(i)->widget()->setStyleSheet(eventItemABDoor_);
            }
        }
    }
    item->setSizeHint(m_eventItemSize);
    item->setData(Qt::UserRole,oid);
    m_eventList->insertItem(0,item);
    m_eventList->setItemWidget(item,itemWidget);
}

void RealtimeMonitoring::slotAddDevice(QVector<CameraInfo> data)
{
//    QTreeWidgetItem *item = new QTreeWidgetItem(m_treeW,QStringList() << tr("Cameras"));
    foreach (const CameraInfo &info, data) {
#if 0
        QTreeWidgetItem *camera = new QTreeWidgetItem(item, QStringList() << QString::fromStdString(info.position));
        camera->setData(0,Qt::UserRole + 1, QString::fromStdString(info.id));
        camera->setData(0,Qt::UserRole + 2, QString::fromStdString(info.rtsp));
        camera->setData(0,Qt::UserRole + 3, QString::fromStdString(info.position));
        camera->setToolTip(0,tr("position:%1\n"
                               "cameraId:%2\n"
                                "%3").arg(QString::fromStdString(info.position))
                           .arg(QString::fromStdString(info.id))
                           .arg(QString::fromStdString(info.rtsp)));
        if(!info.rtsp.empty()){
            m_realPlayM->playByOrder(QString::fromStdString(info.rtsp),QString::fromStdString(info.id),camera->text(0));
        }
        cameraCombox_->addItem(QString::fromStdString(info.position),QString::fromStdString(info.id));
#else
        cameraCombox_->addItem(QString::fromStdString(info.position),QString::fromStdString(info.id));
#endif
    }
    slotPersonTotalCountTimeout();
}

void RealtimeMonitoring::slotOnCameraGroup(QVector<RestServiceI::CameraGoup> groups)
{
    foreach (const RestServiceI::CameraGoup &groupV, groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeW,QStringList() << groupV.groupName,0);
        item->setData(0,Qt::UserRole,groupV.groupNo);
        item->setData(1,Qt::UserRole + 1,groupV.description);
    }
}

void RealtimeMonitoring::slotOnCameraMap(QVariantMap datas)
{
    curCameraMap_ = datas;
    cameraCombox_->clear();
    QStringList mapKeys = curCameraMap_.keys();
    for(auto mapKey : mapKeys){
        cameraCombox_->addItem(curCameraMap_.value(mapKey).toString(),mapKey);
    }
}

void RealtimeMonitoring::slotOnScenePic(QImage img)
{
    QDialog dialog;
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    QLabel *label = new QLabel;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    dialog.setLayout(lay);
    dialog.setModal(true);

    label->setScaledContents(true);
    label->setPixmap(QPixmap::fromImage(img));
    dialog.setFixedSize(960,540);
    dialog.exec();
}

void RealtimeMonitoring::slotPersonStayInfoTimeout()
{
    if(!widgetManger() || !widgetManger()->workerManager()){
        return;
    }
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    RestServiceI::AveragePersonTimeArgs averageArgs;
    averageArgs.cameraId = cameraCombox_->currentData().toString();
    averageArgs.startT = QDateTime::currentDateTime().addSecs(-3600);
    averageArgs.endT = QDateTime::currentDateTime();
    serviceI->getAverageTime(averageArgs);
}

void RealtimeMonitoring::slotPersonTotalCountTimeout()
{
    noNumbersPersonDataCount_ = 0;
    if(!widgetManger() || !widgetManger()->workerManager()){
        return;
    }
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    RestServiceI::PersonsStayArgs args;
    args.cameraId = cameraCombox_->currentData().toString();
    args.startT = QDateTime::currentDateTime().addSecs(-1);
    args.endT = QDateTime::currentDateTime();
    connect(serviceI,&RestServiceI::sigPersonNumbers,this,[this](int nums){
        stayPersonL_->setNum(nums);
        qDebug() << "person numbers" << nums;
        QTimer::singleShot(1000,this,[this]{
            slotPersonTotalCountTimeout();
        });
    });
    serviceI->getPersonStayTotalCount(args);
    startWorker(worker);
}

void RealtimeMonitoring::slotPersonCountTimer()
{
    noNumbersPersonDataCount_++;
    if(noNumbersPersonDataCount_ == 5){
        noNumbersPersonDataCount_ = 0;
        slotPersonTotalCountTimeout();
    }
}
