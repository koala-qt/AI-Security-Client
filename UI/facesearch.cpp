#include "facesearch.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QHeaderView>
#include <QFileDialog>
#include <QApplication>
#include <QStandardPaths>
#include <QScrollBar>
#include <QSpinBox>
#include <QMenu>
#include <QDebug>
#include <QListView>
#include <QEvent>
#include <QMessageBox>
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "informationdialog.h"
#include "nodatatip.h"
#include "windowtitlebar.h"

#pragma execution_character_set("utf-8")
FaceSearch::FaceSearch(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Single Face"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    m_imgBtn = new QPushButton;
    m_imgBtn->setToolTip(tr("添加图片"));
    m_imgBtn->setFocusPolicy(Qt::NoFocus);
    QPixmap imgPix("images/person-face-back.png");
    m_imgBtn->setFixedSize(imgPix.size());
    m_imgBtn->setIconSize(m_imgBtn->size());
    m_imgBtn->setProperty("default-pix",imgPix);
    QCursor curSor = cursor();
    curSor.setShape(Qt::PointingHandCursor);
    m_imgBtn->setCursor(curSor);
    hlay->addWidget(m_imgBtn);

    QGridLayout *gridLay = new QGridLayout;
    cameraLabel_ = new QLabel(tr("Position"));
    cameraLabel_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cameraCombox_ = new QComboBox;
    cameraCombox_->setIconSize(QSize(1,30));
    cameraCombox_->setFocusPolicy(Qt::NoFocus);
    cameraCombox_->setFixedSize(200,34);
    recordCountL_ = new QLabel(tr("Query Count"));
    recordCountL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    topCombox_ = new QComboBox;
    topCombox_->setIconSize(QSize(1,30));
    topCombox_->setFixedSize(200,34);
    topCombox_->setFocusPolicy(Qt::NoFocus);
    QVector<QPair<QString,int>> itemVec{qMakePair(tr("20"),20),
                qMakePair(tr("50"),50),qMakePair(tr("100"),100),
                qMakePair(tr("200"),200),qMakePair(tr("300"),300),
                qMakePair(tr("500"),500)};
    QPixmap pix(1,topCombox_->iconSize().height());
    pix.fill(Qt::transparent);
    for(const QPair<QString,int> &value : itemVec){
        topCombox_->addItem(value.first,value.second);
    }
    similarL_ = new QLabel(tr("Threshold "));
    similarL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    similarSpin_ = new QSpinBox;
    similarSpin_->setRange(0,100);
    similarSpin_->setSuffix("%");
    similarSpin_->setMinimumHeight(34);
    similarSpin_->setMinimumWidth(200);
    similarSpin_->setValue(40);
    similarSpin_->setSingleStep(5);
    startTimeL_ = new QLabel(tr("Starting Time"));
    startTimeEdit_ = new QDateTimeEdit;
    startTimeEdit_->setMinimumSize(200,34);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDate(QDateTime::currentDateTime().addDays(-1).date());
    endTimeL_ = new QLabel(tr("Ending Time"));
    endTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setMinimumSize(200,34);
    endTimeEdit_->setDate(QDateTime::currentDateTime().addDays(1).date());
    m_searchBtn = new QPushButton(tr("Search"));
    m_searchBtn->setFixedSize(99,34);
    m_searchBtn->setFocusPolicy(Qt::NoFocus);
    gridLay->addWidget(cameraLabel_,0,0,1,1);
    gridLay->addWidget(cameraCombox_,0,1,1,1);
    gridLay->addWidget(recordCountL_,0,2,1,1);
    gridLay->addWidget(topCombox_,0,3,1,1);
    gridLay->addWidget(similarL_,0,4,1,1);
    gridLay->addWidget(similarSpin_,0,5,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(m_searchBtn,1,4,1,1);
    gridLay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLay->setSpacing(20);
    gridLay->setMargin(0);
    hlay->addLayout(gridLay);
    hlay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hlay->setSpacing(20);
    hlay->setMargin(0);
    mainLay->addLayout(hlay);
    m_tableW = new QTableWidget;
    mainLay->addWidget(m_tableW);
    m_pageIndicator = new PageIndicator;
    m_pageIndicator->hide();
    mainLay->addWidget(m_pageIndicator);
    mainLay->setContentsMargins(40,40,40,40);
    mainLay->setSpacing(20);

//    titleBar_ = new WindowTitleBar;
//    QVBoxLayout *winLay = new QVBoxLayout;
//    winLay->addLayout(mainLay);
//    winLay->addWidget(titleBar_);
    setLayout(mainLay);

    menu_ = new QMenu(this);
    menu_->addAction(objectName(),[&]{
        setFaceImage(m_tableW->item(m_tableW->currentRow(),0)->data(Qt::UserRole).value<QImage>());
        setOid(m_tableW->item(m_tableW->currentRow(),1)->text());
    });
    menu_->addAction(tr("Scene analysis"),[&]{
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
        serviceI->getSceneInfo(m_tableW->item(m_tableW->currentRow(),1)->data(Qt::UserRole).toString());
        label->show(500);
        menu_->setEnabled(false);
    });
    menu_->addAction(tr("Save face image"),[this]{
        QString personId = m_tableW->item(m_tableW->currentRow(),1)->text();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!m_tableW->item(m_tableW->currentRow(),0)->data(Qt::UserRole).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    m_tableW->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_tableW,&QTableWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!m_tableW->itemAt(p))return;
        menu_->move(QCursor::pos());
        menu_->show();
    });
    connect(m_tableW->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotSectionClicked(int)));
    connect(m_searchBtn,SIGNAL(clicked(bool)),this,SLOT(slotSearchClicked()));
    connect(m_imgBtn,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
    m_pageIndicator->setPageInfo(0,0);
    m_tableW->setIconSize(QSize(112,112));
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->setFrameStyle(QFrame::NoFrame);
    m_tableW->setEditTriggers(QTableView::NoEditTriggers);
    m_tableW->horizontalHeader()->setHighlightSections(false);
    m_tableW->horizontalHeader()->setDefaultSectionSize(112);
    m_tableW->verticalHeader()->setDefaultSectionSize(112);
    m_tableW->horizontalHeader()->setMinimumHeight(60);
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setColumnCount(5);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("人脸") << tr("ID号") << tr("位置") << tr("相似度") << tr("时间"));
    m_tableW->horizontalHeader()->setSortIndicatorShown(true);
    noDataW_ = new NoDataTip(m_tableW);

    setUserStyle(userStyle());
    getCameraInfo();
}

void FaceSearch::setFaceImage(QImage &img)
{
    faceImg_ = img;
    m_imgBtn->setIcon(QPixmap::fromImage(faceImg_).scaled(m_imgBtn->iconSize()));
}

void FaceSearch::setOid(QString s)
{
    oidStr_ = s;
}

void FaceSearch::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        f = cameraLabel_->font();
        f.setPixelSize(14);
        cameraLabel_->setFont(f);
        recordCountL_->setFont(f);
        startTimeL_->setFont(f);
        endTimeL_->setFont(f);
        similarL_->setFont(f);
        cameraCombox_->setFont(f);
        topCombox_->setFont(f);
        startTimeEdit_->setFont(f);
        endTimeEdit_->setFont(f);
        m_searchBtn->setFont(f);

        pal = cameraLabel_->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255,191));
        cameraLabel_->setPalette(pal);
        recordCountL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);
        similarL_->setPalette(pal);

        m_imgBtn->setStyleSheet("QPushButton{"
                                "border-image: url(images/person-face-back.png);"
                                "background: transparent;"
                                "}");
        m_tableW->setStyleSheet("QTableView{"
                                "color: rgba(255,255,255,0.75);"
                                "font-size: 14px;"
                                "background-color: #383F4F;"
                                "border-radius: 4px;"
                                "selection-background-color: rgba(206,206,206,40);"
                                "}"
                                "QTableView QTableCornerButton::section{"
                                "background: rgba(206,206,206,20);"
                                "}"
                                "QHeaderView{"
                                "background-color: rgb(65,73,92);"
                                "border: none;"
                                "border-radius: 0px;"
                                "}"
                                "QHeaderView::section{"
                                "color: rgba(255,255,255,191);"
                                "background-color: transparent;"
                                "border: none;"
                                "border-radius: 0px;"
                                "}");
        m_tableW->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
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

        m_pageIndicator->setUserStyle();

        m_searchBtn->setStyleSheet("QPushButton{"
                                   "background-color: rgb(83,77,251);"
                                   "color: white;"
                                   "border-radius: 4px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "background-color: #312DA6;"
                                   "}");
        cameraCombox_->setStyleSheet("QComboBoxListView{"
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

        topCombox_->setStyleSheet("QComboBoxListView{"
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

        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "color: rgba(255,255,255,0.75);"
                                      "border-radius:4px;"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "padding-left: 10px;"
                                      "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "color: rgba(255,255,255,0.75);"
                                    "border-radius:4px;"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "padding-left: 10px;"
                                    "}");
        similarSpin_->setStyleSheet("QSpinBox{"
                                    "padding-right: 15px;"
                                    "border-width: 3;"
                                    "color: rgba(255,255,255,0.75);"
                                    "border-radius:4px;"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "font-size: 14px;"
                                    "padding-left: 10px;"
                                    "}");
        noDataW_->setUserStyle(s);
    }
}

void FaceSearch::slotAddRow(QVector<RestServiceI::DataRectureItem> info)
{
    if(info.isEmpty()){
        noDataW_->show();
    }
    for(const RestServiceI::DataRectureItem &itemData : info){
        m_tableW->insertRow(m_tableW->rowCount());
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setIcon(QPixmap::fromImage(itemData.img));
        item->setData(Qt::UserRole,itemData.img);
        m_tableW->setItem(m_tableW->rowCount() - 1,0,item);

        item = new QTableWidgetItem;
        item->setText(itemData.id);
        item->setData(Qt::UserRole,itemData.sceneId);
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,1,item);

        item = new QTableWidgetItem;
        item->setText(cameraMap_.value(itemData.cameraId));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,2,item);

        item = new QTableWidgetItem;
        QString similarityStr;
        similarityStr.setNum(itemData.similarity * 100,'g',4);
        item->setText(similarityStr + '%');
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,3,item);

        item = new QTableWidgetItem;
        item->setText(itemData.time.toString("yyyy-MM-dd HH:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,4,item);
    }
}

bool FaceSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    }
    return WidgetI::event(event);
}

void FaceSearch::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void FaceSearch::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    cameraCombox_->clear();
    QPixmap pix(cameraCombox_->iconSize());
    pix.fill(Qt::transparent);
    cameraCombox_->addItem(tr("不限"),"");
    for (auto &info : data) {
        cameraCombox_->addItem(info.cameraPos,info.cameraId);
        cameraMap_[info.cameraId] = info.cameraPos;
    }
}

void FaceSearch::slotSectionClicked(int index)
{
    static bool isDescendingOrder = false;
    if(isDescendingOrder){
        m_tableW->sortByColumn(index, Qt::AscendingOrder);
    }else{
        m_tableW->sortByColumn(index, Qt::DescendingOrder);
    }
    isDescendingOrder = !isDescendingOrder;
}

void FaceSearch::slotSearchClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(m_tableW);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(m_tableW);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        noDataW_->show();
    });
    connect(serviceI,&RestServiceI::sigSearchByImage,this,[&,label](const QVector<RestServiceI::DataRectureItem> value){
        label->close();
        delete label;
        slotAddRow(value);
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
        if(value.isEmpty()){
            noDataW_->show();
            return;
        }
    });
    RestServiceI::SearchUseImageArgs args;
    args.cameraId = cameraCombox_->currentData().toString();
    args.faceId = oidStr_;
    args.image = faceImg_;
    args.mode = 0;
    args.recordsCount = topCombox_->currentData().toInt();
    args.smilarty = qreal(similarSpin_->value()) / 100;
    args.startT = startTimeEdit_->dateTime();
    args.endT = endTimeEdit_->dateTime();
    serviceI->searchByImage(args);
    label->show(500);
    m_searchBtn->setEnabled(false);
    m_pageIndicator->setEnabled(false);
    noDataW_->hide();
    m_tableW->model()->removeRows(0,m_tableW->rowCount());
}

void FaceSearch::slotImgBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("Images (*.png *.xpm *.jpg)"));
    faceImg_ = QImage(filePath);
    if(filePath.isEmpty() || faceImg_.isNull()){
        m_imgBtn->setIcon(m_imgBtn->property("default-pix").value<QPixmap>().scaled(m_imgBtn->iconSize()));
        qSwap(QImage(),faceImg_);
    }else{
        m_imgBtn->setIcon(QPixmap::fromImage(faceImg_).scaled(m_imgBtn->iconSize()));
        oidStr_.clear();
    }
}

void FaceSearch::slotOnSceneInfo(RestServiceI::SceneInfo sinfo)
{
#if 1
    SceneImageDialog dialog;
    dialog.setUserStyle(userStyle());
    dialog.setSceneInfo(sinfo);
    SceneImageDialog *dialogPtr = &dialog;
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this,dialogPtr](const QVector<QImage> images){
        if(images.isEmpty()){
            return;
        }else{
            faceImg_ = images.first();
            m_imgBtn->setIcon(QPixmap::fromImage(faceImg_).scaled(m_imgBtn->iconSize()));
            dialogPtr->accept();
            slotSearchClicked();
        }
    });
    dialog.exec();
#else
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
#endif
}
