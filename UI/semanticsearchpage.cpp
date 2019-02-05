#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTreeWidget>
#include <QListWidget>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QComboBox>
#include <QScrollBar>
#include <QHeaderView>
#include <QSpinBox>
#include <QDir>
#include <QMenu>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include "semanticsearchpage.h"
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facelinkpage.h"
#include "trackingpage.h"
#include "portrait.h"
#include "facesearch.h"
#include "service/restservice.h"

SemanticSearchPage::SemanticSearchPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Semantic search"));
    posL_ = new QLabel(tr("Position"));
    posCombox_ = new QComboBox;
    startTimeL_ = new QLabel(tr("Starting time"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeL_ = new QLabel(tr("Ending time"));
    endTimeEdit_ = new QDateTimeEdit;
    searchBtn_ = new QPushButton(tr("Search"));
    attributTreeW_ = new QTreeWidget;
    dataListW_ = new QListWidget;
    centeVSplieL_ = new QLabel;
    pageIndicator_ = new PageIndicator;
    dataMenu_ = new QMenu(dataListW_);

    QVBoxLayout *vlay = new QVBoxLayout;
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
    vlay->addLayout(topGridLay);
    vlay->addWidget(dataListW_);
    vlay->addWidget(pageIndicator_);
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addLayout(vlay);
    mainLay->addWidget(centeVSplieL_);
    mainLay->addWidget(attributTreeW_);
    setLayout(mainLay);

    attributTreeW_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    dataMenu_->addAction(tr("Details"),[this]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            QMessageBox::information(this,tr("Details"),str);
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigPeronsDetails,this,[this,label](QImage face,QImage body,QStringList faceAttr,QStringList bodyAttr){
            label->close();
            delete label;
            Portrait *detailsW = new Portrait(widgetManger(),this);
            detailsW->setAttribute(Qt::WA_DeleteOnClose);
            detailsW->setWindowFlags(Qt::Window | Qt::Dialog);
            detailsW->setWindowModality(Qt::ApplicationModal);
            detailsW->setUserStyle(widgetManger()->currentStyle());
            detailsW->slotSetData(face,body,faceAttr,bodyAttr);
            detailsW->show();
            dataMenu_->setEnabled(true);
        });
        serviceI->getPersonDetails(dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Scene analysis"),[this]{
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            QMessageBox::information(this,tr("Scene"),str);
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneImage,this,[this,label](const QImage img){
            label->close();
            delete label;
            SceneImageDialog dialog;
            dialog.setUserStyle(widgetManger()->currentStyle());
            dialog.setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
            dialog.setImage(img,dataListW_->currentItem()->data(Qt::UserRole + 5).toString());
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
        serviceI->getScenePic(dataListW_->currentItem()->data(Qt::UserRole + 5).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Search using the image"),[this]{
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
        faceDialog->setFaceImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceDialog->setOid(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    dataMenu_->addAction(tr("Tracking"),[this]{
        TrackingPage *view = new TrackingPage(widgetManger());
        view->setUserStyle(widgetManger()->currentStyle());
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setWindowFlags(Qt::Window | Qt::Dialog);
        view->setWindowModality(Qt::ApplicationModal);
        view->setMinimumSize(1655,924);
        view->setImgageOid(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>(),
                           dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        view->show();
    });
    dataMenu_->addAction(tr("Face link"),[this]{
        FaceLinkPage *faceLinkP = new FaceLinkPage(widgetManger(),this);
        faceLinkP->setUserStyle(widgetManger()->currentStyle());
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
            QMessageBox::information(this,tr("Save face image"),tr("Operation failed!"));
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
    dataListW_->setIconSize(QSize(112,112));
    dataListW_->setResizeMode(QListWidget::Adjust);
    dataListW_->setViewMode(QListWidget::IconMode);
    centeVSplieL_->setFixedWidth(1);
    QVector<itemData> devicesVec;
    itemData items;
    items.name = tr("faceAttribute");
    items.value = 0;
    items.childrens << itemData{tr("5_o_Clock_Shadow "),0,QVector<itemData>()} << itemData{tr("Arched_Eyebrows"),0,QVector<itemData>()}
                    << itemData{tr("Attractive"),0,QVector<itemData>()} << itemData{tr("Bags_Under_Eyes"),0,QVector<itemData>()}
                    << itemData{tr("Bald"),0,QVector<itemData>()} << itemData{tr("Bangs"),0,QVector<itemData>()}
                    << itemData{tr("Big_Lips"),0,QVector<itemData>()} << itemData{tr("Big_Nose"),0,QVector<itemData>()}
                    << itemData{tr("Black_Hair"),0,QVector<itemData>()} << itemData{tr("Blond_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Blurry"),0,QVector<itemData>()} << itemData{tr("Brown_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Bushy_Eyebrows"),0,QVector<itemData>()} << itemData{tr("Chubby"),0,QVector<itemData>()}
                    << itemData{tr("Double_Chin"),0,QVector<itemData>()} << itemData{tr("Eyeglasses"),0,QVector<itemData>()}
                    << itemData{tr("Goatee"),0,QVector<itemData>()} << itemData{tr("Gray_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Heavy_Makeup"),0,QVector<itemData>()} << itemData{tr("High_Cheekbones"),0,QVector<itemData>()}
                    << itemData{tr("Male/Female"),0,QVector<itemData>()} << itemData{tr("Mouth_Slightly_Open"),0,QVector<itemData>()}
                    << itemData{tr("Mustache"),0,QVector<itemData>()} << itemData{tr("Narrow_Eyes"),0,QVector<itemData>()}
                    << itemData{tr("No_Beard"),0,QVector<itemData>()} << itemData{tr("Oval_Face"),0,QVector<itemData>()}
                    << itemData{tr("Pale_Skin"),0,QVector<itemData>()} << itemData{tr("Pointy_Nose"),0,QVector<itemData>()}
                    << itemData{tr("Receding_Hairline"),0,QVector<itemData>()} << itemData{tr("Rosy_Cheeks"),0,QVector<itemData>()}
                    << itemData{tr("Sideburns"),0,QVector<itemData>()} << itemData{tr("Smiling"),0,QVector<itemData>()}
                    << itemData{tr("Straight_Hair"),0,QVector<itemData>()} << itemData{tr("Wavy_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Wearing_Earrings"),0,QVector<itemData>()} << itemData{tr("Wearing_Hat"),0,QVector<itemData>()}
                    << itemData{tr("Wearing_Lipstick"),0,QVector<itemData>()} << itemData{tr("Young"),0,QVector<itemData>()};
    devicesVec << items;
    for(auto value : devicesVec){
        createTreeItem(attributTreeW_,nullptr,value);
    }
    attributTreeW_->expandItem(attributTreeW_->topLevelItem(0));
    attributTreeW_->setHeaderLabel(tr("Attribule labels"));
    attributTreeW_->headerItem()->setTextAlignment(0,Qt::AlignCenter);
    attributTreeW_->header()->setStretchLastSection(true);
    attributTreeW_->header()->setIconSize(QSize(50,50));
    QSize s = attributTreeW_->headerItem()->sizeHint(0);
    attributTreeW_->headerItem()->setSizeHint(0,QSize(s.width(),30));
    posCombox_->setMinimumHeight(44);
    posCombox_->setMaximumWidth(390);
    startTimeEdit_->setMinimumHeight(44);
    startTimeEdit_->setMinimumWidth(250);
    startTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setMinimumHeight(44);
    endTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    endTimeEdit_->setMinimumWidth(250);
    searchBtn_->setMinimumSize(120,44);
    pageIndicator_->setPageInfo(0,0);

    connect(pageIndicator_,SIGNAL(sigPageClicked(int)),this,SLOT(slotSemanticSearch(int)));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    getCameraInfo();
}

void SemanticSearchPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    if(WidgetManagerI::Danyahei == s){
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
                                 "color: white;"
                                 "background-color: rgba(112, 112, 112, 1);"
                                 "}");
        dataListW_->setStyleSheet("QListWidget{"
                                  "background: transparent;"
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
        centeVSplieL_->setStyleSheet("QLabel{"
                                     "background-color: rgba(255,255,255,0.4);"
                                     "}");
        dataMenu_->setStyleSheet("QMenu{"
                                 "background-color: rgb(75,75,75);"
                                 "}"
                                 "QMenu::item:selected{"
                                 "background-color: rgba(255,255,255,0.4);"
                                 "}");
        attributTreeW_->setStyleSheet("QTreeView{"
                               "border:none;"
                               "font-size: 16px;"
                               "color: #CECECE;"
                               "border-radius: 10px;"
                               "background-color: transparent;}");
        attributTreeW_->verticalScrollBar()->setStyleSheet(
                                                    "QScrollBar:vertical{"
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
        attributTreeW_->header()->setStyleSheet("QHeaderView{"
                                         "background-color: transparent;"
                                         "border-radius:10px;"
                                         "}"
                                         "QHeaderView::section{"
                                         "color: #CECECE;"
                                         "font-size:16px;"
                                         "border:none;"
                                         "background-color: transparent;"
                                         "}");
    }
    pageIndicator_->setUserStyle();
}

void SemanticSearchPage::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void SemanticSearchPage::slotSemanticSearch(int page)
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        QMessageBox::information(this,objectName(),str);
        pageIndicator_->setEnabled(true);
        searchBtn_->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigSemanticSearch,this,[this,label](RestServiceI::SemanticReturnData &returnData){
        label->close();
        dataListW_->clear();
        pageIndicator_->adjustRow();
        if(needUpdatePageInfo_){
            pageIndicator_->setPageInfo(returnData.totalPage,returnData.toatal);
            needUpdatePageInfo_ = false;
        }
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime,QString,QString> > dataListVec;
        std::transform(returnData.records.begin(),returnData.records.end(),std::back_inserter(dataListVec),[this](RestServiceI::DataRectureItem &nodeV){
            return std::make_tuple(nodeV.img,nodeV.id,nodeV.cameraId,cameraMapInfo_.value(nodeV.cameraId),nodeV.time,nodeV.personId,nodeV.sceneId);
        });
        for (RestServiceI::DataRectureItem &info : returnData.records) {
            QString posText = cameraMapInfo_.value(info.cameraId);
            QListWidgetItem *item = new QListWidgetItem(posText.left(16) + '\n' + info.time.toString("yyyy-MM-dd HH:mm:ss"));
            item->setIcon(QPixmap::fromImage(info.img));
            item->setData(Qt::UserRole+1,info.img);
            item->setData(Qt::UserRole+2,info.id);
            item->setData(Qt::UserRole+3,info.cameraId);
            item->setData(Qt::UserRole+4,info.personId);
            item->setData(Qt::UserRole+5,info.sceneId);
            item->setToolTip(posText);
            item->setTextAlignment(Qt::AlignHCenter);
            dataListW_->addItem(item);
        }
        pageIndicator_->setEnabled(true);
        searchBtn_->setEnabled(true);
    });
    RestServiceI::SemanticSearchArgs args;
    args.cameraId = curCameraId_;
    args.mode = 0;
    args.startT = curStartTime_;
    args.endT = curEndTime_;
    args.pageNo = page;
    args.pageSize = 40;
    args.faceAttributList = curfaceAttrList_;
    serviceI->semanticSearch(args);
    startWorker(worker);
    label->show(500);
    pageIndicator_->setEnabled(false);
    searchBtn_->setEnabled(false);
}

QStringList SemanticSearchPage::checkedAttrbute(QTreeWidgetItem *item)
{
    QStringList attrbuteList;
    QTreeWidgetItemIterator it(item);
    while (*it) {
        if ((*it)->type() && (*it)->checkState(0) == Qt::Checked){
            attrbuteList << (*it)->text(0);
        }
        ++it;
    }
    return attrbuteList;
}

void SemanticSearchPage::slotSearchBtnClicked()
{
    curfaceAttrList_.clear();
    curfaceAttrList_ = checkedAttrbute(attributTreeW_->topLevelItem(0));
    curCameraId_ = posCombox_->currentData().toString();
    curStartTime_ = startTimeEdit_->dateTime();
    curEndTime_ = endTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    slotSemanticSearch(1);
}

void SemanticSearchPage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    posCombox_->clear();
    posCombox_->addItem(tr("Unlimited"),"");
    for (auto &info : data) {
        posCombox_->addItem(info.cameraPos,info.cameraId);
        cameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}

void SemanticSearchPage::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, SemanticSearchPage::itemData &items)
{
    QTreeWidgetItem *item{nullptr};
    if(parentItem){
        item = new QTreeWidgetItem(parentItem, QStringList() << items.name, items.childrens.isEmpty());
    }else{
        item = new QTreeWidgetItem(treeW, QStringList() << items.name, items.childrens.isEmpty());
    }
    if(item->type()){
        item->setCheckState(0,Qt::Unchecked);
    }
    for(auto value : items.childrens){
        createTreeItem(treeW,item,value);
    }
}
