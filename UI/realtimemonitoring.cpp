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
#include "sceneimagedialog.h"

#pragma execution_character_set("utf-8")
RealtimeMonitoring::RealtimeMonitoring(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Real-time surveillance"));
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
    m_treeW->headerItem()->setText(0,tr("设备列表"));
    centerHboxL->addWidget(m_treeW,2);

    m_realPlayM = new RealPlayManager(wm);
    m_realPlayM->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_faceList = new QListWidget;
    m_faceListL = new QLabel(tr(" Face\n capture")); //人\n脸\n抓\n拍
    m_faceListL->setAlignment(Qt::AlignCenter);
    m_faceListL->setFixedWidth(70);
    QVBoxLayout *vboxLay = new QVBoxLayout;
    vboxLay->addWidget(m_realPlayM,4);
    hboxLay = new QHBoxLayout;
    hboxLay->addWidget(m_faceListL);
    hboxLay->addWidget(m_faceList);
    vboxLay->addLayout(hboxLay,1);
    centerHboxL->addLayout(vboxLay,11);

    eventCombox_ = new QComboBox;
    eventList_ = new QListWidget;
    vboxLay = new QVBoxLayout;
    vboxLay->addWidget(eventCombox_);
    vboxLay->addWidget(eventList_);
    centerHboxL->addLayout(vboxLay,3);

    mainLay->addLayout(centerHboxL);
    setLayout(mainLay);

    faceItemMenu_ = new QMenu(m_faceList);
    faceItemMenu_->addAction(tr("Search using an image"),[&]{
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
    faceItemMenu_->addAction(tr("Scene analysis"),[&]{
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
        serviceI->getScenePic(m_faceList->currentItem()->data(Qt::UserRole + 3).toString());
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

    eventItemMenu_ = new QMenu(eventList_);
    eventItemMenu_->addAction(tr("Scene analysis"),[&]{
        slotOnScenePic(eventList_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
    });
    eventList_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(eventList_,&QListWidget::customContextMenuRequested,this,[&](const QPoint &p){
        if(!eventList_->itemAt(p)){
            return;
        }
        eventItemMenu_->move(eventList_->mapToGlobal(p));
        eventItemMenu_->show();
    });

    QSettings setting("config.ini",QSettings::IniFormat);
    QStringList values = setting.value("VideoScreen/default").toString().split(' ');
    if(values.count() == 6){
        m_realPlayM->splitScreen(values.at(0).toInt(),values.at(1).toInt(),values.at(2).toInt(),values.at(3).toInt(),values.at(4).toInt(),values.at(5).toInt());
    }else{
        m_realPlayM->splitScreen(3,3,0,0,2,2);
    }

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

    eventCombox_->addItem(tr("All events"));
    eventCombox_->addItem(tr("Blacklist events"));
    eventCombox_->addItem(tr("Intruder events"));
    eventCombox_->addItem(tr("AB-Door evetns"));
    eventList_->setViewMode(QListWidget::IconMode);
    eventList_->setSpacing(10);
    eventList_->setMovement(QListView::Static);
    connect(eventCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotEventComboxIndexChanged(int)));
    notifyServiceI_ = dynamic_cast<NotifyServiceI*>(getWoker("NotifyService"));
    connect(notifyServiceI_,SIGNAL(sigGrabedPerson(QStringList,QImage)),this,SLOT(slotAddFaceitem(QStringList,QImage)));
    connect(m_treeW,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeItemDoubleClicked(QTreeWidgetItem*,int)));

    updateCamera();
    slotEventComboxIndexChanged(0);
//    getCameraGroup(nullptr,"1005");
}

RealtimeMonitoring::~RealtimeMonitoring()
{
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

        eventList_->setFlow(QListWidget::LeftToRight);
        eventList_->setStyleSheet("QListWidget{"
                                   "background-color:rgba(0,0,0,30);"
                                   "border-radius:6px;"
                                   "border:none;"
                                   "}");
//        eventList_->setMaximumHeight(137);

        eventCombox_->setStyleSheet(
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

        m_settingBtn->setFlat(true);
        pal = m_settingBtn->palette();
        pal.setColor(QPalette::ButtonText,Qt::white);
        pal.setColor(QPalette::Button, QColor(83,89,100));
        m_settingBtn->setPalette(pal);
        m_settingBtn->setAutoFillBackground(true);
        m_settingBtn->setStyleSheet("background-color:rgba(206,206,206,40);");

       faceItemStyleSheet_ = "QLabel{color:white;font-size: 9pt;font-family:微软雅黑;}";
    }
}

void RealtimeMonitoring::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    int imgItemH = (eventList_->height() - (EVENTITEMCOUNT + 1) * eventList_->spacing() - eventList_->frameWidth() * 2) / EVENTITEMCOUNT;
    int imgItemW = eventList_->width() - 2 * eventList_->spacing() - 2 * eventList_->frameWidth();
    eventItemSize_.setHeight(imgItemH);
    eventItemSize_.setWidth(imgItemW);
    eventList_->setIconSize(eventItemSize_);
    for(int i = 0; i < eventList_->count(); i++){
        QListWidgetItem *item = eventList_->item(i);
        item->setSizeHint(eventItemSize_);
        item->setIcon(QPixmap::fromImage(item->data(Qt::UserRole + 1).value<QImage>().scaled(eventItemSize_)));
    }

    int faceItemW = (m_faceList->width() - (FACEITEMCOUNT + 1) * m_faceList->spacing() - m_faceList->frameWidth() * 2) / FACEITEMCOUNT;
    int faceItemH = m_faceList->height() - m_faceList->spacing() * 2 - m_faceList->frameWidth() * 2;
    m_faceItemSize = QSize(faceItemW,faceItemH);
    for(int i = 0; i < m_faceList->count(); i++){
        QListWidgetItem *item = m_faceList->item(i);
        item->setSizeHint(m_faceItemSize);
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
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotAddDevice(QVector<RestServiceI::CameraInfo>)));
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
            if(!camera->data(0,Qt::UserRole + 2).toString().isEmpty()){
                m_realPlayM->playByOrder(info.rtsp,info.cameraId,camera->text(0));
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

void RealtimeMonitoring::slotEventComboxIndexChanged(int index)
{
    if(index == 0){
        connect(notifyServiceI_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotOnBlacklistEvent(QStringList,QImage)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotOnIntruderEvent(QStringList,QImage)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotOnAbDoorEvent(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 1){
        disconnect(notifyServiceI_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotOnIntruderEvent(QStringList,QImage)));
        disconnect(notifyServiceI_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotOnAbDoorEvent(QStringList,QImage)));
        connect(notifyServiceI_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotOnBlacklistEvent(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 2){
        disconnect(notifyServiceI_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotOnBlacklistEvent(QStringList,QImage)));
        disconnect(notifyServiceI_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotOnAbDoorEvent(QStringList,QImage)));
        connect(notifyServiceI_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotOnIntruderEvent(QStringList,QImage)),Qt::UniqueConnection);
    }else if(index == 3){
        disconnect(notifyServiceI_,SIGNAL(sigBlackListAlarmScene(QStringList,QImage)),this,SLOT(slotOnBlacklistEvent(QStringList,QImage)));
        disconnect(notifyServiceI_,SIGNAL(sigIntruderAlarmScene(QStringList,QImage)),this,SLOT(slotOnIntruderEvent(QStringList,QImage)));
        connect(notifyServiceI_,SIGNAL(sigABDoorAlarmScene(QStringList,QImage)),this,SLOT(slotOnAbDoorEvent(QStringList,QImage)),Qt::UniqueConnection);
    }
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
    QString faceId = data.takeFirst();
    QString sceneId = data.takeLast();
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
    item->setData(Qt::UserRole + 2,faceId);
    item->setData(Qt::UserRole + 3,sceneId);
    item->setSizeHint(m_faceItemSize);
    m_faceList->insertItem(0,item);
    m_faceList->setItemWidget(item,itemWidget);
}

void RealtimeMonitoring::slotOnIntruderEvent(QStringList, QImage img)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap::fromImage(img).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,QVariant::fromValue(img));
    item->setSizeHint(eventItemSize_);
    eventList_->insertItem(0,item);
}

void RealtimeMonitoring::slotOnBlacklistEvent(QStringList, QImage img)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap::fromImage(img).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,QVariant::fromValue(img));
    item->setSizeHint(eventItemSize_);
    eventList_->insertItem(0,item);
}

void RealtimeMonitoring::slotOnAbDoorEvent(QStringList, QImage img)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap::fromImage(img).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,QVariant::fromValue(img));
    item->setSizeHint(eventItemSize_);
    eventList_->insertItem(0,item);
}

void RealtimeMonitoring::slotAddDevice(QVector<RestServiceI::CameraInfo> data)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(m_treeW,QStringList() << tr("Cameras"),0);
    for (auto &info : data) {
        QTreeWidgetItem *camera = new QTreeWidgetItem(item, QStringList() << info.cameraPos,1);
        camera->setData(0,Qt::UserRole + 1, info.cameraId);
        camera->setData(0,Qt::UserRole + 2, info.rtsp);
        camera->setData(0,Qt::UserRole + 3, info.cameraPos);
        camera->setToolTip(0,tr("position:%1\n"
                               "cameraId:%2\n"
                                "%3").arg(info.cameraPos)
                           .arg(info.cameraId)
                           .arg(info.rtsp));
        if(!info.rtsp.isEmpty()){
            m_realPlayM->playByOrder(info.rtsp,info.cameraId,camera->text(0));
        }
        curCameraMap_[info.cameraId] = info.cameraPos;
    }
}

void RealtimeMonitoring::slotOnCameraGroup(QVector<RestServiceI::CameraGoup> groups)
{
    foreach (const RestServiceI::CameraGoup &groupV, groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeW,QStringList() << groupV.groupName,0);
        item->setData(0,Qt::UserRole,groupV.groupNo);
        item->setData(1,Qt::UserRole + 1,groupV.description);
    }
}

void RealtimeMonitoring::slotOnScenePic(QImage img)
{
    SceneImageDialog dialog;
    dialog.setUserStyle(widgetManger()->currentStyle());
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog.setImage(img);
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
}
