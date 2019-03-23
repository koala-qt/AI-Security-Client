#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QListWidget>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QComboBox>
#include <QScrollBar>
#include <QHeaderView>
#include <QSpinBox>
#include <QEvent>
#include <QDir>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QStandardPaths>
#include "facelinktable.h"
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facelinkpage.h"
#include "trackingpage.h"
#include "portrait.h"
#include "facesearch.h"
#include "informationdialog.h"
#include "nodatatip.h"

FacelinkTable::FacelinkTable( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Face link"));
    posL_ = new QLabel(tr("Position"));
    posCombox_ = new QComboBox;
    startTimeL_ = new QLabel(tr("Starting time"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeL_ = new QLabel(tr("Ending time"));
    endTimeEdit_ = new QDateTimeEdit;
    searchBtn_ = new QPushButton(tr("Search"));
    dataListW_ = new QListWidget;
    pageIndicator_ = new PageIndicator;
    dataMenu_ = new QMenu(dataListW_);

    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *topGridLay = new QGridLayout;
    topGridLay->addWidget(posL_,0,0,1,1);
    topGridLay->addWidget(posCombox_,0,1,1,1);
    topGridLay->addWidget(startTimeL_,1,0,1,1);
    topGridLay->addWidget(startTimeEdit_,1,1,1,1);
    topGridLay->addWidget(endTimeL_,1,2,1,1);
    topGridLay->addWidget(endTimeEdit_,1,3,1,1);
    topGridLay->addWidget(searchBtn_,1,4,1,1);
    topGridLay->setAlignment(Qt::AlignLeft);
    topGridLay->setSpacing(25);
    topGridLay->setMargin(0);
    mainLay->addLayout(topGridLay);
    mainLay->addWidget(dataListW_);
    mainLay->addWidget(pageIndicator_);
    setLayout(mainLay);
#if 0
    dataMenu_->addAction(tr("Details"),[this]{
        BLL::Worker * worker = new BLL::RestService(this->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(dataListW_);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigPeronsDetails,this,[this,label](QImage face,QImage body,QStringList faceAttr,QStringList bodyAttr){
            label->close();
            delete label;
            Portrait *detailsW = new Portrait(this,this);
            detailsW->setAttribute(Qt::WA_DeleteOnClose);
            detailsW->setWindowFlags(Qt::Window | Qt::Dialog);
            detailsW->setWindowModality(Qt::ApplicationModal);
            detailsW->setUserStyle(userStyle());
            detailsW->slotSetData(face,body,faceAttr,bodyAttr);
            detailsW->show();
            QPoint r = dataListW_->mapToGlobal(dataListW_->rect().center());
            QRect dr = detailsW->rect();
            dr.moveCenter(r);
            detailsW->move(dr.topLeft());
            dataMenu_->setEnabled(true);
        });
        serviceI->getPersonDetails(dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Scene analysis"),[this]{
        BLL::Worker * worker = new BLL::RestService(this->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(dataListW_);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[this,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            SceneImageDialog dialog(dataListW_);
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
            dataMenu_->setEnabled(true);
        });
        serviceI->getSceneInfo(dataListW_->currentItem()->data(Qt::UserRole + 5).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Search using the image"),[this]{
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(112,110,119));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(userStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceDialog->setOid(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    dataMenu_->addAction(tr("Tracking"),[this]{
        TrackingPage *view = new TrackingPage(this);
        view->setUserStyle(userStyle());
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setWindowFlags(Qt::Window | Qt::Dialog);
        view->setWindowModality(Qt::ApplicationModal);
        view->setMinimumSize(1655,924);
        view->setImgageOid(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>(),
                           dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        view->show();
    });
#endif
    dataMenu_->addAction(tr("Face link"),[this]{
        FaceLinkPage *faceLinkP = new FaceLinkPage(this);
        QPalette pal = faceLinkP->palette();
        pal.setColor(QPalette::Background,QColor(100,100,100));
        faceLinkP->setPalette(pal);
        faceLinkP->setAutoFillBackground(true);
        faceLinkP->setUserStyle(userStyle());
        faceLinkP->setAttribute(Qt::WA_DeleteOnClose);
        faceLinkP->setWindowFlags(Qt::Window | Qt::Dialog);
        faceLinkP->setWindowModality(Qt::ApplicationModal);
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceLinkP->setFaceLinkOidAndImg(dataListW_->currentItem()->data(Qt::UserRole + 4).toString(),pix);
        faceLinkP->resize(1200,900);
        faceLinkP->show();
    });
    dataMenu_->addAction(tr("Save face image"),[this]{
        QString personId = dataListW_->currentItem()->data(Qt::UserRole + 4).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
        }
    });
    dataListW_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dataListW_,&QListWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!dataListW_->itemAt(p))return;
        dataMenu_->move(QCursor::pos());
        dataMenu_->show();
    });
    dataListW_->setFocusPolicy(Qt::NoFocus);
    dataListW_->setMovement(QListWidget::Static);
    dataListW_->setResizeMode(QListWidget::Adjust);
    dataListW_->setViewMode(QListWidget::IconMode);
    posCombox_->setMinimumHeight(44);
    posCombox_->setMaximumWidth(160);
    startTimeEdit_->setMinimumHeight(44);
    startTimeEdit_->setMinimumWidth(160);
    startTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setMinimumHeight(44);
    endTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    endTimeEdit_->setMinimumWidth(160);
    searchBtn_->setMinimumSize(120,44);
    pageIndicator_->setPageInfo(0,0);
    noDataW_ = new NoDataTip(dataListW_);

    connect(pageIndicator_,SIGNAL(sigPageClicked(int)),this,SLOT(slotSemanticSearch(int)));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    QSettings config("config.ini",QSettings::IniFormat);
    dataRows_ = config.value("App/FaceLinkCollRows").toInt();
    dataCols_ = config.value("App/FaceLinkCollCols").toInt();
    setUserStyle(userStyle());
    getCameraInfo();
}

void FacelinkTable::setUserStyle(int s)
{
    if(s == 0){
        posL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
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
        startTimeL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        endTimeL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size: 18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        dataListW_->setStyleSheet("QListWidget{"
                                  "background: transparent;"
                                  "font: 11px;"
                                  "color: white;"
                                  "}");
        dataListW_->verticalScrollBar()->setStyleSheet(
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
        noDataW_->setUserStyle(s);
    }
    pageIndicator_->setUserStyle();
}

void FacelinkTable::resizeEvent(QResizeEvent *event)
{
    int w = (dataListW_->width() - style()->pixelMetric(QStyle::PM_ScrollBarSliderMin) - dataListW_->frameWidth() * 2 - (dataCols_ + 1) * dataListW_->spacing()) / dataCols_;
    int h = (dataListW_->height() - style()->pixelMetric(QStyle::PM_ScrollBarSliderMin)  - dataListW_->frameWidth() * 2 - (dataRows_ + 1) * dataListW_->spacing()) / dataRows_;
    itemSize_.setWidth(w);
    itemSize_.setHeight(h);

    QFontMetrics fm = dataListW_->fontMetrics();
    iconSize_.setWidth(itemSize_.width() - 4);
    iconSize_.setHeight(itemSize_.height() - fm.height() * 2 - 5);
    dataListW_->setIconSize(iconSize_);
    for(int i = 0; i < dataListW_->count(); i++){
        QListWidgetItem *item = dataListW_->item(i);
        item->setSizeHint(itemSize_);
        item->setIcon(QPixmap::fromImage(item->data(Qt::UserRole + 1).value<QImage>().scaled(dataListW_->iconSize())));
    }
    return WidgetI::resizeEvent(event);
}

bool FacelinkTable::event(QEvent *event)
{
    if(event->type() == QEvent::Show  && searchBtn_->isEnabled()){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
        slotSearchBtnClicked();
        return true;
    }
    return WidgetI::event(event);
}

void FacelinkTable::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void FacelinkTable::slotSemanticSearch(int page)
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
        pageIndicator_->setEnabled(true);
        searchBtn_->setEnabled(true);
        noDataW_->show();
    });
    connect(serviceI,&RestServiceI::sigFaceLinkDataColl,this,[this,label](RestServiceI::FaceLinkDataCollReturn &returnData){
        label->close();
        delete label;
        pageIndicator_->adjustRow();
        if(needUpdatePageInfo_){
            pageIndicator_->setPageInfo(returnData.totalPage,returnData.toatal);
            needUpdatePageInfo_ = false;
        }
        if(returnData.toatal == 0){
            noDataW_->show();
        }
        dataListW_->clear();
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime,QString,QString> > dataListVec;
        std::transform(returnData.records.begin(),returnData.records.end(),std::back_inserter(dataListVec),[this](RestServiceI::DataRectureItem &nodeV){
            return std::make_tuple(nodeV.img,nodeV.id,nodeV.cameraId,cameraMapInfo_.value(nodeV.cameraId),nodeV.time,nodeV.personId,nodeV.sceneId);
        });
        QFontMetrics fs(dataListW_->font());
        int textwidth = itemSize_.width() / (fs.height() >> 1);
        for (RestServiceI::DataRectureItem &info : returnData.records) {
            QString cameraPosStr = cameraMapInfo_.value(info.cameraId);
            if(fs.width(cameraPosStr) >  itemSize_.width()){
                cameraPosStr = cameraPosStr.left(textwidth);
                cameraPosStr.remove(cameraPosStr.count() - 4, 3);
                cameraPosStr.append("...");
            }
            QListWidgetItem *item = new QListWidgetItem(cameraPosStr + '\n' + info.time.toString("yyyy-MM-dd HH:mm:ss"));
            item->setSizeHint(itemSize_);
            item->setIcon(QPixmap::fromImage(info.img));
            item->setData(Qt::UserRole+1,info.img);
            item->setData(Qt::UserRole+2,info.id);
            item->setData(Qt::UserRole+3,info.cameraId);
            item->setData(Qt::UserRole+4,info.personId);
            item->setData(Qt::UserRole+5,info.sceneId);
            item->setToolTip(cameraMapInfo_.value(info.cameraId));
            item->setTextAlignment(Qt::AlignHCenter);
            dataListW_->addItem(item);
        }
        pageIndicator_->setEnabled(true);
        searchBtn_->setEnabled(true);
    });
    RestServiceI::FaceLinkDataCollArgs args;
    args.cameraId = curCameraId_;
    args.startT = curStartTime_;
    args.endT = curEndTime_;
    args.pageNo = page;
    args.pageSize = dataRows_ * dataCols_;
    serviceI->getFaceLinkDataColl(args);
    label->show(500);
    pageIndicator_->setEnabled(false);
    searchBtn_->setEnabled(false);
    noDataW_->hide();
    dataListW_->clear();
}

void FacelinkTable::slotSearchBtnClicked()
{
    curCameraId_ = posCombox_->currentData().toString();
    curStartTime_ = startTimeEdit_->dateTime();
    curEndTime_ = endTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    slotSemanticSearch(1);
}

void FacelinkTable::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    posCombox_->clear();
    posCombox_->addItem(tr("Unlimited"),"");
    for (auto &info : data) {
        posCombox_->addItem(info.cameraPos,info.cameraId);
        cameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}
