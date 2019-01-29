#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QFileDialog>
#include <QStandardPaths>
#include "multiplesearch.h"
#include "pageindicator.h"
#include "service/restservice.h"

MultipleSearch::MultipleSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Multiple search"));
    backImg_.load("images/Mask.png");
    QVBoxLayout *mainLay = new QVBoxLayout;
    imgList_ = new QListWidget;
    dataList_ = new QListWidget;
    similarityL_ = new QLabel(tr("Similarity"));
    queryCountL_ = new QLabel(tr("Query count"));
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
    searchBtn_ = new QPushButton(tr("Search"));
    similaritySpin_ = new QSpinBox;
    queryCountCobox_ = new QComboBox;
    posCombox_ = new QComboBox;
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    pageIndicator_ = new PageIndicator;

    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(imgList_,1);
    hlay->addStretch(4);
    mainLay->addLayout(hlay,1);
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(positionL_,0,0,1,1);
    gridLay->addWidget(posCombox_,0,1,1,1);
    gridLay->addWidget(similarityL_,0,2,1,1);
    gridLay->addWidget(similaritySpin_,0,3,1,1);
    gridLay->addWidget(queryCountL_,0,4,1,1);
    gridLay->addWidget(queryCountCobox_,0,5,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(searchBtn_,1,6,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(gridLay,1);
    mainLay->addWidget(dataList_,7);
    mainLay->addWidget(pageIndicator_);
    setLayout(mainLay);

    imgList_->setFlow(QListWidget::LeftToRight);
    dataList_->setFlow(QListWidget::LeftToRight);
    dataList_->setViewMode(QListWidget::IconMode);
    posCombox_->setMaximumWidth(290);
    posCombox_->setMinimumHeight(44);
    similaritySpin_->setMinimumHeight(44);
    queryCountCobox_->setMinimumHeight(44);
    startTimeEdit_->setMinimumSize(250,44);
    endTimeEdit_->setMinimumSize(250,44);
    searchBtn_->setMinimumSize(120,44);
    similaritySpin_->setValue(40);
    similaritySpin_->setSuffix(tr("%"));
    pageIndicator_->setPageInfo(0,0);
    QVector<int> searchCountVec;
    searchCountVec << 20 << 50 << 100 << 500;
    for(const int countV : searchCountVec){
        queryCountCobox_->addItem(QString::number(countV));
    }

    connect(imgList_,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slotItemClicked(QListWidgetItem*)));
    getCameraInfo();
}

void MultipleSearch::setUserStyle(WidgetManagerI::SkinStyle style)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == style){
        imgList_->setStyleSheet("QListWidget{"
                                "background-color: transparent;"
                                "}");
        pal = similarityL_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        similarityL_->setPalette(pal);
        queryCountL_->setPalette(pal);
        positionL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);
        similaritySpin_->setStyleSheet("QSpinBox{"
                                    "padding-right: 15px;"
                                    "background-color: transparent;"
                                    "border:1px solid #CECECE;"
                                    "border-radius:4px;"
                                    "color: white;"
                                    "font-size: 16px;"
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
        queryCountCobox_->setStyleSheet(
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
                    "}");
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
                                 "color: white;"
                                 "background-color: rgba(112, 112, 112, 1);"
                                 "}");
        dataList_->setStyleSheet("QListWidget{"
                                 "background-color: transparent;"
                                 "}");
        QCursor imgListCursor = imgList_->cursor();
        imgListCursor.setShape(Qt::PointingHandCursor);
        imgList_->setCursor(imgListCursor);
    }
    pageIndicator_->setUserStyle();
}

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
            imgList_->addItem(item);
        }
    }
}

void MultipleSearch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void MultipleSearch::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void MultipleSearch::slotOnCameraInfo(QVector<CameraInfo> data)
{
    posCombox_->clear();
    posCombox_->addItem(tr("Unlimited"),"");
    foreach (const CameraInfo &info, data) {
        posCombox_->addItem(QString::fromStdString(info.position),QString::fromStdString(info.id));
    }
}

void MultipleSearch::slotItemClicked(QListWidgetItem *item)
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    QPixmap pix(filePath);
    if(pix.isNull()){
        return;
    }
    item->setIcon(pix);
}
