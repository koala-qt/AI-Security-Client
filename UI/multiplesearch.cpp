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
#include <QEvent>
#include "multiplesearch.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facesearch.h"
#include "service/restservice.h"
#include "informationdialog.h"
#include "nodatatip.h"

MultipleSearch::MultipleSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Multiple search"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    imgList_ = new QListWidget;
    dataList_ = new QListWidget;
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
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
    hlay->addStretch();
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
    mainLay->addLayout(gridLay,1);
    mainLay->addWidget(dataList_,7);
    setLayout(mainLay);

    dataMenu_->addAction(tr("Scene analysis"),[this]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(widgetManger()->currentStyle());
            infoDialog.showMessage(str);
            infoDialog.exec();
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            SceneImageDialog dialog;
            dialog.setUserStyle(widgetManger()->currentStyle());
            dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            dialog.setSceneInfo(sinfo);
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
            dataMenu_->setEnabled(true);
        });
        serviceI->getSceneInfo(dataList_->currentItem()->data(Qt::UserRole).toString());
        startWorker(worker);
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
    dataList_->setFlow(QListWidget::LeftToRight);
    dataList_->setViewMode(QListWidget::IconMode);
    dataList_->setMovement(QListWidget::Static);
    dataList_->setIconSize(QSize(192,108));
    posCombox_->setMaximumWidth(290);
    posCombox_->setMinimumHeight(44);
    startTimeEdit_->setMinimumSize(250,44);
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setMinimumSize(250,44);
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    searchBtn_->setMinimumSize(120,44);
#ifndef MULTIPSEARCHUSEMOVESIZE
    imgList_->setIconSize(QSize(140,140));
    int listWidth = imgList_->iconSize().width() * itemCount_ + (itemCount_ + 1 )* imgList_->spacing() + imgList_->frameWidth() * 2;
    int listHeight = imgList_->iconSize().height() + imgList_->frameWidth() * 2 + imgList_->spacing() * 2;
    imgList_->setFixedSize(listWidth,listHeight);
    for(int i = 0; i < itemCount_; i++){
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(imgList_->iconSize());
        item->setIcon(QPixmap("images/person-face-back.png").scaled(imgList_->iconSize()));
        item->setData(Qt::UserRole,false);
        imgList_->addItem(item);
    }
#endif

    connect(imgList_,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotItemClicked(QListWidgetItem*)));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    noDataW_ = new NoDataTip(dataList_);

    getCameraInfo();
}

void MultipleSearch::setUserStyle(WidgetManagerI::SkinStyle style)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == style){
        imgList_->setStyleSheet("QListWidget{"
                                "background-color: transparent;"
                                "}");
        pal = positionL_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        positionL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);

        posCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 16px;"
                    "background-color: transparent;"
                    "border: 1px solid rgba(255, 255, 255, 1);"
                    "border-radius: 4px;"
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
        startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: #B4A06C;"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size:18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: rgba(255,0,0,100);"
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
        return true;
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
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
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
    BLL::Worker *worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(widgetManger()->currentStyle());
        infoDialog.showMessage(str);
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
    startWorker(worker);
    label->show(500);
    searchBtn_->setEnabled(false);
    noDataW_->hide();
    dataList_->clear();
}
