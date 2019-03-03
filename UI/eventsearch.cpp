﻿#include "eventsearch.h"
#include <QLabel>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDialog>
#include <QMenu>
#include <QTimer>
#include <QListView>
#include <QPainter>
#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facesearch.h"
#include "informationdialog.h"
#include "nodatatip.h"

#pragma execution_character_set("utf-8")
EventSearch::EventSearch( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("事件检索"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(15);
    m_positionL = new QLabel(tr("区域位置"));
    m_positionCombox = new QComboBox;
    m_positionCombox->setMinimumSize(90,40);
    m_positionCombox->setIconSize(QSize(1,30));
    m_positionCombox->setFixedHeight(40);
    m_positionCombox->setFocusPolicy(Qt::NoFocus);
    m_waringTyleL = new QLabel(tr("报警类型"));
    m_waringTyleCombox = new QComboBox;
    m_waringTyleCombox->setIconSize(QSize(1,30));
    m_waringTyleCombox->setFixedHeight(40);
    m_waringTyleCombox->setFocusPolicy(Qt::NoFocus);
    m_startTimeL = new QLabel(tr("开始时间"));
    m_startTimeEdit = new QDateTimeEdit;
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_startTimeEdit->setDate(QDateTime::currentDateTime().addDays(-1).date());
    m_startTimeEdit->setMinimumSize(160,40);
    m_endTimeL = new QLabel(tr("结束时间"));
    m_endTimeEdit = new QDateTimeEdit;
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_endTimeEdit->setDate(QDateTime::currentDateTime().addDays(1).date());
    m_endTimeEdit->setMinimumSize(160,40);
    m_searchBtn = new QPushButton(tr("检索"));
    m_searchBtn->setFixedSize(90,40);
    hlay->addWidget(m_positionL);
    hlay->addWidget(m_positionCombox);
    hlay->addWidget(m_waringTyleL);
    hlay->addWidget(m_waringTyleCombox);
    hlay->addWidget(m_startTimeL);
    hlay->addWidget(m_startTimeEdit);
    hlay->addWidget(m_endTimeL);
    hlay->addWidget(m_endTimeEdit);
    hlay->addWidget(m_searchBtn);
    hlay->addStretch();
    mainLay->addLayout(hlay);

    m_tableW = new QTableWidget;
    mainLay->addWidget(m_tableW);

    m_pageindicator = new PageIndicator;
    hlay = new QHBoxLayout;
    hlay->addWidget(m_pageindicator);
    hlay->setAlignment(Qt::AlignRight);
    mainLay->setSpacing(20);
    mainLay->addLayout(hlay);

    centerBack_ = new QWidget;
    mainLay->setContentsMargins(40,40,40,40);
    centerBack_->setLayout(mainLay);
    hlay = new QHBoxLayout;
    hlay->addWidget(centerBack_);
    hlay->setContentsMargins(40,40,40,40);
    setLayout(hlay);

    menu_ = new QMenu(this);
    menu_->addAction(tr("查看场景图"),[&]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            menu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            menu_->setEnabled(true);
        });
        serviceI->getSceneInfo(m_tableW->item(m_tableW->currentRow(),1)->text());
        label->show(500);
        menu_->setEnabled(false);
    });
    m_tableW->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableW,&QTableWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!m_tableW->itemAt(p))return;
        menu_->move(QCursor::pos());
        menu_->show();
    });

    connect(m_searchBtn,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(m_pageindicator,SIGNAL(sigPageClicked(int)),this,SLOT(slotSearchPageAlarmHistory(int)));
    centerBack_->installEventFilter(this);
    m_tableW->setColumnCount(5);
    m_tableW->setFrameStyle(QFrame::NoFrame);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("抓拍图片") << tr("抓拍ID") << tr("位置") << tr("报警类型") << tr("时间"));
    m_tableW->horizontalHeader()->setMinimumHeight(60);
    m_tableW->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    m_tableW->horizontalHeader()->setHighlightSections(false);
    m_tableW->horizontalHeader()->setDefaultSectionSize(192);
    m_tableW->verticalHeader()->setDefaultSectionSize(108);
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tableW->hideColumn(1);
    m_tableW->setShowGrid(false);
    m_pageindicator->setPageInfo(0,0);
    QPixmap pix(1,m_waringTyleCombox->iconSize().height());
    pix.fill(Qt::transparent);
    QVector<QPair<QString,QString>>  waringType;
    waringType << qMakePair(tr(""),tr("不限"))
               << qMakePair(tr("smsr_alarm_intruder"),tr("闯入报警"))
               << qMakePair(tr("smsr_alarm_abdoor"),tr("AB-Door events"))
               << qMakePair(tr("smsr_alarm_climb"),tr("Climb events"))
               << qMakePair(tr("smsr_alarm_gather"),tr("Gather events"))
               << qMakePair(tr("smsr_alarm_face"),tr("黑名单报警"));
    for(const QPair<QString,QString> &pairValue : waringType) {
        m_waringTyleCombox->addItem(pix,pairValue.second,pairValue.first);
        waryingTypeMap_.insert(pairValue.first,pairValue.second);
    }
    noDataTipW_ = new NoDataTip(m_tableW);
    setUserStyle(userStyle());
    getCameraInfo();

    QSettings configSeting("config.ini",QSettings::IniFormat);
    javaHost_ = configSeting.value("Http/Javahost").toString();
}

void EventSearch::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        f = font();
        f.setFamily("Arial");
        setFont(f);
        f = m_positionL->font();
        f.setPixelSize(18);
        m_positionL->setFont(f);
        m_waringTyleL->setFont(f);
        m_startTimeL->setFont(f);
        m_endTimeL->setFont(f);

        pal = m_positionL->palette();
        pal.setColor(QPalette::Foreground,QColor(126,140,177));
        m_positionL->setPalette(pal);
        m_waringTyleL->setPalette(pal);
        m_startTimeL->setPalette(pal);
        m_endTimeL->setPalette(pal);

        m_searchBtn->setStyleSheet("QPushButton{"
                                   "background-color: rgb(83,77,251);"
                                   "color: white;"
                                   "border-radius: 6px;"
                                   "font-size: 18px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "background-color: #312DA6;"
                                   "}");
        m_tableW->setStyleSheet("QTableView{"
                                "selection-background-color: #383F4F;"
                                "background-color: #383F4F;"
                                "color: #7E8CB1;"
                                "}"
                                "QTableView QTableCornerButton::section{"
                                "background: rgb(65,73,92);"
                                "}"
                                "QTableWidget::Item{"
                                "border-bottom: 1px solid rgb(62,69,84);"
                                "}"
                                "QTableWidget::item:selected{"
                                "background-color: rgb(43,49,61);"
                                "color: white;"
                                "}"
                                "QHeaderView{"
                                "background-color: rgb(65,73,92);"
                                "}"
                                "QHeaderView::section{"
                                "color: rgb(126,140,177);"
                                "background-color: transparent;"
                                "border: none;"
                                "border-radius: 0px;"
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

        m_startTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                       "color: rgb(126,140,177);"
                                       "box-shadow:1px 1px 0px rgba(77,86,107,1);"
                                       "border-radius:6px;"
                                       "background-color: rgb(40,45,56);}");

        m_endTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                     "color: rgb(126,140,177);"
                                     "box-shadow:1px 1px 0px rgba(77,86,107,1);"
                                     "border-radius:6px;"
                                     "background-color: rgb(40,45,56);}");

        m_positionCombox->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: rgb(43,49,61);"
                    "}"
                    "QComboBox{"
                    "color: rgb(126,140,177);"
                    "font-size: 18px;"
                    "background-color: rgb(40,45,56);"
                    "box-shadow: 1px 1px 0px rgba(77,86,107,1);"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: rgb(71,65,242);"
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
        m_waringTyleCombox->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: rgb(43,49,61);"
                    "}"
                    "QComboBox{"
                    "color: rgb(126,140,177);"
                    "font-size: 18px;"
                    "background-color: rgb(40,45,56);"
                    "box-shadow:1px 1px 0px rgba(77,86,107,1);"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: rgb(71,65,242);"
                    "}"
                    "QComboBox::drop-down{"
                    "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                    "}");
        menu_->setStyleSheet("QMenu{"
                             "background-color: rgb(75,75,75);"
                             "}"
                             "QMenu::item:selected{"
                             "background-color: rgba(255,255,255,0.4);"
                             "}");
        m_pageindicator->setUserStyle();
        noDataTipW_->setUserStyle(s);
    }
}

bool EventSearch::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchWid = qobject_cast<QWidget*>(watched);
    if(watchWid == centerBack_ && event->type() == QEvent::Paint){
        QPainter p(watchWid);
        if(userStyle() == 0){
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(48,54,68));
            p.drawRoundedRect(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
        }
    }
    return WidgetI::eventFilter(watched,event);
}

bool EventSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show && m_searchBtn->isEnabled()){
        m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
        slotSearchBtnClicked();
        return true;
    }
    return WidgetI::event(event);
}

void EventSearch::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void EventSearch::slotSearchPageAlarmHistory(int page)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        label->close();
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        m_searchBtn->setEnabled(true);
        m_pageindicator->setEnabled(true);
        noDataTipW_->hide();
    });
    connect(serviceI,&RestServiceI::sigAlarmHistory,this,[this,label](const RestServiceI::EventSearchReturn value){
        label->close();
        slotAlarmHistory(value);
        m_searchBtn->setEnabled(true);
        m_pageindicator->setEnabled(true);
    });
    RestServiceI::EventSearchArgs args;
    args.pageNo = page;
    args.pageSize = 20;
    args.cameraId = curCameraid_;
    args.alarmType = curWaringType_;
    args.start = curStartDateTime_;
    args.end = curEndDateTime_;
    serviceI->searchAlarmHistory(args);
    label->show(500);
    m_searchBtn->setEnabled(false);
    m_pageindicator->setEnabled(false);
    noDataTipW_->hide();
}

void EventSearch::slotOnSceneInfo(RestServiceI::SceneInfo sinfo)
{
#if 0
    QDialog dialog;
    QLabel *label = new QLabel;
    QHBoxLayout *lay = new QHBoxLayout;
    lay->addWidget(label);
    dialog.setLayout(lay);
    dialog.setModal(true);

    label->setScaledContents(true);
    label->setPixmap(QPixmap::fromImage(img));
    dialog.setFixedSize(960,540);
    dialog.exec();
#else
    SceneImageDialog dialog;
    dialog.setUserStyle(userStyle());
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog.setSceneInfo(sinfo);
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
#endif
}

void EventSearch::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    m_positionCombox->clear();
    QPixmap pix(m_positionCombox->iconSize());
    pix.fill(Qt::transparent);
    m_positionCombox->addItem(tr("不限"),"");
    for (auto &info : data) {
        m_positionCombox->addItem(info.cameraPos,info.cameraId);
    }
    installEventFilter(this);
}

void EventSearch::slotSearchBtnClicked()
{
    curCameraid_ = m_positionCombox->currentData().toString();
    curWaringType_ = m_waringTyleCombox->currentData().toString();
    curStartDateTime_ = m_startTimeEdit->dateTime();
    curEndDateTime_ = m_endTimeEdit->dateTime();
    needUpdatePageInfo_ = true;
    slotSearchPageAlarmHistory(1);
}

void EventSearch::slotAlarmHistory(RestServiceI::EventSearchReturn data)
{
    m_pageindicator->adjustRow();
    if(needUpdatePageInfo_){
        m_pageindicator->setPageInfo(data.totalPage,data.total);
        needUpdatePageInfo_ = false;
    }
    m_tableW->model()->removeRows(0,m_tableW->rowCount());
    for(const RestServiceI::EventSearchItem &itemData : data.data){
        m_tableW->insertRow(m_tableW->rowCount());
        QTableWidgetItem *item = new QTableWidgetItem;
        m_tableW->setItem(m_tableW->rowCount() - 1,0,item);
        QLabel *label = new QLabel;
        label->setScaledContents(true);
        label->setPixmap(QPixmap::fromImage(itemData.image));
        m_tableW->setCellWidget(m_tableW->rowCount() - 1,0,label);

        item = new QTableWidgetItem;
        item->setText(itemData.sceneId);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,1,item);

        item = new QTableWidgetItem;
        item->setText(itemData.sourceName);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,2,item);

        item = new QTableWidgetItem;
        item->setText(itemData.eventType);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,3,item);

        item = new QTableWidgetItem;
        item->setText(itemData.timeStamp.toString("yyyy-MM-dd HH:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,4,item);
    }
    if(!data.total){
        noDataTipW_->show();
    }
}
