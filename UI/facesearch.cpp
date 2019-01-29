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
#include <QStandardPaths>
#include <QSpinBox>
#include <QMenu>
#include <QDebug>
#include <QListView>
#include <QEvent>
#include <QMessageBox>
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "service/restservice.h"

#pragma execution_character_set("utf-8")
FaceSearch::FaceSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Recent snapshots and Tracking"));
    //this->setWindowFlags(Qt::Widget | Qt::WindowCloseButtonHint);
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    m_imgBtn = new QPushButton;
    m_imgBtn->setToolTip(tr("添加图片"));
    m_imgBtn->setFixedSize(150,150);
    m_imgBtn->setIconSize(QSize(m_imgBtn->size()));
    m_imgBtn->setFocusPolicy(Qt::NoFocus);
    QCursor curSor = cursor();
    curSor.setShape(Qt::PointingHandCursor);
    m_imgBtn->setCursor(curSor);
    hlay->addWidget(m_imgBtn);

    QGridLayout *gridLay = new QGridLayout;
    cameraLabel_ = new QLabel(tr("位置"));
    cameraLabel_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    cameraCombox_ = new QComboBox;
    cameraCombox_->setIconSize(QSize(1,30));
    cameraCombox_->setFocusPolicy(Qt::NoFocus);
    cameraCombox_->setFixedHeight(40);
    recordCountL_ = new QLabel(tr("查询条数"));
    recordCountL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    topCombox_ = new QComboBox;
    topCombox_->setIconSize(QSize(1,30));
    topCombox_->setFixedHeight(40);
    topCombox_->setFocusPolicy(Qt::NoFocus);
    QVector<QPair<QString,int>> itemVec{qMakePair(tr("20"),20),
                qMakePair(tr("50"),50),qMakePair(tr("100"),100),
                qMakePair(tr("200"),200),qMakePair(tr("300"),300),
                qMakePair(tr("500"),500)};
    QPixmap pix(1,topCombox_->iconSize().height());
    pix.fill(Qt::transparent);
    for(const QPair<QString,int> &value : itemVec){
        topCombox_->addItem(pix,value.first,value.second);
    }
    similarL_ = new QLabel(tr("最低相似度"));
    similarL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    similarSpin_ = new QSpinBox;
    similarSpin_->setRange(0,100);
    similarSpin_->setSuffix("%");
    similarSpin_->setMinimumHeight(40);
    similarSpin_->setMinimumWidth(200);
    similarSpin_->setValue(40);
    startTimeL_ = new QLabel(tr("开始时间"));
    startTimeEdit_ = new QDateTimeEdit;
    startTimeEdit_->setMinimumSize(160,40);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDate(QDateTime::currentDateTime().addDays(-1).date());
    endTimeL_ = new QLabel(tr("结束时间"));
    endTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setMinimumSize(160,40);
    endTimeEdit_->setDate(QDateTime::currentDateTime().addDays(1).date());
    m_searchBtn = new QPushButton(tr("检索"));
    m_searchBtn->setFixedHeight(40);
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
    gridLay->addWidget(m_searchBtn,2,5,1,1);
    gridLay->setContentsMargins(10,5,0,0);
    gridLay->setHorizontalSpacing(15);
    gridLay->setVerticalSpacing(0);
    QHBoxLayout *topRighHlay = new QHBoxLayout;
    topRighHlay->addLayout(gridLay);
    topRighHlay->addStretch();
    hlay->addLayout(topRighHlay);
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);
    m_tableW = new QTableWidget;
    mainLay->addWidget(m_tableW);
    m_pageIndicator = new PageIndicator;
    m_pageIndicator->hide();
    hlay = new QHBoxLayout;
    hlay->addWidget(m_pageIndicator);
    hlay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(hlay);
    mainLay->setContentsMargins(30,20,10,10);
    setLayout(mainLay);

    menu_ = new QMenu(this);
    menu_->addAction(tr("View recent snapshots"),[&]{
        QLabel *curLab = dynamic_cast<QLabel *>(m_tableW->cellWidget(m_tableW->currentRow(), 0));
        if (curLab)
        {
            this->setFaceImage(curLab->pixmap()->toImage());
            this->setOid(m_tableW->item(m_tableW->currentRow(),1)->text());
        }
    });
    menu_->addAction(tr("Scene analysis"),[&]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            QMessageBox::information(this,tr("Scene"),str);
            menu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnScenePic(img);
            menu_->setEnabled(true);
        });
        serviceI->getScenePic(m_tableW->item(m_tableW->currentRow(),1)->text());
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
    connect(m_searchBtn,SIGNAL(clicked(bool)),this,SLOT(slotSearchClicked()));
    connect(m_imgBtn,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
    m_pageIndicator->setPageInfo(0,0);
    m_tableW->setIconSize(QSize(112,112));
    m_tableW->setFocusPolicy(Qt::NoFocus);
    m_tableW->horizontalHeader()->setHighlightSections(false);
    m_tableW->horizontalHeader()->setDefaultSectionSize(112);
    m_tableW->verticalHeader()->setDefaultSectionSize(112);
    m_tableW->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableW->setColumnCount(5);
    m_tableW->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_tableW->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    m_tableW->setHorizontalHeaderLabels(QStringList() << tr("人脸") << tr("ID号") << tr("位置") << tr("相似度") << tr("时间"));
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

void FaceSearch::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    QFont f;
    if(s == WidgetManagerI::Danyahei){
        f = font();
        f.setFamily("Arial");
        setFont(f);
        f = cameraLabel_->font();
        f.setPixelSize(18);
        f.setFamily("Arial");
        cameraLabel_->setFont(f);
        recordCountL_->setFont(f);
        startTimeL_->setFont(f);
        endTimeL_->setFont(f);
        similarL_->setFont(f);

        m_imgBtn->setFlat(true);
        m_imgBtn->setStyleSheet("background-color:#B4A06C;border:1px solid #B4A06C;border-radius:6px;");

        pal = cameraLabel_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        cameraLabel_->setPalette(pal);
        recordCountL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);
        similarL_->setPalette(pal);

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

        m_pageIndicator->setUserStyle();

        m_searchBtn->setStyleSheet("QPushButton{"
                                   "border: 0px;"
                                   "color:white;"
                                   "background: #B4A06C;"
                                   "border-radius: 6px;"
                                   "font-size:18px;"
                                   //"font-family:微软雅黑;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 1px;"
                                   "}");

//        m_idEdit_->setStyleSheet("QLineEdit{"
//                                  "color:white;"
//                                  "background-color:transparent;"
//                                  "font-size:24px;"
//                                  "border:1px solid #CECECE;"
//                                  "border-radius:6px;"
//                                  "}");
        // 设置 min-width有效果，其它无效果
        //        "QComboBox QAbstractItemView{"
        //        "selection-color: red; min-width: 250px;"
        //        "outline: 0px;"
        //        "selection-background-color: red;"
        //        "}"
        cameraCombox_->setStyleSheet(
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

        topCombox_->setStyleSheet(
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

        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:4px;"
            "background-color: transparent;"
                                       "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color:#CECECE;"
            "border:1px solid #CECECE;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        similarSpin_->setStyleSheet("QSpinBox{"
                                 "padding-right: 15px;"
                                 "border-width: 3;"
                                 "background-color: transparent;"
                                 "border:1px solid #CECECE;"
                                    "border-radius:6px;"
                                 "color: white;"
                                 "font-size: 18px;"
                                 "}"
                                 "QSpinBox::up-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: top right;"
                                 "width: 16px;"
                                 "border-image: url(images/on.png) 1;"
                                 "}"
                                 "QSpinBox::down-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: bottom right;"
                                 "width: 16px;"
                                 "border-image: url(images/under.png) 1;"
                                 "}");

//        "QSpinBox::up-arrow{"
//        "image: url(images/on.png);"
//        "}"
//        "QSpinBox::down-arrow{"
//        "image: url(images/under.png);"
//        "}"
    }
}

void FaceSearch::slotAddRow(QVector<SearchFace> info)
{
    m_tableW->model()->removeRows(0,m_tableW->rowCount());
    for(const SearchFace &itemData : info){
        m_tableW->insertRow(m_tableW->rowCount());
        QTableWidgetItem *item = new QTableWidgetItem;
        m_tableW->setItem(m_tableW->rowCount() - 1,0,item);
        QImage img;
        img.loadFromData(QByteArray::fromStdString(itemData.face_img));
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
        item->setText(QString::number(itemData.score));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,3,item);

        item = new QTableWidgetItem;
        item->setText(QDateTime::fromMSecsSinceEpoch(itemData.time).toString("yyyy-MM-dd HH:mm:ss"));
        item->setTextAlignment(Qt::AlignCenter);
        m_tableW->setItem(m_tableW->rowCount() - 1,4,item);
    }
}

bool FaceSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
        return true;
    }
    return WidgetI::event(event);
}

void FaceSearch::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void FaceSearch::slotOnCameraInfo(QVector<CameraInfo> data)
{
    cameraCombox_->clear();
    QPixmap pix(cameraCombox_->iconSize());
    pix.fill(Qt::transparent);
    cameraCombox_->addItem(pix,tr("不限"),"");
    foreach (const CameraInfo &info, data) {
        cameraCombox_->addItem(pix,QString::fromStdString(info.position),QString::fromStdString(info.id));
    }
}

void FaceSearch::slotSearchClicked()
{
    if(faceImg_.isNull() && oidStr_.isEmpty()){
        return;
    }
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        oidStr_.clear();
        QMessageBox::information(this,objectName(),str);
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigFaceSearch,this,[&,label](const QVector<SearchFace> value){
        label->close();
        oidStr_.clear();
        slotAddRow(value);
        m_searchBtn->setEnabled(true);
        m_pageIndicator->setEnabled(true);
    });
    if(oidStr_.isEmpty()){
        serviceI->searchSnap("",faceImg_,"",cameraCombox_->currentData().toString(),topCombox_->currentData().toInt(), qreal(similarSpin_->value()) / 100,startTimeEdit_->dateTime(),endTimeEdit_->dateTime());
    }else{
        serviceI->searchSnap("",QImage(),oidStr_,cameraCombox_->currentData().toString(),topCombox_->currentData().toInt(), qreal(similarSpin_->value()) / 100,startTimeEdit_->dateTime(),endTimeEdit_->dateTime());
    }
    startWorker(worker);
    label->show(500);
    m_searchBtn->setEnabled(false);
    m_pageIndicator->setEnabled(false);
}

void FaceSearch::slotImgBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    faceImg_ = QImage(filePath);
    m_imgBtn->setIcon(QPixmap::fromImage(faceImg_).scaled(m_imgBtn->iconSize()));
}

void FaceSearch::slotOnScenePic(QImage img)
{
#if 1
    SceneImageDialog dialog;
    dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    dialog.setUserStyle(widgetManger()->currentStyle());
    dialog.setImage(img);
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](const QVector<QImage> images){
        if(images.isEmpty()){
            return;
        }else{
            faceImg_ = images.first();
            m_imgBtn->setIcon(QPixmap::fromImage(faceImg_).scaled(m_imgBtn->iconSize()));
            slotSearchClicked();
        }
    });
    dialog.setRectLinePen(Qt::yellow);
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
