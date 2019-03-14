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
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
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
    setObjectName(tr("Event"));
    backImg_.load(tr("images/Mask.png"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->setSpacing(15);
    m_positionL = new QLabel(tr("Regional Location"));
    m_positionCombox = new QComboBox;
    m_positionCombox->setMinimumSize(90,40);
    m_positionCombox->setIconSize(QSize(1,30));
    m_positionCombox->setFixedHeight(40);
    m_positionCombox->setFocusPolicy(Qt::NoFocus);
    m_waringTyleL = new QLabel(tr("Alarm Type "));
    waringTypeMenu_ = new QMenu();
    waringTypeBtn_ = new QPushButton;
    m_startTimeL = new QLabel(tr("Starting Time"));
    m_startTimeEdit = new QDateTimeEdit;
    m_startTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_startTimeEdit->setDate(QDateTime::currentDateTime().addDays(-1).date());
    m_startTimeEdit->setMinimumSize(160,40);
    m_endTimeL = new QLabel(tr("Ending Time"));
    m_endTimeEdit = new QDateTimeEdit;
    m_endTimeEdit->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    m_endTimeEdit->setDate(QDateTime::currentDateTime().addDays(1).date());
    m_endTimeEdit->setMinimumSize(160,40);
    m_searchBtn = new QPushButton(tr("Search"));
    m_searchBtn->setFixedSize(90,40);
    hlay->addWidget(m_positionL);
    hlay->addWidget(m_positionCombox);
    hlay->addWidget(m_waringTyleL);
    hlay->addWidget(waringTypeBtn_);
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
        int curRowIndex = m_tableW->currentRow();
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            menu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label,curRowIndex](RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            QPainter p(&sinfo.image);
            p.setBrush(waringColorMap_.value(m_tableW->item(curRowIndex,3)->text()));
            p.setPen(Qt::NoPen);
            p.setRenderHint(QPainter::Antialiasing);
            p.drawPolygon(m_tableW->item(curRowIndex,0)->data(Qt::UserRole).value<QPolygonF>());
            p.end();
            slotOnSceneInfo(sinfo);
            menu_->setEnabled(true);
        });
        serviceI->getSceneInfo(m_tableW->item(curRowIndex,1)->text(),m_tableW->item(curRowIndex,0)->data(Qt::UserRole + 1).toString());
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
    m_tableW->verticalHeader()->hide();
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->setEditTriggers(QTableWidget::NoEditTriggers);
    m_tableW->hideColumn(1);
    m_tableW->setShowGrid(false);
    m_pageindicator->setPageInfo(0,0);

#if 0
    QVector<std::tuple<QString,QString,QColor>> waringTypeTupleVec;
    waringTypeTupleVec << std::make_tuple(tr(""),tr("Unlimited"),Qt::transparent)
                       << std::make_tuple(tr("smsr_alarm_intruder"),tr("Intrusion Alarm"),QColor(200,0,0,100))
                       << std::make_tuple(tr("smsr_alarm_abdoor"),tr("Trailing Alarm"),QColor(0,200,0,100))
                       << std::make_tuple(tr("smsr_alarm_climb"),tr("Climbing Alarm"),QColor(100,100,0,100))
                       << std::make_tuple(tr("smsr_alarm_gather"),tr("Aggregate Alarm"),QColor(100,0,100,100))
                       << std::make_tuple(tr("smsr_face_gather"),tr("Blacklist Alarm"),Qt::transparent);
    for(int i = 0; i < waringTypeTupleVec.count(); i++) {
        if(i == waringTypeTupleVec.count() - 1){
            QMenu *personTypeMenu = new QMenu(std::get<1>(waringTypeTupleVec.at(i)));
            connect(personTypeMenu,&QMenu::aboutToShow,personTypeMenu,[this,personTypeMenu]{
                ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
                RestServiceI *serviceI = factoryI->makeRestServiceI();
                connect(serviceI,&RestServiceI::sigPersonTypesResult,personTypeMenu,[this,personTypeMenu](QVector<RestServiceI::PersonType> data){
                    for(RestServiceI::PersonType &value : data){
                        QMenu *typeMenu = new QMenu(value.strTypeName);
                        typeMenu->setProperty("groupNo",value.groupNo);
                        typeMenu->setProperty("personType",value.strTypeNo);
                        personTypeMenu->addMenu(typeMenu);
                        connect(typeMenu,&QMenu::aboutToShow,typeMenu,[typeMenu,this]{
                            ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
                            RestServiceI *serviceI = factoryI->makeRestServiceI();
                            connect(serviceI,&RestServiceI::sigPersonGroupInfos,typeMenu,[typeMenu,this](QVector<RestServiceI::PersonGroupInfo> infos){
                                QVector<PersonGroup> dataVec;
                                for(RestServiceI::PersonGroupInfo &value : infos){
                                    parseGroupToVec(value,dataVec);
                                }
                                for(int i = 0; i < dataVec.count(); i++){
                                    QAction *act = new QAction(dataVec.at(i).name);
                                    act->setData(dataVec.at(i).no);
                                    connect(act,&QAction::triggered,act,[this,act,typeMenu]{
                                        waringTypeBtn_->setProperty("personType",typeMenu->property("personType").toString());
                                        waringTypeBtn_->setProperty("groupNo",act->data().toString());
                                        waringTypeBtn_->setProperty("alarm type","");
                                    });
                                    typeMenu->addAction(act);
                                }
                            });
                            connect(serviceI,&RestServiceI::sigError,typeMenu,[typeMenu](QString str){
                                qDebug() << str;
                            });
                            serviceI->getPersonGoupInfos(typeMenu->property("groupNo").toString());
                        });
                        connect(typeMenu,&QMenu::aboutToHide,typeMenu,[typeMenu]{typeMenu->clear();});
                    }
                });
                connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
                    qDebug() << str;
                });
                serviceI->queryPersonTypes();
            });
            connect(personTypeMenu,&QMenu::aboutToHide,this,[this,personTypeMenu]{
                personTypeMenu->clear();
            });
            waringTypeMenu_->addMenu(personTypeMenu);
        }else{
            QAction *acti = new QAction(std::get<1>(waringTypeTupleVec.at(i)));
            if(i == 0){
                waringTypeBtn_->setText(acti->text());
            }
            acti->setData(std::get<0>(waringTypeTupleVec.at(i)));
            connect(acti,&QAction::triggered,waringTypeBtn_,[acti,this]{
                waringTypeBtn_->setText(acti->text());
                waringTypeBtn_->setProperty("alarm type",acti->data().toString());
                waringTypeBtn_->setProperty("groupNo","");
                waringTypeBtn_->setProperty("personType","");
            });
            waringTypeMenu_->addAction(acti);
        }
    }
#else
    QVector<std::tuple<QString,QString,QColor>> waringTypeTupleVec;
    waringTypeTupleVec << std::make_tuple(tr(""),tr("Unlimited"),Qt::transparent)
                       << std::make_tuple(tr("smsr_alarm_intruder"),tr("Intrusion"),QColor(200,0,0,100))
                       << std::make_tuple(tr("smsr_alarm_abdoor"),tr("Trailing"),QColor(0,200,0,100))
                       << std::make_tuple(tr("smsr_alarm_climb"),tr("Climbing"),QColor(100,100,0,100))
                       << std::make_tuple(tr("smsr_alarm_gather"),tr("Gathering"),Qt::transparent);
    for(int i = 0; i < waringTypeTupleVec.count(); i++){
        QString alarmType = std::get<0>(waringTypeTupleVec.at(i));
        waringColorMap_.insert(std::get<1>(waringTypeTupleVec.at(i)),std::get<2>(waringTypeTupleVec.at(i)));
        originalEventMap_.insert(std::get<0>(waringTypeTupleVec.at(i)),std::get<1>(waringTypeTupleVec.at(i)));
        QAction *act = new QAction(std::get<1>(waringTypeTupleVec.at(i)));
        connect(act,&QAction::triggered,waringTypeMenu_,[this,alarmType,act]{
            waringTypeBtn_->setText(act->text());
            waringTypeBtn_->setProperty("alarm type",alarmType);
            waringTypeBtn_->setProperty("groupNo","");
            waringTypeBtn_->setProperty("personType","");
        });
        waringTypeMenu_->addAction(act);
        if(i == 0){
            waringTypeBtn_->setText(act->text());
        }
    }

    QVector<std::tuple<QString,QString,QString,QColor>> subTypeAlarmVec;
    subTypeAlarmVec << std::make_tuple(tr("smsr_alarm_black"),tr("Blacklist"),tr("100010001008"),Qt::transparent)
                    << std::make_tuple(tr("smsr_alarm_vip"),tr("VIP"),tr("100010001007"),Qt::transparent);
    for(int i = 0; i < subTypeAlarmVec.count(); i++){
        QString personTypeNo = std::get<2>(subTypeAlarmVec.at(i));
        waringColorMap_.insert(std::get<1>(subTypeAlarmVec.at(i)),std::get<3>(subTypeAlarmVec.at(i)));
        faceAlarmMap_.insert(std::get<2>(subTypeAlarmVec.at(i)),std::get<1>(subTypeAlarmVec.at(i)));
        QMenu *typeMenu = new QMenu(std::get<1>(subTypeAlarmVec.at(i)));
        typeMenu->setProperty("personType",personTypeNo);
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigPersonTypeDetailes,typeMenu,[typeMenu,this](RestServiceI::PersonTypeDetail data){
            typeMenu->setProperty("groupNo",data.group);
            connect(typeMenu,&QMenu::aboutToShow,typeMenu,[typeMenu,this]{
                ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
                RestServiceI *serviceI = factoryI->makeRestServiceI();
                connect(serviceI,&RestServiceI::sigPersonGroupInfos,typeMenu,[typeMenu,this](QVector<RestServiceI::PersonGroupInfo> infos){
                    QAction *act = new QAction(tr("All"));
                    act->setData("");
                    connect(act,&QAction::triggered,act,[this,act,typeMenu]{
                        waringTypeBtn_->setProperty("personType",typeMenu->property("personType").toString());
                        waringTypeBtn_->setProperty("groupNo","");
                        waringTypeBtn_->setProperty("alarm type","");
                        waringTypeBtn_->setText(typeMenu->title());
                    });
                    typeMenu->addAction(act);

                    QVector<PersonGroup> dataVec;
                    for(RestServiceI::PersonGroupInfo &value : infos){
                        parseGroupToVec(value,dataVec);
                    }
                    for(int i = 0; i < dataVec.count(); i++){
                        QAction *act = new QAction(dataVec.at(i).name);
                        act->setData(dataVec.at(i).no);
                        connect(act,&QAction::triggered,act,[this,act,typeMenu]{
                            waringTypeBtn_->setProperty("personType",typeMenu->property("personType").toString());
                            waringTypeBtn_->setProperty("groupNo",act->data().toString());
                            waringTypeBtn_->setProperty("alarm type","");
                            waringTypeBtn_->setText(typeMenu->title() + ": " + act->text());
                        });
                        typeMenu->addAction(act);
                    }
                });
                connect(serviceI,&RestServiceI::sigError,typeMenu,[typeMenu,this](QString str){
                    qDebug() << str;
                });
                serviceI->getPersonGoupInfos(typeMenu->property("groupNo").toString());
            });
            connect(typeMenu,&QMenu::aboutToHide,typeMenu,[typeMenu]{typeMenu->clear();});
        });
        connect(serviceI,&RestServiceI::sigError,typeMenu,[typeMenu,this](QString str){
            qDebug() << str;
        });
        waringTypeMenu_->addMenu(typeMenu);
        serviceI->searchPesonTypeDetail(personTypeNo);
    }
#endif
    waringTypeBtn_->setMenu(waringTypeMenu_);
    setUserStyle(userStyle());
    noDataTipW_ = new NoDataTip(m_tableW);
    getCameraInfo();

    QSettings configSeting("config.ini",QSettings::IniFormat);
    javaHost_ = configSeting.value("Http/Javahost").toString();
}

void EventSearch::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        f = m_positionL->font();
        f.setPixelSize(14);
        m_positionL->setFont(f);
        m_waringTyleL->setFont(f);
        m_startTimeL->setFont(f);
        m_endTimeL->setFont(f);
        m_positionCombox->setFont(f);
        waringTypeBtn_->setFont(f);
        m_startTimeEdit->setFont(f);
        m_endTimeEdit->setFont(f);
        m_searchBtn->setFont(f);

        pal = m_positionL->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255,191));
        m_positionL->setPalette(pal);
        m_waringTyleL->setPalette(pal);
        m_startTimeL->setPalette(pal);
        m_endTimeL->setPalette(pal);

        waringTypeBtn_->setFixedSize(200,34);
        waringTypeMenu_->setMinimumSize(waringTypeBtn_->size());
        m_searchBtn->setFixedSize(99,34);
        m_startTimeEdit->setFixedSize(200,34);
        m_endTimeEdit->setFixedSize(200,34);
        m_positionCombox->setFixedSize(200,34);
        m_searchBtn->setStyleSheet("QPushButton{"
                                   "background-color: rgb(83,77,251);"
                                   "color: white;"
                                   "border-radius: 6px;"
                                   "width: 99px;"
                                   "height: 34px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "background-color: #312DA6;"
                                   "}");

        waringTypeBtn_->setStyleSheet("QPushButton{"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "color: rgba(255,255,255,0.75);"
                                      "border-radius: 6px;"
                                      "width: 99px;"
                                      "height: 34px;"
                                      "text-align: center left;"
                                      "padding-left: 10px;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "padding: 2px;"
                                      "background-color: #312DA6;"
                                      "text-align: center left;"
                                      "padding-left: 10px;"
                                      "}"
                                      "QPushButton::menu-indicator{"
                                      "subcontrol-position: center right;"
                                      "subcontrol-origin: padding;"
                                      "margin-right:5px;"
                                      "}");

        m_tableW->setStyleSheet("QTableView{"
                                "selection-background-color: #383F4F;"
                                "background-color: #383F4F;"
                                "color: rgba(255,255,255,191);"
                                "font-size: 14px;"
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
                                "font-size: 14px;"
                                "font-weight: bold;"
                                "}"
                                "QHeaderView::section{"
                                "color: rgba(255,255,255,191);"
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

        m_positionCombox->setStyleSheet("QComboBoxListView{"
                                        "color: #CECECE;"
                                        "background-color: transparent;"
                                        "border-radius: 0px;"
                                        "border: none;"
                                        "}"
                                        "QComboBox{"
                                        "color: rgba(255,255,255,0.75);"
                                        "font-size: 14px;"
                                        "background-color: rgba(255,255,255,0.1);"
                                        "border: none;"
                                        "border-radius: 4px;"
                                        "padding-left: 10px;"
                                        "}"
                                        "QComboBox QAbstractItemView{"
                                        "background-color: rgb(43,49,61);"
                                        "border-radius: 0px;"
                                        "selection-color: white;"
                                        "outline: 0px;"
                                        "selection-background-color: rgba(255,255,255,0.1);"
                                        "}"
                                        "QComboBox::drop-down{"
                                        "subcontrol-position: center right;"
                                        "border-image: url(images/dropdown2.png);"
                                        "width:11px;"
                                        "height:8px;"
                                        "subcontrol-origin: padding;"
                                        "margin-right:5px;"
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
                                       "color: rgba(255,255,255,191);"
                                       "border-radius:4px;"
                                       "background-color: rgba(255,255,255,0.1);"
                                       "padding-left: 10px;"
                                       "}");
        m_endTimeEdit->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                     "color: rgba(255,255,255,191);"
                                     "border-radius:4px;"
                                     "background-color: rgba(255,255,255,0.1);"
                                     "padding-left: 10px;"
                                     "}");
        m_pageindicator->setUserStyle();
        noDataTipW_->setUserStyle(s);
    }
}

void EventSearch::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.drawImage(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),backImg_);
}

bool EventSearch::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchWid = qobject_cast<QWidget*>(watched);
    if(watchWid == centerBack_ && event->type() == QEvent::Paint){
        QPainter p(watchWid);
        if(userStyle() == 0){
            p.setPen(Qt::NoPen);
            p.setBrush(QColor(0,0,0,102));
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

void EventSearch::parseGroupToVec(RestServiceI::PersonGroupInfo &datas, QVector<PersonGroup> &Vec)
{
    PersonGroup pdata;
    pdata.no = datas.no;
    pdata.name = datas.name;
    pdata.id = datas.id;
    pdata.description = datas.description;
    Vec << pdata;
    for(RestServiceI::PersonGroupInfo &value : datas.children){
        parseGroupToVec(value,Vec);
    }
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
    args.personType = curPersonType_;
    args.groupNo = curGroupNo_;
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
    dialog.setSceneInfo(sinfo);
    dialog.setShowRect(true,true);
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
        if(!images.count()){
            return;
        }
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
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
    curWaringType_ = waringTypeBtn_->property("alarm type").toString();
    curPersonType_ = waringTypeBtn_->property("personType").toString();
    curGroupNo_ = waringTypeBtn_->property("groupNo").toString();
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
    for(RestServiceI::EventSearchItem &itemData : data.data){
        m_tableW->insertRow(m_tableW->rowCount());

        QString eventStr;
        if(itemData.eventType == "smsr_alarm_face"){
            eventStr = faceAlarmMap_.value(itemData.persontypNo);
        }else{
            eventStr = originalEventMap_.value(itemData.eventType);
        }
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(Qt::UserRole,itemData.warnZong);
        item->setData(Qt::UserRole + 1,itemData.bodyId);
        m_tableW->setItem(m_tableW->rowCount() - 1,0,item);
        QLabel *label = new QLabel;
        label->setScaledContents(true);
        QPainter p(&itemData.image);
        p.setBrush(waringColorMap_.value(eventStr));
        p.setPen(Qt::NoPen);
        p.drawPolygon(itemData.warnZong);
        p.end();
        label->setPixmap(QPixmap::fromImage(itemData.image));
        label->setContentsMargins(20,10,0,10);
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
        item->setText(eventStr);
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
