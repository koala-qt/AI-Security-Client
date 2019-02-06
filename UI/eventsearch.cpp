#include "eventsearch.h"
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
#include <QMessageBox>
#include "pageindicator.h"
#include "waitinglabel.h"
#include "service/restservice.h"
#include "sceneimagedialog.h"
#include "facesearch.h"

#pragma execution_character_set("utf-8")
EventSearch::EventSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("事件检索"));
    backImg_.load("images/Mask.png");
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
    hlay->addStretch();
    hlay->addWidget(m_pageindicator);
    hlay->addStretch();
    mainLay->addLayout(hlay);

    mainLay->setContentsMargins(30,30,30,0);
    setLayout(mainLay);

    menu_ = new QMenu(this);
    menu_->addAction(tr("查看场景图"),[&]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnSceneImage(img);
            menu_->setEnabled(true);
        });
        serviceI->getAlarmScenePic(m_tableW->item(m_tableW->currentRow(),1)->text());
        startWorker(worker);
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
    m_tableW->setColumnCount(5);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("抓拍图片") << tr("抓拍ID") << tr("位置") << tr("报警类型") << tr("时间"));
    m_tableW->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    m_tableW->horizontalHeader()->setHighlightSections(false);
    m_tableW->horizontalHeader()->setDefaultSectionSize(112);
    m_tableW->verticalHeader()->setDefaultSectionSize(112);
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->setEditTriggers(QTableWidget::NoEditTriggers);
    m_pageindicator->setPageInfo(0,0);
    QPixmap pix(1,m_waringTyleCombox->iconSize().height());
    pix.fill(Qt::transparent);
    QVector<QPair<QString,QString>>  waringType;
    waringType << qMakePair(tr(""),tr("不限")) << qMakePair(tr("intruder"),tr("闯入报警")) << qMakePair(tr("blacklistface"),tr("黑名单报警"))
               << qMakePair(tr("abdoor"),tr("AB门报警"));
    for(const QPair<QString,QString> &pairValue : waringType) {
        m_waringTyleCombox->addItem(pix,pairValue.second,pairValue.first);
        waryingTypeMap_.insert(pairValue.first,pairValue.second);
    }
    getCameraInfo();
}

void EventSearch::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    QFont f;
    if(s == WidgetManagerI::Danyahei){
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
        pal.setColor(QPalette::Foreground,Qt::white);
        m_positionL->setPalette(pal);
        m_waringTyleL->setPalette(pal);
        m_startTimeL->setPalette(pal);
        m_endTimeL->setPalette(pal);

        m_searchBtn->setStyleSheet("QPushButton{"
                                   "background-color: #B4A06C;"
                                   "color: white;"
                                   //"border: 1px solid #34A2FF;"
                                   "border-radius: 6px;"
                                   "font-size: 18px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "}");
        m_tableW->setStyleSheet(
                    "QTableView{"
                    "color: white;border:1px solid #CECECE;"
                    "font-size: 18px;"
                    "background-color: transparent;"
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

        m_startTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:6px;"
            "background-color: transparent;"
                                       "}");

        m_endTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:6px;"
            "background-color: transparent;"
                                       "}");

//        m_positionCombox->setStyleSheet(
//                    "QComboBoxListView{"
//                    "color: white;"
//                    "background-color: rgb(59,69,78);"
//                    "}"
//                    "QComboBox{"
//                    "color: white;"
//                    "background-color: rgb(59,69,78);"
//                    "}"
//                    "QComboBox QAbstractItemView{"
//                    "selection-color: white;"
//                    "outline: 0px;"
//                    "selection-background-color: rgb(49,54,57);"
//                    "}"
//                    "QScrollBar:vertical{"
//                    "background: rgb(59,69,78);"
//                    "border: 0px solid gray;"
//                    "width: 13px;"
//                    "}"
//                    "QScrollBar::handle:vertical{"
//                    "background: rgb(29,35,39);"
//                    "border-radius: 5px;"
//                    "}"
//                    "QScrollBar::add-line:vertical{"
//                    "background: transparent;"
//                    "border:0px solid #274168;"
//                    "border-radius: 5px;"
//                    "min-height: 10px;"
//                    "width: 13px;"
//                    "}"
//                    "QScrollBar::sub-line:vertical{"
//                    "background: transparent;"
//                    "border:0px solid #274168;"
//                    "border-radius: 5px;"
//                    "min-height: 10px;"
//                    "width: 13px;"
//                    "}"
//                    "QScrollBar::up-arrow:vertical{"
//                    "subcontrol-origin: margin;"
//                    "height: 0px;"
//                    "border:0 0 0 0;"
//                    "visible:false;"
//                    "}"
//                   "QScrollBar::down-arrow:vertical{"
//                   "subcontrol-origin: margin;"
//                   "height: 0px;"
//                   "visible:false;"
//                   "}"
//                   "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
//                   "background: rgb(46,53,54);"
//                   "border: 0px solid gray;"
//                   "}");

        m_positionCombox->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: 1px solid #CECECE;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
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
        m_waringTyleCombox->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: 1px solid #CECECE;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: #CECECE;"
                    "}"
                    "QComboBox::drop-down{"
                    "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                    "}");
        m_pageindicator->setUserStyle();
    }
}

bool EventSearch::event(QEvent *event)
{    if(event->type() == QEvent::Show){
        m_endTimeEdit->setDateTime(QDateTime::currentDateTime());
        slotSearchBtnClicked();
        return true;
    }
    return WidgetI::event(event);
}

void EventSearch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void EventSearch::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void EventSearch::slotSearchPageAlarmHistory(int page)
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        label->close();
        QMessageBox::information(this,objectName(),str);
        m_searchBtn->setEnabled(true);
        m_pageindicator->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigAlarmHistory,this,[this,label](const PagedAlarmHis value){
        label->close();
        slotAlarmHistory(value);
        m_searchBtn->setEnabled(true);
        m_pageindicator->setEnabled(true);
    });
    serviceI->searchAlarmHistory(page,20,curCameraid_,curWaringType_,curStartDateTime_,curEndDateTime_);
    startWorker(worker);
    label->show(500);
    m_searchBtn->setEnabled(false);
    m_pageindicator->setEnabled(false);
}

void EventSearch::slotOnSceneImage(QImage img)
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

void EventSearch::slotAlarmHistory(PagedAlarmHis data)
{
    m_tableW->model()->removeRows(0,m_tableW->rowCount());
    m_pageindicator->adjustRow();
    if(needUpdatePageInfo_){
        m_pageindicator->setPageInfo(data.total_page,data.total_count);
        needUpdatePageInfo_ = false;
    }
    for(const AlarmHis &itemData : data.alarm_his){
        m_tableW->insertRow(m_tableW->rowCount());
        QTableWidgetItem *item = new QTableWidgetItem;
        m_tableW->setItem(m_tableW->rowCount() - 1,0,item);
        QImage img;
        img.loadFromData(QByteArray::fromStdString(itemData.alarm_img));
        QLabel *label = new QLabel;
        label->setScaledContents(true);
        label->setPixmap(QPixmap::fromImage(img));
        m_tableW->setCellWidget(m_tableW->rowCount() - 1,0,label);

        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(itemData.oid));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,1,item);

        item = new QTableWidgetItem;
        item->setText(QString::fromStdString(itemData.camera_pos));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,2,item);

        item = new QTableWidgetItem;
        item->setText(waryingTypeMap_.value(QString::fromStdString(itemData.alarm_type)));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,3,item);

        item = new QTableWidgetItem;
        item->setText(QDateTime::fromMSecsSinceEpoch(itemData.time).toString("yyyy-MM-dd HH:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,4,item);
    }
}
