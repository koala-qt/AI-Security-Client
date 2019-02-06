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
#include <QMessageBox>
#include "combinationpage.h"
#include "sceneimagedialog.h"
#include "waitinglabel.h"
#include "facesearch.h"
#include "service/restservice.h"

CombinationPage::CombinationPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Combination search"));
    imageBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    similarityL_ = new QLabel(tr("Similarity"));
    queryCountL_ = new QLabel(tr("Query count"));
    positionL_ = new QLabel(tr("Position"));
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
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
    gridLay->setAlignment(Qt::AlignLeft);
    conditionBackW_->setLayout(gridLay);
    QHBoxLayout *hlay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(faceDataL_);
    vlay->addWidget(faceTable_);
    faceDataBackW_->setLayout(vlay);
    hlay->addWidget(faceDataBackW_);
    vlay = new QVBoxLayout;
    vlay->addWidget(bodyDataL_);
    vlay->addWidget(bodyTable_);
    bodyDataBackW_->setLayout(vlay);
    hlay->addWidget(bodyDataBackW_);
    mainLay->addWidget(conditionBackW_);
    mainLay->addLayout(hlay);
    mainLay->setMargin(0);
    setLayout(mainLay);

    faceDataMenu_->addAction(tr("Save face image"),[this]{
        QString personId = faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole + 1).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole).value<QImage>().save(filePath)){
            QMessageBox::information(this,tr("Save face image"),tr("Operation failed!"));
        }
    });
    faceDataMenu_->addAction(tr("Scene analysis"),[this]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[&,label](const QString str){
            label->close();
            delete label;
            QMessageBox::information(this,objectName(),str);
            faceDataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnSceneImg(img);
            faceDataMenu_->setEnabled(true);
        });
        serviceI->getScenePic(faceTable_->item(faceTable_->currentRow(),0)->data(Qt::UserRole + 1).toString());
        startWorker(worker);
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
            QMessageBox::information(this,tr("Save face image"),tr("Operation failed!"));
        }
    });
    bodyDataMenu_->addAction(tr("Save body image"),[this]{
        QString personId = bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole + 1).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole).value<QImage>().save(filePath)){
            QMessageBox::information(this,tr("Save face image"),tr("Operation failed!"));
        }
    });
    bodyDataMenu_->addAction(tr("Scene analysis"),[this]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[&,label](const QString str){
            label->close();
            delete label;
            QMessageBox::information(this,objectName(),str);
            bodyDataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[&,label](const QImage img){
            label->close();
            delete label;
            slotOnSceneImg(img);
            bodyDataMenu_->setEnabled(true);
        });
        serviceI->getScenePic(bodyTable_->item(bodyTable_->currentRow(),1)->data(Qt::UserRole + 1).toString());
        startWorker(worker);
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
    imageBtn_->setFixedSize(92,92);
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
    similaritySpin_->setMinimumSize(120,44);
    similaritySpin_->setSuffix("%");
    similaritySpin_->setValue(50);
    similaritySpin_->setRange(0,100);
    queryCountCombox_->setMinimumHeight(44);
    queryCountCombox_->setMaximumWidth(250);
    quanzhongSpin_->setMinimumHeight(44);
    quanzhongSpin_->setSuffix("%");
    quanzhongSpin_->setValue(40);
    quanzhongSpin_->setRange(0,100);
    cameraCombox_->setMinimumHeight(44);
    cameraCombox_->setMaximumWidth(250);
    startTimeEdit_->setMinimumHeight(44);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setMinimumHeight(44);
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    searchBtn_->setMinimumSize(120,44);
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
    faceTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    faceTable_->setColumnCount(4);
    faceTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    faceTable_->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    faceTable_->setHorizontalHeaderLabels(QStringList() << tr("Face")  << tr("Position") << tr("Similarity") << tr("Time"));
    faceTable_->horizontalHeader()->setSortIndicatorShown(true);
    faceTable_->setShowGrid(false);

    bodyTable_->setIconSize(QSize(112,112));
    bodyTable_->setFocusPolicy(Qt::NoFocus);
    bodyTable_->horizontalHeader()->setHighlightSections(false);
    bodyTable_->horizontalHeader()->setDefaultSectionSize(112);
    bodyTable_->verticalHeader()->setDefaultSectionSize(112);
    bodyTable_->setSelectionBehavior(QAbstractItemView::SelectRows);
    bodyTable_->setColumnCount(5);
    bodyTable_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bodyTable_->horizontalHeader()->setSectionResizeMode(0,QHeaderView::Fixed);
    bodyTable_->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Fixed);
    bodyTable_->setHorizontalHeaderLabels(QStringList() << tr("Face") << tr("Body") << tr("Position") << tr("Similarity") << tr("Time"));
    bodyTable_->horizontalHeader()->setSortIndicatorShown(true);
    bodyTable_->setShowGrid(false);

    connect(faceTable_->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotFaceTabelSectionClicked(int)));
    connect(bodyTable_->horizontalHeader(),SIGNAL(sectionClicked(int)),this,SLOT(slotBodyTabelSectionClicked(int)));
    connect(imageBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImageBtnClicked()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    getCameraInfo();
}

bool CombinationPage::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *backW = qobject_cast<QWidget*>(watched);
    if((backW == faceDataBackW_ || backW == bodyDataBackW_ || backW == conditionBackW_) && event->type() == QEvent::Paint){
        QPainter p(backW);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,50));
        p.drawRoundedRect(backW->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}

void CombinationPage::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
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
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        delete label;
        QMessageBox::information(this,objectName(),str);
        searchBtn_->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigCombinationSearch,this,[this,label](RestServiceI::CombinationSearchReturenData returnData){
        label->close();
        delete label;
        faceTable_->model()->removeRows(0,faceTable_->rowCount());
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

        bodyTable_->model()->removeRows(0,bodyTable_->rowCount());
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
    startWorker(worker);
    label->show(500);
    searchBtn_->setEnabled(false);
}

void CombinationPage::slotImageBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    if(filePath.isEmpty()){
        imageBtn_->setIcon(imageBtn_->property("default-pix").value<QPixmap>().scaled(imageBtn_->iconSize()));
    }else{
        QPixmap pix(filePath);
        imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",pix);
    }
}

void CombinationPage::slotOnSceneImg(QImage img)
{
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
}
void CombinationPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == s){
        pal = palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        setPalette(pal);

        faceTable_->setStyleSheet(
                    "QTableView{"
                    "selection-background-color: rgb(235,245,255);"
                    "background-color: transparent;"
                    "color: white;"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgb(0,138,194);"
                    "}"
                    "QTableWidget::Item{"
                    "border-bottom: 1px solid rgb(184,184,184);"
                    "}"
                    "QTableWidget::item:selected{"
                    "background-color: rgba(0,138,194,102);"
                    "color: blue;"
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
                                                      "background-color: rgba(0,0,0,100);"
                                                      "}"
                                                      "QHeaderView::section{"
                                                      "background-color: transparent;"
                                                      "color:white;"
                                                      "}");
        faceTable_->verticalHeader()->setStyleSheet("QHeaderView{"
                                                    "background-color: rgba(0,0,0,100);"
                                                    "}"
                                                    "QHeaderView::section{"
                                                    "background-color: transparent;"
                                                    "color:white;"
                                                    "}");
        bodyTable_->setStyleSheet(
                    "QTableView{"
                    "selection-background-color: rgb(235,245,255);"
                    "background-color: transparent;"
                    "color: white;"
                    "}"
                    "QTableView QTableCornerButton::section{"
                    "background: rgb(0,138,194);"
                    "}"
                    "QTableWidget::Item{"
                    "border-bottom: 1px solid rgb(184,184,184);"
                    "}"
                    "QTableWidget::item:selected{"
                    "background-color: rgb(0,138,194,102);"
                    "color: blue;"
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
                                                      "background-color: rgba(0,0,0,100);"
                                                      "}"
                                                      "QHeaderView::section{"
                                                      "background-color: transparent;"
                                                      "color:white;"
                                                      "}");
        bodyTable_->verticalHeader()->setStyleSheet("QHeaderView{"
                                                    "background-color: rgba(0,0,0,100);"
                                                    "}"
                                                    "QHeaderView::section{"
                                                    "background-color: transparent;"
                                                    "color:white;"
                                                    "}");

        cameraCombox_->setStyleSheet(
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
        queryCountCombox_->setStyleSheet(
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
        searchBtn_->setStyleSheet("QPushButton{"
                                   "background-color: #B4A06C;"
                                   "color: white;"
                                   "border-radius: 6px;"
                                   "font-size:18px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "}");
        similaritySpin_->setStyleSheet("QSpinBox{"
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
        quanzhongSpin_->setStyleSheet("QSpinBox{"
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
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        imageBtn_->setStyleSheet("QPushButton{"
                                 "background-color: transparent;"
                                 "}");
    }
}
