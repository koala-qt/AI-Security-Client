#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include <QListView>
#include <QFile>
#include <QApplication>
#include "realmonitorsetting.h"
#include "buttondelegate.h"
#include "informationdialog.h"

#pragma execution_character_set("utf-8")
RealMonitorSetting::RealMonitorSetting(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    videoL_ = new QLabel(tr("视频显示设置"));
    screenCountL_ = new QLabel(tr("分屏数量"));
    screenCountCombox_ = new QComboBox;
    screenCountCombox_->setIconSize(QSize(1,30));

    gridLay->addWidget(videoL_,0,0,1,1);
    gridLay->addWidget(screenCountL_,1,1,1,1,Qt::AlignRight);
    gridLay->addWidget(screenCountCombox_,1,2,1,1);

    timeCostL_ = new QLabel(tr("时间差路线设置"));
    startLocationL_ = new QLabel(tr("起点"));
    startLocationCombox_ = new QComboBox;
    startLocationCombox_->setIconSize(QSize(1,30));
    startLocationCombox_->setMinimumWidth(360);

    endLocationL_ = new QLabel(tr("Ending point"));
    endLocationCombox_ = new QComboBox;
    endLocationCombox_->setIconSize(QSize(1,30));
    endLocationCombox_->setMinimumWidth(360);
    arrorL_ = new QLabel(tr(">>>"));
    addStatisBtn_ = new QPushButton(tr("添加"));
    gridLay->addWidget(timeCostL_,2,0,1,1);
    gridLay->addWidget(startLocationL_,3,1,1,1,Qt::AlignRight);
    gridLay->addWidget(startLocationCombox_,3,2,1,1);
    gridLay->addWidget(arrorL_,3,3,1,1);
    gridLay->addWidget(endLocationL_,3,4,1,1,Qt::AlignRight);
    gridLay->addWidget(endLocationCombox_,3,5,1,1);
    gridLay->addWidget(addStatisBtn_,3,6,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(gridLay);

    timeCostTable_ = new QTableWidget;
    mainLay->addWidget(timeCostTable_);
    setLayout(mainLay);

    connect(addStatisBtn_,&QPushButton::clicked,this,[&]{addStatis();});
    connect(timeCostTable_,SIGNAL(cellClicked(int,int)),this,SLOT(slotCellClicked(int,int)));
    timeCostTable_->setFocusPolicy(Qt::NoFocus);
    timeCostTable_->setColumnCount(3);
    ButtonDelegate *itemDelegate = new ButtonDelegate;
    itemDelegate->setText(tr("删除"));
    timeCostTable_->setItemDelegateForColumn(timeCostTable_->columnCount() - 1,itemDelegate);
    timeCostTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    timeCostTable_->setEditTriggers(QTableWidget::NoEditTriggers);
    timeCostTable_->setHorizontalHeaderLabels(QStringList() << tr("起点") << tr("终点") << tr("操作"));
    QSettings setting("config.ini",QSettings::IniFormat);
    int count = setting.beginReadArray("VideoScreen");
    QPixmap pix(screenCountCombox_->iconSize());
    pix.fill(Qt::transparent);
    for(int i = 0; i < count; i++){
        setting.setArrayIndex(i);
        QStringList scrennInfoList = setting.value("screen").toString().split(' ');
        if(scrennInfoList.size() != 6){
            continue;
        }
        int rows = scrennInfoList.first().toInt();
        int cols = scrennInfoList.at(1).toInt();
        int bigFrameRow = scrennInfoList.at(2).toInt();
        int bigFrameCol = scrennInfoList.at(3).toInt();
        int bigFrameRowSpan = scrennInfoList.at(4).toInt();
        int bigFrameColSpan = scrennInfoList.at(5).toInt();
        int showCount = rows * cols - bigFrameRowSpan * bigFrameColSpan + 1;
        screenCountCombox_->addItem(pix,tr("%1分屏").arg(showCount));
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,showCount,Qt::UserRole);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,rows,Qt::UserRole + 1);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,cols,Qt::UserRole + 2);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,bigFrameRow,Qt::UserRole + 3);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,bigFrameCol,Qt::UserRole + 4);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,bigFrameRowSpan,Qt::UserRole + 5);
        screenCountCombox_->setItemData(screenCountCombox_->count() - 1,bigFrameColSpan,Qt::UserRole + 6);
    }

    getCameraInfo();

//    timeCostL_->hide();
//    startLocationL_->hide();
//    startLocationCombox_->hide();
//    arrorL_->hide();
//    endLocationL_->hide();
//    endLocationCombox_->hide();
//    addStatisBtn_->hide();
//    timeCostTable_->hide();
}

void RealMonitorSetting::setScreenIndex(int screenCount)
{
    for(int i = 0; i < screenCountCombox_->count(); i++){
        if(screenCountCombox_->itemData(i,Qt::UserRole).toInt() == screenCount)
        {
            screenCountCombox_->setCurrentIndex(i);
            break;
        }
    }
}

void RealMonitorSetting::screenSelected(int *rows, int *cols, int *bigRow, int *bigCol, int *rowSpan, int *colSpan)
{
    *rows = screenCountCombox_->currentData(Qt::UserRole + 1).toInt();
    *cols = screenCountCombox_->currentData(Qt::UserRole + 2).toInt();
    *bigRow = screenCountCombox_->currentData(Qt::UserRole + 3).toInt();
    *bigCol = screenCountCombox_->currentData(Qt::UserRole + 4).toInt();
    *rowSpan = screenCountCombox_->currentData(Qt::UserRole + 5).toInt();
    *colSpan = screenCountCombox_->currentData(Qt::UserRole + 6).toInt();
}

void RealMonitorSetting::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    QFont f;
    if(s == WidgetManagerI::Danyahei){
        pal = palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        setPalette(pal);
        setAutoFillBackground(true);

        pal = videoL_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        videoL_->setPalette(pal);
        screenCountL_->setPalette(pal);
        timeCostL_->setPalette(pal);
        startLocationL_->setPalette(pal);
        endLocationL_->setPalette(pal);
        pal.setColor(QPalette::Foreground,Qt::yellow);
        arrorL_->setPalette(pal);

        addStatisBtn_->setFlat(true);
        addStatisBtn_->setStyleSheet("background-color:#B4A06C;border-radius: 4px;height:28px;width:50px;color:white");

        pal.setColor(QPalette::ButtonText,Qt::white);
        pal.setColor(QPalette::Button,Qt::green);
        pal.setColor(QPalette::Highlight,Qt::red);
        reinterpret_cast<ButtonDelegate*>(timeCostTable_->itemDelegateForColumn(2))->setPalette(pal);

        timeCostTable_->setStyleSheet(
                    "QTableView{"
                    "color: white;"
                    "border:1px solid #CECECE;"
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
        screenCountCombox_->setStyleSheet(
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
        startLocationCombox_->setStyleSheet(
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
        endLocationCombox_->setStyleSheet(
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
    }
}

void RealMonitorSetting::addStatis()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigResultState(bool)),this,SLOT(slotAddStatis(bool)));
    serviceI->addStatis(startLocationCombox_->currentData().toString(),endLocationCombox_->currentData().toString());
    curStartLocation_ = startLocationCombox_->currentData().toString();
    curEndLocation_ = endLocationCombox_->currentData().toString();
}

void RealMonitorSetting::updateStatis()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigStatisInfo(QVector<StatisTask>)),this,SLOT(slotSnapInfo(QVector<StatisTask>)));
    serviceI->getStatisInfo();
}

void RealMonitorSetting::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

QString RealMonitorSetting::findNameById(QString id)
{
    for(int i = 0; i < startLocationCombox_->count();i++){
        if(startLocationCombox_->itemData(i).toString() == id)
            return startLocationCombox_->itemText(i);
    }

    return QString();
}

void RealMonitorSetting::slotCellClicked(int row, int col)
{
    if(col == 2){
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        connect(serviceI,SIGNAL(sigResultState(bool)),this,SLOT(slotRemoveStatis(bool)));
        serviceI->removeStatis(timeCostTable_->item(row,0)->data(Qt::UserRole + 1).toString(),timeCostTable_->item(row,1)->data(Qt::UserRole + 1).toString());
        curRmRow_ = row;
    }
}

void RealMonitorSetting::slotAddStatis(bool s)
{
    if(s){
        timeCostTable_->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem(findNameById(curStartLocation_));
        item->setData(Qt::UserRole + 1,curStartLocation_);
        item->setTextAlignment(Qt::AlignCenter);
        timeCostTable_->setItem(0,0,item);

        item = new QTableWidgetItem(findNameById(curEndLocation_));
        item->setData(Qt::UserRole + 1,curEndLocation_);
        item->setTextAlignment(Qt::AlignCenter);
        timeCostTable_->setItem(0,1,item);

        item = new QTableWidgetItem;
        item->setBackgroundColor(QColor(59,78,112));
        item->setTextAlignment(Qt::AlignCenter);
        item->setIcon(QPixmap("images/red.png"));
        item->setText(tr("删除"));
        timeCostTable_->setItem(0,2,item);
    }else{
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(1);
        infoDialog.showMessage(tr("Add path failed"));
        infoDialog.exec();
    }
}

void RealMonitorSetting::slotRemoveStatis(bool s)
{
    if(s){
        timeCostTable_->removeRow(curRmRow_);
    }else{
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(1);
        infoDialog.showMessage(tr("Deleted path failed"));
        infoDialog.exec();
    }
}

void RealMonitorSetting::slotSnapInfo(QVector<StatisTask> data)
{
    foreach (const StatisTask &info, data) {
        timeCostTable_->insertRow(0);
        QTableWidgetItem *item = new QTableWidgetItem(findNameById(QString::fromStdString(info.start_camera_id)));
        item->setData(Qt::UserRole + 1,QString::fromStdString(info.start_camera_id));
        item->setTextAlignment(Qt::AlignCenter);
        timeCostTable_->setItem(0,0,item);

        item = new QTableWidgetItem(findNameById(QString::fromStdString(info.end_camera_id)));
        item->setData(Qt::UserRole + 1,QString::fromStdString(info.end_camera_id));
        item->setTextAlignment(Qt::AlignCenter);
        timeCostTable_->setItem(0,1,item);

        item = new QTableWidgetItem;
        item->setIcon(QPixmap("images/red.png"));
        item->setTextAlignment(Qt::AlignCenter);
        timeCostTable_->setItem(0,2,item);
    }
}

void RealMonitorSetting::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    QPixmap pix(startLocationCombox_->iconSize());
    pix.fill(Qt::transparent);
    for (auto &info : data) {
        startLocationCombox_->addItem(pix,info.cameraPos,info.cameraId);
        endLocationCombox_->addItem(pix,info.cameraPos,info.cameraId);
    }
    updateStatis();
}

void RealMonitorSetting::slotOnCameraMap(QVariantMap datas)
{
    QPixmap pix(startLocationCombox_->iconSize());
    pix.fill(Qt::transparent);
    QStringList mapKeys = datas.keys();
    for(auto mapKey : mapKeys){
        startLocationCombox_->addItem(datas.value(mapKey).toString(),mapKey);
        endLocationCombox_->addItem(datas.value(mapKey).toString(),mapKey);
    }
}
