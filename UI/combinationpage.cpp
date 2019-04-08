#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QLabel>
#include <QComboBox>
#include <QTableWidget>
#include <QHeaderView>
#include <QScrollBar>
#include <QSpinBox>
#include <QEvent>
#include <QFileDialog>
#include <QStandardPaths>
#include <QCursor>
#include <QMenu>
#include <QPainter>
#include <QApplication>
#include <QMessageBox>
#include "combinationpage.h"
#include "sceneimagedialog.h"
#include "waitinglabel.h"
#include "facesearch.h"
#include "service/restserviceconcureent.h"
#include "informationdialog.h"
#include "nodatatip.h"

CombinationPage::CombinationPage( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Body"));
    imageBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    similarityL_ = new QLabel(tr("Similarity"));
    queryCountL_ = new QLabel(tr("Query Count"));
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting Time"));
    endTimeL_ = new QLabel(tr("Ending Time"));
    quanzhongL_ = new QLabel(tr("Tradeoff"));
    quanzhongSpin_ = new QSpinBox;
    similaritySpin_ = new QSpinBox;
    queryCountCombox_ = new QComboBox;
    cameraCombox_ = new QComboBox;
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    faceDataL_ = new QLabel(tr("Faces larger than similarity"));
    bodyDataL_ = new QLabel(tr("Combined score"));
    faceTable_ = new QTableWidget;
    bodyTable_ = new QTableWidget;
    faceDataBackW_ = new QWidget;
    bodyDataBackW_ = new QWidget;
    conditionBackW_ = new QWidget;
    faceDataMenu_ = new QMenu(faceTable_);
    bodyDataMenu_ = new QMenu(bodyTable_);

    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(imageBtn_,0,0,2,1);
    gridLay->addWidget(similarityL_,0,1,1,1);
    gridLay->addWidget(similaritySpin_,0,2,1,1);
    gridLay->addWidget(queryCountL_,0,3,1,1);
    gridLay->addWidget(queryCountCombox_,0,4,1,1);
    gridLay->addWidget(quanzhongL_,0,5,1,1);
    gridLay->addWidget(quanzhongSpin_,0,6,1,1);
    gridLay->addWidget(positionL_,1,1,1,1);
    gridLay->addWidget(cameraCombox_,1,2,1,1);
    gridLay->addWidget(startTimeL_,1,3,1,1);
    gridLay->addWidget(startTimeEdit_,1,4,1,1);
    gridLay->addWidget(endTimeL_,1,5,1,1);
    gridLay->addWidget(endTimeEdit_,1,6,1,1);
    gridLay->addWidget(searchBtn_,1,7,1,1);
    gridLay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    gridLay->setSpacing(20);
    gridLay->setMargin(40);
    conditionBackW_->setLayout(gridLay);
    QHBoxLayout *hlay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(faceDataL_);
    vlay->addWidget(faceTable_);
    vlay->setSpacing(20);
    vlay->setContentsMargins(30,20,30,40);
    faceDataBackW_->setLayout(vlay);
    hlay->addWidget(faceDataBackW_);
    vlay = new QVBoxLayout;
    vlay->addWidget(bodyDataL_);
    vlay->addWidget(bodyTable_);
    vlay->setSpacing(20);
    vlay->setContentsMargins(30,20,30,40);
    bodyDataBackW_->setLayout(vlay);
    hlay->addWidget(bodyDataBackW_);
    mainLay->addWidget(conditionBackW_);
    hlay->setSpacing(20);
    mainLay->addLayout(hlay);
    mainLay->setSpacing(26);
    mainLay->setMargin(0);
    setLayout(mainLay);

    faceDataMenu_->addAction(tr("Save face image"),[this]{
        QString personId = faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole + 1).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    faceDataMenu_->addAction(tr("Scene analysis"),[this]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[&,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            faceDataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            faceDataMenu_->setEnabled(true);
        });
        serviceI->getSceneInfo(faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole + 1).toString());
        label->show(500);
        faceDataMenu_->setEnabled(false);
    });
    faceTable_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(faceTable_,&QTableWidget::customContextMenuRequested,this,[this](const QPoint &p){
        if(!faceTable_->itemAt(p))return;
        faceDataMenu_->move(QCursor::pos());
        faceDataMenu_->show();
    });

    bodyDataMenu_->addAction(tr("Save face image"),[this]{
        QString personId = bodyTable_->item(bodyTable_->currentRow(),0)->data(Qt::UserRole + 1).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!bodyTable_->item(bodyTable_->currentRow(),0)->data(Qt::UserRole).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    bodyDataMenu_->addAction(tr("Save body image"),[this]{
        QString personId = bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole + 1).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    bodyDataMenu_->addAction(tr("Scene analysis"),[this]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[&,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            bodyDataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[&,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            slotOnSceneInfo(sinfo);
            bodyDataMenu_->setEnabled(true);
        });
        serviceI->getSceneInfo(bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole + 1).toString());
        label->show(500);
        bodyDataMenu_->setEnabled(false);
    });
    bodyTable_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(bodyTable_,&QTableWidget::customContextMenuRequested,this,[this](const QPoint &p){
        if(!bodyTable_->itemAt(p))return;
        bodyDataMenu_->move(QCursor::pos());
        bodyDataMenu_->show();
    });
    QPixmap pix("images/person-face-back.png");
    imageBtn_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    imageBtn_->setFixedSize(pix.size());
    imageBtn_->setIconSize(imageBtn_->size());
    imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
    imageBtn_->setProperty("default-pix",pix);
    imageBtn_->setFocusPolicy(Qt::NoFocus);
    QCursor curSor = cursor();
    curSor.setShape(Qt::PointingHandCursor);
    imageBtn_->setCursor(curSor);
    faceDataBackW_->installEventFilter(this);
    bodyDataBackW_->installEventFilter(this);
    conditionBackW_->installEventFilter(this);
    similaritySpin_->setMinimumSize(200,34);
    similaritySpin_->setSuffix("%");
    similaritySpin_->setValue(30);
    similaritySpin_->setRange(0,100);
    queryCountCombox_->setMinimumSize(200,34);
    quanzhongSpin_->setSuffix("%");
    quanzhongSpin_->setValue(10);
    quanzhongSpin_->setRange(0,100);
    quanzhongSpin_->setMinimumSize(200,34);
    cameraCombox_->setFixedSize(200,34);
    startTimeEdit_->setMinimumHeight(44);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    startTimeEdit_->setFixedSize(200,34);
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    endTimeEdit_->setFixedSize(200,34);
    searchBtn_->setFixedSize(99,33);
    searchBtn_->setFocusPolicy(Qt::NoFocus);
    QVector<QPair<QString,int>> itemVec{qMakePair(tr("20"),20),
                qMakePair(tr("50"),50),qMakePair(tr("100"),100),
                qMakePair(tr("200"),200),qMakePair(tr("300"),300),
                qMakePair(tr("500"),500)};
    for(const QPair<QString,int> &value : itemVec){
        queryCountCombox_->addItem(value.first,value.second);
    }
    faceTable_->setIconSize(QSize(112,112));
    faceTable_->setFocusPolicy(Qt::NoFocus);
    faceTable_->horizontalHeader()->setHighlightSections(false);
    faceTable_->horizontalHeader()->setDefaultSectionSize(112);
    faceTable_->verticalHeader()->setDefaultSectionSize(112);
    faceTable_->horizontalHeader()->setMinimumHeight(44);
    faceTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    faceTable_->setColumnCount(4);
    faceTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    faceTable_->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    faceTable_->setHorizontalHeaderLabels(QStringList() << tr("Face")  << tr("Position") << tr("Similarity") << tr("Time"));
    faceTable_->horizontalHeader()->setSortIndicatorShown(true);
    faceTable_->setShowGrid(false);
    faceTable_->setFrameStyle(QFrame::NoFrame);
    faceTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);

    bodyTable_->setIconSize(QSize(112,112));
    bodyTable_->setFocusPolicy(Qt::NoFocus);
    bodyTable_->horizontalHeader()->setHighlightSections(false);
    bodyTable_->horizontalHeader()->setDefaultSectionSize(112);
    bodyTable_->verticalHeader()->setDefaultSectionSize(112);
    bodyTable_->horizontalHeader()->setMinimumHeight(44);
    bodyTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    bodyTable_->setColumnCount(5);
    bodyTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bodyTable_->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    bodyTable_->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    bodyTable_->setHorizontalHeaderLabels(QStringList() << tr("Face") << QObject::tr("Body") << tr("Position") << tr("Similarity") << tr("Time"));
    bodyTable_->horizontalHeader()->setSortIndicatorShown(true);
    bodyTable_->setShowGrid(false);
    bodyTable_->setFrameStyle(QFrame::NoFrame);
    bodyTable_->setEditTriggers(QAbstractItemView::NoEditTriggers);
    noFaceDataW_ = new NoDataTip(faceTable_);
    noBodyDataW_ = new NoDataTip(bodyTable_);

    connect(faceTable_->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotFaceTabelSectionClicked(int)));
    connect(bodyTable_->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotBodyTabelSectionClicked(int)));
    connect(imageBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImageBtnClicked()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    setUserStyle(userStyle());
    getCameraInfo();
}

bool CombinationPage::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *backW = qobject_cast<QWidget*>(watched);
    if((backW == faceDataBackW_ || backW == bodyDataBackW_ || backW == conditionBackW_) && event->type() == QEvent::Paint){
        QPainter p(backW);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,102));
        p.drawRoundedRect(backW->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}

void CombinationPage::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void CombinationPage::slotFaceTabelSectionClicked(int index)
{
    if(faceTableOrder_){
        faceTable_->sortByColumn(index, Qt::AscendingOrder);
    }else{
        faceTable_->sortByColumn(index, Qt::DescendingOrder);
    }
    faceTableOrder_ = !faceTableOrder_;
}

void CombinationPage::slotBodyTabelSectionClicked(int index)
{
    if(bodyTableOrder_){
        bodyTable_->sortByColumn(index, Qt::AscendingOrder);
    }else{
        bodyTable_->sortByColumn(index, Qt::DescendingOrder);
    }
    bodyTableOrder_ = !bodyTableOrder_;
}

void CombinationPage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    cameraCombox_->clear();
    cameraCombox_->addItem(tr("Unlimited"),"");
    for (auto &info : data) {
        cameraCombox_->addItem(info.cameraPos,info.cameraId);
        curCameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}

void CombinationPage::slotSearchBtnClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        searchBtn_->setEnabled(true);
        noFaceDataW_->show();
        noBodyDataW_->show();
    });
    connect(serviceI,&RestServiceI::sigCombinationSearch,this,[this,label](RestServiceI::CombinationSearchReturenData returnData){
        label->close();
        delete label;
        for(const RestServiceI::DataRectureItem &itemData : returnData.faceList){
            faceTable_->insertRow(faceTable_->rowCount());
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setIcon(QPixmap::fromImage(itemData.img));
            item->setData(Qt::UserRole,itemData.img);
            item->setData(Qt::UserRole + 1,itemData.sceneId);
            faceTable_->setItem(faceTable_->rowCount() - 1,0,item);

            item = new QTableWidgetItem;
            item->setText(curCameraMapInfo_.value(itemData.cameraId));
            item->setTextAlignment(Qt::AlignCenter);
            faceTable_->setItem(faceTable_->rowCount() - 1,1,item);

            item = new QTableWidgetItem;
            item->setText(QString::number(itemData.similarity));
            item->setTextAlignment(Qt::AlignCenter);
            faceTable_->setItem(faceTable_->rowCount() - 1,2,item);

            item = new QTableWidgetItem;
            item->setText(itemData.time.toString("yyyy-MM-dd HH:mm:ss"));
            item->setTextAlignment(Qt::AlignCenter);
            faceTable_->setItem(faceTable_->rowCount() - 1,3,item);
        }

        for(const RestServiceI::CombinationScoreReturnItem &itemData : returnData.bodyList){
            bodyTable_->insertRow(bodyTable_->rowCount());
            QTableWidgetItem *item = new QTableWidgetItem;
            item->setIcon(QPixmap::fromImage(itemData.faceImg));
            item->setData(Qt::UserRole,itemData.faceImg);
            item->setData(Qt::UserRole + 1,itemData.sceneId);
            bodyTable_->setItem(bodyTable_->rowCount() - 1,0,item);

            item = new QTableWidgetItem;
            item->setSizeHint(QSize(bodyTable_->iconSize().width() >> 1, bodyTable_->iconSize().height()));
            item->setIcon(QPixmap::fromImage(itemData.bodyImg));
            item->setData(Qt::UserRole,itemData.bodyImg);
            item->setData(Qt::UserRole + 1,itemData.sceneId);
            bodyTable_->setItem(bodyTable_->rowCount() - 1,1,item);

            item = new QTableWidgetItem;
            item->setText(curCameraMapInfo_.value(itemData.cameraId));
            item->setTextAlignment(Qt::AlignCenter);
            bodyTable_->setItem(bodyTable_->rowCount() - 1,2,item);

            item = new QTableWidgetItem;
            item->setText(QString::number(itemData.similarity));
            item->setTextAlignment(Qt::AlignCenter);
            bodyTable_->setItem(bodyTable_->rowCount() - 1,3,item);

            item = new QTableWidgetItem;
            item->setText(itemData.time.toString("yyyy-MM-dd HH:mm:ss"));
            item->setTextAlignment(Qt::AlignCenter);
            bodyTable_->setItem(bodyTable_->rowCount() - 1,4,item);
        }

        searchBtn_->setEnabled(true);
        if(returnData.faceList.isEmpty()){
            noFaceDataW_->show();
        }
        if(returnData.bodyList.isEmpty()){
            noBodyDataW_->show();
        }
    });
    RestServiceI::CombinationSearchArgs args;
    args.cameraId = cameraCombox_->currentData().toString();
    args.startTime = startTimeEdit_->dateTime();
    args.endTime = endTimeEdit_->dateTime();
    args.img = imageBtn_->property("pixmap").value<QPixmap>().toImage();
    args.queryCount = queryCountCombox_->currentData().toInt();
    args.similarity = similaritySpin_->value() / (qreal)100;
    args.tradeoff = quanzhongSpin_->value() / (qreal)100;
    serviceI->combinationSearch(args);
    label->show(500);
    searchBtn_->setEnabled(false);
    noFaceDataW_->hide();
    noBodyDataW_->hide();
    faceTable_->model()->removeRows(0,faceTable_->rowCount());
    bodyTable_->model()->removeRows(0,bodyTable_->rowCount());
}

void CombinationPage::slotImageBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    if(filePath.isEmpty()){
        imageBtn_->setIcon(imageBtn_->property("default-pix").value<QPixmap>().scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",QPixmap());
    }else{
        QPixmap pix(filePath);
        imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",pix);
    }
}

void CombinationPage::slotOnSceneInfo(RestServiceI::SceneInfo sinfo)
{
    SceneImageDialog dialog;
    dialog.setUserStyle(userStyle());
    dialog.setSceneInfo(sinfo);
    connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
        if(!images.count()){
            return;
        }
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint | Qt::FramelessWindowHint);
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
void CombinationPage::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(0 == s){
        pal = palette();
        pal.setColor(QPalette::Foreground,QColor(255,255,255,191));
        setPalette(pal);
        similarityL_->setPalette(pal);
        queryCountL_->setPalette(pal);
        quanzhongL_->setPalette(pal);
        positionL_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);
        faceDataL_->setPalette(pal);
        bodyDataL_->setPalette(pal);

        f = font();
        f.setPixelSize(14);
        similarityL_->setFont(f);
        queryCountL_->setFont(f);
        quanzhongL_->setFont(f);
        positionL_->setFont(f);
        startTimeL_->setFont(f);
        endTimeL_->setFont(f);

        f.setPixelSize(16);
        f.setBold(true);
        bodyDataL_->setFont(f);
        faceDataL_->setFont(f);

        faceTable_->setStyleSheet("QTableView{"
                                  "selection-background-color: #383F4F;"
                                  "background-color: rgba(255,255,255,0.05);"
                                  "color: rgba(255,255,255,191);"
                                  "}"
                                  "QTableView QTableCornerButton::section{"
                                  "background: rgb(65,73,92);"
                                  "}"
                                  "QTableWidget::item{"
                                  "border-bottom: 1px solid rgb(62,69,84);"
                                  "}"
                                  "QTableWidget::item:selected{"
                                  "background-color: rgb(43,49,61);"
                                  "color: white;"
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
        faceTable_->horizontalHeader()->setStyleSheet("QHeaderView{"
                                                      "background-color: rgba(255,255,255,0.05);"
                                                      "}"
                                                      "QHeaderView::section{"
                                                      "background-color: transparent;"
                                                      "color: rgba(255,255,255,0.75);"
                                                      "}");
        faceTable_->verticalHeader()->setStyleSheet("QHeaderView{"
                                                    "background-color: rgba(255,255,255,0.15);"
                                                    "}"
                                                    "QHeaderView::section{"
                                                    "background-color: transparent;"
                                                    "color: rgba(255,255,255,0.75);"
                                                    "}");
        bodyTable_->setStyleSheet("QTableView{"
                                  "selection-background-color: #383F4F;"
                                  "background-color: rgba(255,255,255,0.05);"
                                  "color: rgba(255,255,255,191);"
                                  "}"
                                  "QTableView QTableCornerButton::section{"
                                  "background: rgb(65,73,92);"
                                  "}"
                                  "QTableWidget::item{"
                                  "border-bottom: 1px solid rgb(62,69,84);"
                                  "}"
                                  "QTableWidget::item:selected{"
                                  "background-color: rgb(43,49,61);"
                                  "color: white;"
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
        bodyTable_->horizontalHeader()->setStyleSheet("QHeaderView{"
                                                      "background-color: rgba(255,255,255,0.05);"
                                                      "}"
                                                      "QHeaderView::section{"
                                                      "background-color: transparent;"
                                                      "color: rgba(255,255,255,0.75);"
                                                      "}");
        bodyTable_->verticalHeader()->setStyleSheet("QHeaderView{"
                                                    "background-color: rgba(255,255,255,0.15);"
                                                    "}"
                                                    "QHeaderView::section{"
                                                    "background-color: transparent;"
                                                    "color: rgba(255,255,255,0.75);"
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
                                     "width:10px;"
                                     "height:6px;"
                                     "subcontrol-origin: padding;"
                                     "margin-right:2px;"
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
        queryCountCombox_->setStyleSheet("QComboBoxListView{"
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
                                         "width:10px;"
                                         "height:6px;"
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
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "width: 99px;"
                                  "height: 34px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        similaritySpin_->setStyleSheet("QSpinBox{"
                                       "padding-right: 15px;"
                                       "border-width: 3;"
                                       "color: rgba(255,255,255,191);"
                                       "background-color: rgba(255,255,255,0.1);"
                                       "border-radius:4px;"
                                       "font-size: 14px;"
                                       "padding-left: 10px;"
                                       "}");
        quanzhongSpin_->setStyleSheet("QSpinBox{"
                                      "padding-right: 15px;"
                                      "border-width: 3;"
                                      "color: rgba(255,255,255,191);"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "border-radius:4px;"
                                      "font-size: 14px;"
                                      "padding-left: 10px;"
                                      "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "border-radius:4px;"
                                      "color: rgba(255,255,255,191);"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "padding-left: 10px;"
                                      "font-size: 14px;"
                                      "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "border-radius:4px;"
                                    "color: rgba(255,255,255,191);"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "padding-left: 10px;"
                                    "font-size: 14px;"
                                    "}");
        imageBtn_->setStyleSheet("QPushButton{"
                                 "background-color: transparent;"
                                 "}");
        noFaceDataW_->setUserStyle(s);
        noBodyDataW_->setUserStyle(s);
    }
}

bool CombinationPage::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
        return true;
    }
    return WidgetI::event(event);
}
