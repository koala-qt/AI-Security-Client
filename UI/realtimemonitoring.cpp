﻿#include <QTreeWidget>
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
#include <QApplication>
#include <QCompleter>
#include <QScrollBar>
#include <QLineEdit>
#include <QStringListModel>
#include "realtimemonitoring.h"
#include "realplaymanager.h"
#include "cornerwidget.h"
#include "realmonitorsetting.h"
#include "facesearch.h"
#include "waitinglabel.h"
#include "videoplayer.h"
#include "sceneimagedialog.h"
#include "informationdialog.h"

#pragma execution_character_set("utf-8")
RealtimeMonitoring::RealtimeMonitoring( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Real-time surveillance"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hboxLay = new QHBoxLayout;
    m_settingBtn = new QPushButton(tr("设置"));
    hboxLay->addWidget(m_settingBtn);
    hboxLay->setAlignment(Qt::AlignRight);
    mainLay->addLayout(hboxLay);

    QHBoxLayout *centerHboxL = new QHBoxLayout;
    QVBoxLayout *vboxLay = new QVBoxLayout;
    posEdit_ = new QLineEdit;
    m_treeW = new QTreeWidget;
    cameraGoupBackW_ = new QWidget;
    vboxLay->addWidget(posEdit_);
    vboxLay->addWidget(m_treeW);
    cameraGoupBackW_->setLayout(vboxLay);
    // 设置水平滚动条设备字体太长还是显示不完整，自动省略
    m_treeW->headerItem()->setText(0,tr("设备列表"));
    m_treeW->setHeaderHidden(true);
    centerHboxL->addWidget(cameraGoupBackW_,2);

    m_realPlayM = new RealPlayManager();
    m_realPlayM->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    m_faceList = new QListWidget;
    m_faceListL = new QLabel(tr("Face capture"));
    faceCaptureBackW_ = new QWidget;
    vboxLay = new QVBoxLayout;
    vboxLay->addWidget(m_realPlayM,4);
    QVBoxLayout *faceSanLayout = new QVBoxLayout;
    faceSanLayout->addWidget(m_faceListL);
    faceSanLayout->addWidget(m_faceList);
    faceCaptureBackW_->setLayout(faceSanLayout);
    vboxLay->addWidget(faceCaptureBackW_);
    centerHboxL->addLayout(vboxLay,11);

    eventCombox_ = new QComboBox;
    eventList_ = new QListWidget;
    eventBackW_ = new QWidget;
    vboxLay = new QVBoxLayout;
    vboxLay->addWidget(eventCombox_);
    vboxLay->addWidget(eventList_);
    eventBackW_->setLayout(vboxLay);
    centerHboxL->addWidget(eventBackW_,3);

    mainLay->addLayout(centerHboxL);
    setLayout(mainLay);

    faceItemMenu_ = new QMenu(m_faceList);
    faceItemMenu_->addAction(tr("Search using an image"),[&]{
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(userStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(m_faceList->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceDialog->setOid(m_faceList->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    faceItemMenu_->addAction(tr("Scene analysis"),[&]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.showMessage(str);
            infoDialog.exec();
            faceItemMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            faceItemMenu_->setEnabled(true);
        });
        label->show(800);
        faceItemMenu_->setEnabled(false);
        serviceI->getSceneInfo(m_faceList->currentItem()->data(Qt::UserRole + 3).toString());
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
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[this,label](RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            faceItemMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.showMessage(str);
            infoDialog.exec();
            faceItemMenu_->setEnabled(true);
        });
        label->show(800);
        faceItemMenu_->setEnabled(false);
        serviceI->getSceneInfo(eventList_->currentItem()->data(Qt::UserRole + 1).toString());
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
        RealMonitorSetting settingDialog(this, Qt::Dialog | Qt::WindowCloseButtonHint);
        settingDialog.setMinimumWidth(1200);
        settingDialog.setScreenIndex(m_realPlayM->screenCount());
        settingDialog.setUserStyle(userStyle());
        settingDialog.exec();
        int rows = 0, cols = 0, bigRow = 0, bigCol = 0, bigRowSpan = 0, bigColSpan = 0;
        settingDialog.screenSelected(&rows,&cols,&bigRow,&bigCol,&bigRowSpan,&bigColSpan);
        m_realPlayM->splitScreen(rows,cols,bigRow,bigCol,bigRowSpan,bigColSpan);
    });

    posEdit_->setClearButtonEnabled(true);
    QCompleter *cpter = new QCompleter(posEdit_);
    cpter->setFilterMode(Qt::MatchStartsWith);
    cpter->setModelSorting(QCompleter::CaseSensitivelySortedModel);
    posEdit_->setCompleter(cpter);
    posEdit_->setMinimumHeight(36);
    posEdit_->setPlaceholderText(tr("Search"));
    eventCombox_->addItem(tr("All events"));
    eventCombox_->addItem(tr("Intruder events"));
    eventCombox_->addItem(tr("AB-Door evetns"));
    eventCombox_->addItem(tr("Blacklist events"));
    eventList_->setViewMode(QListWidget::IconMode);
    eventList_->setSpacing(10);
    eventList_->setMovement(QListView::Static);
    connect(eventCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotEventComboxIndexChanged(int)));
    notifyServiceI_ = reinterpret_cast<NotifyServiceI*>(qApp->property("NotifyServiceI").toULongLong());
    connect(notifyServiceI_,SIGNAL(sigFaceSnap(NotifyPersonI::FaceSnapEventData)),this,SLOT(slotAddFaceitem(NotifyPersonI::FaceSnapEventData)));
    connect(m_treeW,SIGNAL(itemDoubleClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeItemDoubleClicked(QTreeWidgetItem*,int)));
    m_settingBtn->hide();

    setUserStyle(userStyle());
    updateCamera();
    slotEventComboxIndexChanged(0);
//    getCameraGroup(nullptr,"1005");
}

RealtimeMonitoring::~RealtimeMonitoring()
{
}

void RealtimeMonitoring::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        cameraGoupBackW_->setStyleSheet("QWidget{"
                                        "background-color: rgb(46,52,65);"
                                        "border-radius: 4px;"
                                        "}");
        posEdit_->setStyleSheet("QLineEdit{"
                                "color: white;"
                                "background-color: rgb(40,45,56);"
                                "border-radius: 15px;"
                                "}");
        m_treeW->header()->setStretchLastSection(true);
        m_treeW->header()->setIconSize(QSize(50,50));
        QSize s = m_treeW->headerItem()->sizeHint(0);
        m_treeW->headerItem()->setSizeHint(0,QSize(s.width(),60));
        m_treeW->headerItem()->setTextAlignment(0,Qt::AlignCenter);
        m_treeW->setStyleSheet("QTreeView{"
                               "border:none;"
                               "border-radius:0px;"
                               "font-size: 16px;"
                               "color: #CECECE;"
                               "border-radius: 10px;"
                               "background-color: rgb(46,52,65);}");
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

        faceCaptureBackW_->setStyleSheet("QWidget{"
                                         "background-color: rgb(48,54,68);"
                                         "border-radius: 4px;"
                                         "}");
        m_faceListL->setFont(f);
        m_faceListL->setAlignment(Qt::AlignLeft);
        m_faceListL->setStyleSheet("QLabel{"
                                   "color: rgb(126,140,177);"
                                   "font-size: 16px;"
                                   "border-radius:0px;"
                                   "border: none;}");
        m_faceList->setFlow(QListWidget::LeftToRight);
        m_faceList->setStyleSheet("QListWidget{"
                                  "background-color:transparent;"
                                  "border-radius:0px;"
                                  "border:none;}");

        eventList_->setStyleSheet("QListWidget{"
                                   "background-color:transparent;"
                                   "border-radius:0px;"
                                   "border:none;"
                                   "}");

        eventBackW_->setStyleSheet("QWidget{"
                                   "background-color: rgb(48,54,68);"
                                   "border-radius: 4px;"
                                   "}");
        eventCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: transparent;"
                    "border-radius: 0px;"
                    "border: none;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: none;"
                    "border-radius: 0px;"
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
        faceItemMenu_->setStyleSheet("QMenu{"
                                     "background-color: rgb(75,75,75);"
                                     "}"
                                     "QMenu::item:selected{"
                                     "background-color: rgba(255,255,255,0.4);"
                                     "}");
        eventItemMenu_->setStyleSheet("QMenu{"
                                      "background-color: rgb(75,75,75);"
                                      "}"
                                      "QMenu::item:selected{"
                                      "background-color: rgba(255,255,255,0.4);"
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

bool RealtimeMonitoring::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        int imgItemH = (eventList_->height() - (EVENTITEMCOUNT + 1) * eventList_->spacing() - eventList_->frameWidth() * 2) / EVENTITEMCOUNT;
        int imgItemW = eventList_->width() - 2 * eventList_->spacing() - 2 * eventList_->frameWidth();
        eventItemSize_.setHeight(imgItemH);
        eventItemSize_.setWidth(imgItemW);
        eventList_->setIconSize(eventItemSize_);
        for(int i = 0; i < eventList_->count(); i++){
            QListWidgetItem *item = eventList_->item(i);
            item->setSizeHint(eventItemSize_);
            item->setIcon(QPixmap::fromImage(item->data(Qt::UserRole + 2).value<QImage>().scaled(eventItemSize_)));
        }

        int faceItemW = (m_faceList->width() - (FACEITEMCOUNT + 1) * m_faceList->spacing() - m_faceList->frameWidth() * 2) / FACEITEMCOUNT;
        int faceItemH = m_faceList->height() - m_faceList->spacing() * 2 - m_faceList->frameWidth() * 2;
        m_faceItemSize = QSize(faceItemW,faceItemH);
        for(int i = 0; i < m_faceList->count(); i++){
            QListWidgetItem *item = m_faceList->item(i);
            item->setSizeHint(m_faceItemSize);
        }
    }

    return WidgetI::event(event);
}

void RealtimeMonitoring::updateCamera()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotAddDevice(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void RealtimeMonitoring::getCameraGroup(QTreeWidgetItem* item,QString groupNo)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
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
}

void RealtimeMonitoring::getCameraDevice(QTreeWidgetItem *item, QString groupNo)
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
            if(!camera->data(0,Qt::UserRole + 2).toString().isEmpty()){
                m_realPlayM->playByOrder(info.rtsp,info.cameraId,camera->text(0));
            }
        }
    });
    serviceI->getCameraDevice(groupNo);
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
    disconnect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)));
    if(index == 0){
        connect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)),Qt::UniqueConnection);
    }else if(index == 1){
        connect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
    }else if(index == 2){
        connect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)),Qt::UniqueConnection);
    }else if(index == 3){
        connect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)),Qt::UniqueConnection);
    }
}

void RealtimeMonitoring::slotTreeItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if(item->type() == 0){
        return;
    }
    if(!m_realPlayM->focusPlayer()){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.showMessage("Please selecte a window");
        infoDialog.exec();
        return;
    }

    QString cameraId = item->data(column,Qt::UserRole + 1).toString();
    if(cameraId.isEmpty()){
        return;
    }

    m_realPlayM->playByFocus(item->data(column,Qt::UserRole + 2).toString(),item->data(column,Qt::UserRole + 1).toString(),item->data(column,Qt::UserRole + 3).toString());
}

void RealtimeMonitoring::slotAddFaceitem(NotifyPersonI::FaceSnapEventData faceEvData)
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
    label->setPixmap(QPixmap::fromImage(faceEvData.faceImg));
    vboxLay->addWidget(label);

    label = new QLabel(faceEvData.cameraPos);
    QPalette pal = label->palette();
    pal.setColor(QPalette::Foreground,Qt::white);
    label->setPalette(pal);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setStyleSheet(faceItemStyleSheet_);
    vboxLay->addWidget(label);

    label = new QLabel(faceEvData.snapTime.toString("yyyy-MM-dd HH:mm:ss"));
    pal = label->palette();
    pal.setColor(QPalette::Foreground,Qt::white);
    label->setPalette(pal);
    label->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    label->setStyleSheet(faceItemStyleSheet_);
    vboxLay->addWidget(label);
    itemWidget->setLayout(vboxLay);

    QListWidgetItem *item = new QListWidgetItem;
    item->setData(Qt::UserRole + 1,faceEvData.faceImg);
    item->setData(Qt::UserRole + 2,faceEvData.sceneId);
    item->setData(Qt::UserRole + 3,faceEvData.sceneId);
    item->setSizeHint(m_faceItemSize);
    m_faceList->insertItem(0,item);
    m_faceList->setItemWidget(item,itemWidget);
}

void RealtimeMonitoring::slotOnIntruderEvent(NotifyEventI::IntruderEventData evData)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    QPainter p(&evData.sceneImg);
    p.setBrush(QColor(200,0,0,100));
    p.drawPolygon(evData.warnZone);
    item->setIcon(QPixmap::fromImage(evData.sceneImg).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setData(Qt::UserRole + 2,evData.sceneImg);
    item->setSizeHint(eventItemSize_);
    eventList_->insertItem(0,item);
}

void RealtimeMonitoring::slotOnPersonEvent(NotifyEventI::PersonEventData evData)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    item->setIcon(QPixmap::fromImage(evData.image).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventList_->insertItem(0,item);
}

void RealtimeMonitoring::slotOnAbDoorEvent(NotifyEventI::ABDoorEventData evData)
{
    if(eventList_->count() >= EVENTITEMCOUNT){
        delete eventList_->takeItem(EVENTITEMCOUNT - 1);
    }
    QListWidgetItem *item = new QListWidgetItem;
    QPainter p(&evData.sceneImg);
    p.setBrush(QColor(100,0,100,100));
    p.drawPolygon(evData.warnZone);
    item->setIcon(QPixmap::fromImage(evData.sceneImg).scaled(eventItemSize_));
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setData(Qt::UserRole + 2,evData.sceneImg);
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
        cameraStringList_ << info.cameraPos;
    }
    QStringListModel *listMode = new QStringListModel;
    listMode->setStringList(cameraStringList_);
    posEdit_->completer()->setModel(listMode);
}

void RealtimeMonitoring::slotOnCameraGroup(QVector<RestServiceI::CameraGoup> groups)
{
    foreach (const RestServiceI::CameraGoup &groupV, groups) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_treeW,QStringList() << groupV.groupName,0);
        item->setData(0,Qt::UserRole,groupV.groupNo);
        item->setData(1,Qt::UserRole + 1,groupV.description);
    }
}

void RealtimeMonitoring::slotOnSceneInfo(RestServiceI::SceneInfo sinfo)
{
    SceneImageDialog dialog;
    dialog.setUserStyle(userStyle());
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog.setSceneInfo(sinfo);
    dialog.setRectLinePen(Qt::yellow);
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
        if(!images.count()){
            return;
        }
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(userStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(images.first());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    dialog.exec();
}
