#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QCursor>
#include <QFileDialog>
#include <QMenu>
#include <QStandardPaths>
#include <QApplication>
#include <QEvent>
#include "multiplesearch.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facesearch.h"
#include "informationdialog.h"
#include "nodatatip.h"

MultipleSearch::MultipleSearch( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Simultaneous Scene"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    imgList_ = new QListWidget;
    dataList_ = new QListWidget;
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting Time"));
    endTimeL_ = new QLabel(tr("Ending Time"));
    searchBtn_ = new QPushButton(tr("Search"));
    posCombox_ = new QComboBox;
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    dataMenu_ = new QMenu(dataList_);

    QHBoxLayout *hlay = new QHBoxLayout;
#ifdef MULTIPSEARCHUSEMOVESIZE
    hlay->addWidget(imgList_,1);
    hlay->addStretch(4);
#else
    hlay->addWidget(imgList_);
    hlay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    hlay->setMargin(0);
#endif
    mainLay->addLayout(hlay,1);
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(positionL_,0,0,1,1);
    gridLay->addWidget(posCombox_,0,1,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(searchBtn_,1,6,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    gridLay->setSpacing(20);
    gridLay->setMargin(0);
    mainLay->addLayout(gridLay,1);
    mainLay->addWidget(dataList_,7);
    mainLay->setContentsMargins(37,39,40,40);
    mainLay->setSpacing(20);
    setLayout(mainLay);

    dataMenu_->addAction(tr("Scene analysis"),[this]{
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
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
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
            dataMenu_->setEnabled(true);
        });
        serviceI->getSceneInfo(dataList_->currentItem()->data(Qt::UserRole).toString());
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataList_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dataList_,&QListWidget::customContextMenuRequested,this,[this](const QPoint&p){
        if(!dataList_->itemAt(p))return;
        dataMenu_->move(QCursor::pos());
        dataMenu_->show();
    });
    imgList_->setFlow(QListWidget::LeftToRight);
    imgList_->setFrameStyle(QFrame::NoFrame);
    dataList_->setFlow(QListWidget::LeftToRight);
    dataList_->setViewMode(QListWidget::IconMode);
    dataList_->setMovement(QListWidget::Static);
    dataList_->setIconSize(QSize(340,191));
    dataList_->setFrameStyle(QFrame::NoFrame);
    posCombox_->setFixedSize(200,34);
    startTimeEdit_->setFixedSize(200,34);
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setFixedSize(200,34);
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    searchBtn_->setFixedSize(99,34);
#ifndef MULTIPSEARCHUSEMOVESIZE
    QPixmap iconPix("images/person-face-back.png");
    imgList_->setIconSize(iconPix.size());
    int listWidth = imgList_->iconSize().width() * itemCount_ + (itemCount_ + 1 )* imgList_->spacing() + imgList_->frameWidth() * 2;
    int listHeight = imgList_->iconSize().height() + imgList_->frameWidth() * 2 + imgList_->spacing() * 2;
    imgList_->setFixedSize(listWidth,listHeight);
    for(int i = 0; i < itemCount_; i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(imgList_->iconSize());
        item->setIcon(iconPix);
        item->setData(Qt::UserRole,false);
        imgList_->addItem(item);
    }
#endif

    connect(imgList_,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotItemClicked(QListWidgetItem*)));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    noDataW_ = new NoDataTip(dataList_);

    setUserStyle(userStyle());
    getCameraInfo();
}

void MultipleSearch::setUserStyle(int style)
{
    QPalette pal;
    QFont f;
    if(0 == style){
        imgList_->setStyleSheet("QListWidget{"
                                "background-color: transparent;"
                                "}");
        pal = positionL_->palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255,191));
        positionL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);

        f = font();
        f.setPixelSize(14);
        positionL_->setFont(f);
        startTimeL_->setFont(f);
        endTimeL_->setFont(f);
        startTimeEdit_->setFont(f);
        endTimeEdit_->setFont(f);
        searchBtn_->setFont(f);

        posCombox_->setStyleSheet("QComboBoxListView{"
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
        startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "color: rgba(255,255,255,0.75);"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "border-radius: 4px;"
                                      "padding-left: 10px;"
                                      "}");
        endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "color: rgba(255,255,255,0.75);"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "border-radius: 4px;"
                                    "padding-left: 10px;"
                                    "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 4px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        dataList_->setStyleSheet("QListWidget{"
                                 "background-color: transparent;"
                                 "color: white;"
                                 "}");
        QCursor imgListCursor = imgList_->cursor();
        imgListCursor.setShape(Qt::PointingHandCursor);
        imgList_->setCursor(imgListCursor);

        noDataW_->setUserStyle(style);
    }
}

bool MultipleSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    }
    return WidgetI::event(event);
}

#ifdef MULTIPSEARCHUSEMOVESIZE
void MultipleSearch::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    int itemW = (imgList_->width() - imgList_->frameWidth() * 2 - imgList_->spacing() * (itemCount_ + 1)) / itemCount_;
    int itemH = imgList_->height() - imgList_->frameWidth() * 2 - imgList_->spacing() * 2;
    imgList_->setIconSize(QSize(itemW,itemH));
    if(imgList_->count()){
        for(int i = 0; i < imgList_->count(); i++){
            QListWidgetItem *item = imgList_->item(i);
            item->setSizeHint(QSize(itemW,itemH));
        }
    }else{
        for(int i = 0; i < itemCount_; i++){
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(QSize(itemW,itemH));
            item->setIcon(QPixmap("images/person-face-back.png").scaled(imgList_->iconSize()));
            item->setData(Qt::UserRole,false);
            imgList_->addItem(item);
        }
    }
}
#endif

void MultipleSearch::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void MultipleSearch::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    posCombox_->clear();
    posCombox_->addItem(tr("Unlimited"),"");
    for (auto &info : data) {
        posCombox_->addItem(info.cameraPos,info.cameraId);
        curCameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}

void MultipleSearch::slotItemClicked(QListWidgetItem *item)
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    QPixmap pix(filePath);
    bool isValid = true;
    if(pix.isNull()){
        pix = QPixmap("images/person-face-back.png");
        isValid = false;
    }
    item->setIcon(pix.scaled(imgList_->iconSize()));
    item->setData(Qt::UserRole,isValid);
    item->setData(Qt::UserRole + 1, pix.toImage());
}

void MultipleSearch::slotSearchBtnClicked()
{
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
        searchBtn_->setEnabled(true);
        noDataW_->show();
    });
    connect(serviceI,&RestServiceI::sigMultipleSearch,this,[this,label](QVector<RestServiceI::MultipleSearchItem> data){
        label->close();
        delete label;
        if(data.isEmpty()){
            noDataW_->show();
        }
        for(auto &itemInfo : data){
            QListWidgetItem *item = new QListWidgetItem;
            item->setIcon(QPixmap::fromImage(itemInfo.img).scaled(dataList_->iconSize()));
            item->setText(curCameraMapInfo_.value(itemInfo.cameraId).left(18) + "\n" + itemInfo.time.toString("yyyy-MM-dd HH:mm:ss"));
            item->setData(Qt::UserRole,itemInfo.sceneId);
            dataList_->addItem(item);
        }
        searchBtn_->setEnabled(true);
    });
    RestServiceI::MultipleSearchArgs args;
    args.cameraId = posCombox_->currentData().toString();
    args.startT = startTimeEdit_->dateTime();
    args.endT = endTimeEdit_->dateTime();
    for(int i = 0; i < imgList_->count(); i++){
        if(imgList_->item(i)->data(Qt::UserRole).toBool()){
            args.images << imgList_->item(i)->data(Qt::UserRole + 1).value<QImage>();
        }
    }
    serviceI->multipleSearch(args);
    label->show(500);
    searchBtn_->setEnabled(false);
    noDataW_->hide();
    dataList_->clear();
}
