#include <QStackedWidget>
#include <QHBoxLayout>
#include <QListWidget>
#include <QDebug>
#include <QPainter>
#include <QTreeWidget>
#include <QPushButton>
#include <QDateTimeEdit>
#include <QPaintEvent>
#include <QLabel>
#include <QSpinBox>
#include <QComboBox>
#include <QApplication>
#include <QHeaderView>
#include <QFileDialog>
#include <QMessageBox>
#include <QMenu>
#include <QGraphicsView>
#include <QStandardPaths>
#include <QScrollBar>
#include <QApplication>
#include "targetsearch.h"
#include "waitinglabel.h"
#include "pageindicator.h"
#include "sceneimagedialog.h"
#include "service/restservice.h"
#include "cameramapscene.h"
#include "treecharts.h"
#include "trackingpage.h"
#include "facelinkpage.h"
#include "portrait.h"

#pragma execution_character_set("utf-8")
TargetSearch::TargetSearch(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("目标检索"));
    backImg_.load("images/Mask.png");
    //search using an image
    imagePosL_ = new QLabel(tr("Position"));
    imagePosCombox_ = new QComboBox;
    imageSmilarL_ = new QLabel("smilarty");
    imageSmilarSpin_ = new QSpinBox;
    imageCountL_ = new QLabel("count");
    imageCountCombox_ = new QComboBox;
    imageBtn_ = new QPushButton;
    imageStartTL_ = new QLabel(tr("Starting time"));
    imageStartTimeEdit_ = new QDateTimeEdit;
    imageEndTL_ = new QLabel(tr("Ending time"));
    imageEndTimeEdit_ = new QDateTimeEdit;
    imageSearchBtn_ = new QPushButton(tr("Search"));
    imageConditionBackW_ = new QWidget;
    QHBoxLayout *imageMainLay = new QHBoxLayout;
    QGridLayout *imageGridLay = new QGridLayout;
    imageGridLay->addWidget(imagePosL_,0,0,1,1);
    imageGridLay->addWidget(imagePosCombox_,0,1,1,1);
    imageGridLay->addWidget(imageCountL_,0,2,1,1);
    imageGridLay->addWidget(imageCountCombox_,0,3,1,1);
    imageGridLay->addWidget(imageSmilarL_,0,4,1,1);
    imageGridLay->addWidget(imageSmilarSpin_,0,5,1,1);
    imageGridLay->addWidget(imageStartTL_,1,0,1,1);
    imageGridLay->addWidget(imageStartTimeEdit_,1,1,1,1);
    imageGridLay->addWidget(imageEndTL_,1,2,1,1);
    imageGridLay->addWidget(imageEndTimeEdit_,1,3,1,1);
    imageGridLay->addWidget(imageSearchBtn_,1,5,1,1);
    imageGridLay->setAlignment(Qt::AlignLeft);
    imageGridLay->setSpacing(25);
    imageMainLay->addWidget(imageBtn_);
    imageMainLay->addLayout(imageGridLay);
    imageMainLay->setSpacing(30);
    imageMainLay->setMargin(0);
    imageConditionBackW_->setLayout(imageMainLay);
    imageConditionBackW_->setObjectName(tr("Search using an image"));

    //semantic search
    semanticPosL_ = new QLabel(tr("Position"));
    semanticPosCombox_ = new QComboBox;
    semanticStartTimeL_ = new QLabel(tr("Starting time"));
    semanticStartTimeEdit_ = new QDateTimeEdit;
    semanticEndTimeL_ = new QLabel(tr("Ending time"));
    semanticEndTimeEdit_ = new QDateTimeEdit;
    semanticSearchBtn_ = new QPushButton(tr("Search"));
    semanticConditionBacwW_ = new QWidget;
    QGridLayout *semanticMLay = new QGridLayout;
    semanticMLay->addWidget(semanticPosL_,0,0,1,1);
    semanticMLay->addWidget(semanticPosCombox_,0,1,1,1);
    semanticMLay->addWidget(semanticStartTimeL_,1,0,1,1);
    semanticMLay->addWidget(semanticStartTimeEdit_,1,1,1,1);
    semanticMLay->addWidget(semanticEndTimeL_,1,2,1,1);
    semanticMLay->addWidget(semanticEndTimeEdit_,1,3,1,1);
    semanticMLay->addWidget(semanticSearchBtn_,1,4,1,1);
    semanticMLay->setAlignment(Qt::AlignLeft);
    semanticMLay->setSpacing(25);
    semanticMLay->setMargin(0);
    semanticConditionBacwW_->setLayout(semanticMLay);
    semanticConditionBacwW_->setObjectName(tr("Semantic search"));

    //main page controls
    leftBarBack_ = new QWidget;
    treeW_ = new QTreeWidget;
    attributTreeW_ = new QTreeWidget;
    conditionListW_ = new QListWidget;
    dataListW_ = new QListWidget;
    dataVSplitL_ = new QLabel;
    centeVSplieL_ = new QLabel;
    stackedW_ = new QStackedWidget;
    stackedW_->installEventFilter(this);
    pageIndicator_ = new PageIndicator;
    centerBackW_ = new QWidget;
    dataMenu_ = new QMenu;
    QHBoxLayout *mainLay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(treeW_);
    vlay->setAlignment(Qt::AlignTop);
    vlay->setContentsMargins(30,45,30,0);
    leftBarBack_->setLayout(vlay);
    leftBarBack_->installEventFilter(this);
    mainLay->addWidget(leftBarBack_,33);

    vlay = new QVBoxLayout;
    vlay->addWidget(conditionListW_,1);
    vlay->addWidget(centeVSplieL_,0);
    QVBoxLayout *dataVlay = new QVBoxLayout;
    dataVlay->addWidget(stackedW_,3);
    dataVlay->addWidget(dataListW_,13);
    dataVlay->addWidget(pageIndicator_);
    dataVlay->setContentsMargins(0,20,0,0);
    dataVlay->setSpacing(20);
    dataHlay_ = new QHBoxLayout;
    dataHlay_->addLayout(dataVlay,11);
    dataHlay_->addWidget(dataVSplitL_);
    dataHlay_->addWidget(attributTreeW_,2);
    dataHlay_->setSpacing(20);
    dataHlay_->setMargin(0);
    vlay->addLayout(dataHlay_,10);
    vlay->setContentsMargins(27,0,27,0);
    vlay->setSpacing(0);
    centerBackW_->setLayout(vlay);
    centerBackW_->installEventFilter(this);
    mainLay->addWidget(centerBackW_,155);
    mainLay->setContentsMargins(20,30,20,25);
    mainLay->setSpacing(20);
    setLayout(mainLay);

    QVector<itemData> devicesVec;
    itemData items;
    items.name = tr("Face search");
    items.childrens << itemData{tr("Query in the face database"),QVector<itemData>()} << itemData{tr("Query in the capture database"),QVector<itemData>()};
    devicesVec << items;
    items.childrens.clear();
    items.name = tr("Combinatorial search");
    devicesVec << items;
    items.childrens.clear();
    items.name = tr("Object search");
    items.childrens << itemData{tr("Lost property tracking"),QVector<itemData>()};
    devicesVec << items;
    items.name = tr("Vehicle tracking");
    items.childrens.clear();
    items.childrens << itemData{tr("Large Vehicles"),QVector<itemData>()} << itemData{tr("Small Vehicles"),QVector<itemData>()};
    devicesVec << items;
    for(auto value : devicesVec){
        createTreeItem(treeW_,nullptr,value);
    }
    treeW_->expandAll();
    if(treeW_->topLevelItemCount() && treeW_->topLevelItem(0)->childCount() >= 2){
        treeW_->setCurrentItem(treeW_->topLevelItem(0)->child(1),true);
    }

    devicesVec.clear();
    items.name = tr("Eye");
    items.childrens.clear();
    items.childrens << itemData{tr("Arched_Eyebrows"),QVector<itemData>()} << itemData{tr("Bushy_Eyebrows"),QVector<itemData>()}
                    << itemData{tr("Babs_Under_Eyes"),QVector<itemData>()} << itemData{tr("Eyeglasses"),QVector<itemData>()}
                    << itemData{tr("Narrow_Eyes"),QVector<itemData>()};
    devicesVec << items;
    items.childrens.clear();
    items.name = tr("Hair");
    items.childrens << itemData{tr("Yellow"),QVector<itemData>()} << itemData{tr("black"),QVector<itemData>()};
    devicesVec << items;
    items.childrens.clear();
    items.name = tr("Lips");
    items.childrens << itemData{tr("lip"),QVector<itemData>()};
    devicesVec << items;
    items.name = tr("Nose");
    items.childrens.clear();
    items.childrens << itemData{tr("Big"),QVector<itemData>()} << itemData{tr("Small"),QVector<itemData>()};
    devicesVec << items;
    items.name = tr("Chine");
    items.childrens.clear();
    items.childrens << itemData{tr("Big"),QVector<itemData>()} << itemData{tr("Small"),QVector<itemData>()};
    devicesVec << items;
    items.name = tr("Other");
    items.childrens.clear();
    items.childrens << itemData{tr("Big"),QVector<itemData>()} << itemData{tr("Small"),QVector<itemData>()};
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

    //main page setting
    treeW_->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    treeW_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    treeW_->setHeaderHidden(true);
    centeVSplieL_->setFixedHeight(1);
    dataVSplitL_->setFixedWidth(1);
    imageSmilarSpin_->setRange(0,100);
    imageSmilarSpin_->setSuffix("%");
    imageSmilarSpin_->setMinimumHeight(44);
    imageCountCombox_->setMinimumHeight(44);
    imageCountCombox_->setIconSize(QSize(1,30));;
    conditionListW_->setFlow(QListWidget::LeftToRight);
    conditionListW_->setFocusPolicy(Qt::NoFocus);
    conditionListW_->setFrameStyle(QFrame::NoFrame);
    dataListW_->setFocusPolicy(Qt::NoFocus);
    dataListW_->setMovement(QListWidget::Static);
    dataListW_->setIconSize(QSize(112,112));
    dataListW_->setResizeMode(QListWidget::Adjust);
    dataListW_->setViewMode(QListWidget::IconMode);
    pageIndicator_->setPageInfo(0,0);
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
        connect(serviceI,&RestServiceI::sigPeronsDetails,this,[this,label](QImage face,QImage body,QStringList attribute){
            label->close();
            delete label;
            Portrait *detailsW = new Portrait(widgetManger(),this);
            detailsW->setAttribute(Qt::WA_DeleteOnClose);
            detailsW->setWindowFlags(Qt::Window | Qt::Dialog);
            detailsW->setWindowModality(Qt::ApplicationModal);
            detailsW->setUserStyle(widgetManger()->currentStyle());
            detailsW->slotSetData(face,body,attribute);
            detailsW->show();
            dataMenu_->setEnabled(true);
        });
        serviceI->getPersonDetails(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
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
            dialog.setImage(img);
            dialog.setRectLinePen(Qt::yellow);
            connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
                if(!images.count()){
                    return;
                }
                QPixmap pix = QPixmap::fromImage(images.first());
                imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
                imageBtn_->setProperty("pixmap",pix);
                slotImageSearchBtnClicked();
            });
            dialog.exec();
            dataMenu_->setEnabled(true);
        });
        serviceI->getScenePic(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Search using the image"),[this]{
#if 1
        imageCurOid_ = dataListW_->currentItem()->data(Qt::UserRole + 2).toString();
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",pix);
#else
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",pix);
#endif
        slotImageSearchBtnClicked();
    });
    dataMenu_->addAction(tr("Tracking"),[this]{
#if 0
        QGraphicsView *cameraView = new QGraphicsView(this);
        CameraMapScene *cameraScene = new CameraMapScene;
        cameraView->setScene(cameraScene);
        cameraView->setAttribute(Qt::WA_DeleteOnClose);
        cameraView->setWindowFlags(Qt::Window | Qt::Dialog);
        cameraView->setWindowModality(Qt::ApplicationModal);
        cameraView->show();
#else
        TrackingPage *view = new TrackingPage(widgetManger());
        view->setUserStyle(widgetManger()->currentStyle());
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setWindowFlags(Qt::Window | Qt::Dialog);
        view->setWindowModality(Qt::ApplicationModal);
        view->setMinimumSize(1655,924);
        view->setImgageOid(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>(),
                           dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        view->show();
#endif
    });
    dataMenu_->addAction(tr("Face link"),[this]{
#if 1
        FaceLinkPage *faceLinkP = new FaceLinkPage(widgetManger(),this);
        faceLinkP->setUserStyle(widgetManger()->currentStyle());
        faceLinkP->setAttribute(Qt::WA_DeleteOnClose);
        faceLinkP->setWindowFlags(Qt::Window | Qt::Dialog);
        faceLinkP->setWindowModality(Qt::ApplicationModal);
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceLinkP->setFaceLinkOidAndImg(dataListW_->currentItem()->data(Qt::UserRole + 2).toString(),pix);
//        faceLinkP->resize(1200,900);
        faceLinkP->show();
#else
        TreeCharts *dataTreeW = new TreeCharts(this);
        dataTreeW->setAttribute(Qt::WA_DeleteOnClose);
        dataTreeW->setWindowFlags(Qt::Window | Qt::Dialog);
        dataTreeW->setWindowModality(Qt::ApplicationModal);
        dataTreeW->show();
#endif
    });
    dataListW_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dataListW_,&QListWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!dataListW_->itemAt(p))return;
        dataMenu_->move(QCursor::pos());
        dataMenu_->show();
    });
    //using an image settting
    imageBtn_->setFixedSize(150,150);
    QCursor imageBtncursor =imageBtn_->cursor();
    imageBtncursor.setShape(Qt::PointingHandCursor);
    imageBtn_->setCursor(imageBtncursor);
    imageBtn_->setIconSize(imageBtn_->size());
    imageBtn_->setFocusPolicy(Qt::NoFocus);
    imagePosCombox_->setMinimumHeight(44);
    imageStartTimeEdit_->setMinimumSize(250,44);
    imageEndTimeEdit_->setMinimumSize(250,44);
    imageSearchBtn_->setMinimumSize(120,44);
    imageStartTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    imageEndTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    imageStartTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    imageEndTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    imagePosCombox_->setMaximumWidth(290);
    QPixmap defaultPersonBackPix("images/person-face-back.png");
    imageBtn_->setIcon(defaultPersonBackPix);
    imageBtn_->setProperty("default-pix",defaultPersonBackPix);
    //semantic search
    semanticPosCombox_->setMinimumHeight(44);
    semanticStartTimeEdit_->setMinimumSize(250,44);
    semanticEndTimeEdit_->setMinimumSize(250,44);
    semanticSearchBtn_->setMinimumSize(120,44);
    semanticStartTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    semanticEndTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    semanticStartTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    semanticEndTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    semanticPosCombox_->setMaximumWidth(290);

    QVector<QPair<QString,int>> itemVec{qMakePair(tr("20 records"),20),
                qMakePair(tr("50 records"),50),qMakePair(tr("100 records"),100),
                qMakePair(tr("200 records"),200),qMakePair(tr("300 records"),300),
                qMakePair(tr("500 records"),500)};
    QPixmap pix(1,imageCountCombox_->iconSize().height());
    pix.fill(Qt::transparent);
    for(const QPair<QString,int> &value : itemVec){
        imageCountCombox_->addItem(pix,value.first,value.second);
    }
    new QListWidgetItem(semanticConditionBacwW_->objectName(),conditionListW_);
    stackedW_->addWidget(semanticConditionBacwW_);
    new QListWidgetItem(imageConditionBackW_->objectName(),conditionListW_);
    stackedW_->addWidget(imageConditionBackW_);

    connect(conditionListW_,SIGNAL(currentRowChanged(int)),this,SLOT(slotConditionListIndexChanged(int)));
    connect(imageSearchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImageSearchBtnClicked()));
    connect(semanticSearchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSemanticSearchBtnClicked()));
    connect(imageBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImageFaceImgBtnClicked()));
    connect(pageIndicator_,SIGNAL(sigPageClicked(int)),this,SLOT(slotNextPage(int)));
    conditionListW_->setCurrentRow(0);
    imageSmilarSpin_->setValue(40);
    imageCountCombox_->setCurrentIndex(0);

    getCameraInfo();
}

void TargetSearch::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(s == WidgetManagerI::Danyahei){
        //main
        treeW_->setStyleSheet("QTreeView{"
                              "alternate-background-color: yellow;"
                              "show-decoration-selected:1;"
                              "background: transparent;"
                              "border-radius: 0px;"
                              "font-size: 16px;"
                              "}"
                              "QTreeView::item{"
                              "color: rgba(206, 206, 206, 1);"
                              "}"
                              "QTreeView::item:selected:active{"
                              "color: rgba(255, 255, 255, 1);"
                              "}"
                              "QTreeView::item:selected:!active{"
                              "color: rgba(255, 255, 255, 1);"
                              "}"
                              "QTreeView::branch:has-children:!has-siblings:closed,"
                              "QTreeView::branch:closed:has-children:has-siblings {"
                              "border-image: none;"
                              "image: url(images/tree_unexpand.png);"
                              "}"
                              "QTreeView::branch:open:has-children:!has-siblings,"
                              "QTreeView::branch:open:has-children:has-siblings  {"
                              "border-image: none;"
                              "image: url(images/tree_expand.png);"
                              "}");
        imageSmilarL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "border-radius: none;"
                                "font-size: 16px;"
                                "}");
        imageSmilarSpin_->setStyleSheet("QSpinBox{"
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
        imageCountL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        imageCountCombox_->setStyleSheet(
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
        conditionListW_->setStyleSheet("QListWidget{"
                                 "background-color: transparent;"
                                 "}"
                                 "QListWidget::item{"
                                 "color: rgb(206, 206, 206);"
                                 "}"
                                 "QListWidget::item:selected{"
                                 "color: white;"
                                 "border-image: url(images/item_selected.png);;"
                                 "}");
        centeVSplieL_->setStyleSheet("QLabel{"
                                     "background-color: rgba(255,255,255,0.1);"
                                     "}");
        pal = dataListW_->palette();
        pal.setColor(QPalette::Base,Qt::transparent);
        pal.setColor(QPalette::Text,Qt::white);
        dataListW_->setPalette(pal);
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
        dataVSplitL_->setStyleSheet("QLabel{"
                                     "background-color: rgba(255,255,255,0.4);"
                                     "}");
        dataMenu_->setStyleSheet("QMenu{"
                                 "background-color: rgb(75,75,75);"
                                 "}"
                                 "QMenu::item:selected{"
                                 "background-color: rgba(255,255,255,0.4);"
                                 "}");
        //using an image
        imageBtn_->setStyleSheet("QPushButton{"
                                 "background-color: transparent;"
                                 "}");
        imagePosL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        imagePosCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: 1px solid #CECECE;"
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
        imageStartTL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        imageStartTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        imageEndTL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        imageEndTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        imageSearchBtn_->setStyleSheet("QPushButton{"
                                 "color: white;"
                                 "background-color: rgba(112, 112, 112, 1);"
                                 "}");
        //semantic search
        semanticPosL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        semanticPosCombox_->setStyleSheet(
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
        semanticStartTimeL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        semanticStartTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        semanticEndTimeL_->setStyleSheet("QLabel{"
                                "background-color: transparent;"
                                "color: rgba(206, 206, 206, 1);"
                                "font-size: 16px;"
                                "border-radius: none;"
                                "}");
        semanticEndTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        semanticSearchBtn_->setStyleSheet("QPushButton{"
                                 "color: white;"
                                 "background-color: rgba(112, 112, 112, 1);"
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

bool TargetSearch::eventFilter(QObject *watched, QEvent *event)
{
    if(qobject_cast<QWidget*>(watched) == leftBarBack_ && event->type() == QEvent::Paint){
        QPainter p(leftBarBack_);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0, 0, 0, 90));
        p.drawRoundedRect(leftBarBack_->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
        return true;
    }else if(qobject_cast<QWidget*>(watched) == centerBackW_ && event->type() == QEvent::Paint){
        QPainter p(centerBackW_);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,50));
        p.drawRoundedRect(centerBackW_->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return QWidget::eventFilter(watched,event);
}

void TargetSearch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

bool TargetSearch::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        imageEndTimeEdit_->setDateTime(QDateTime::currentDateTime());
        semanticEndTimeEdit_->setDateTime(QDateTime::currentDateTime());
        return true;
    }
    return WidgetI::event(event);
}

void TargetSearch::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, TargetSearch::itemData &items)
{
    QTreeWidgetItem *item{nullptr};
    if(parentItem){
        item = new QTreeWidgetItem(parentItem, QStringList() << items.name, items.childrens.isEmpty());
    }else{
        item = new QTreeWidgetItem(treeW, QStringList() << items.name, items.childrens.isEmpty());
    }
    if(item->type() && treeW == attributTreeW_){
        item->setCheckState(0,Qt::Unchecked);
    }
    for(auto value : items.childrens){
        createTreeItem(treeW,item,value);
    }
}

void TargetSearch::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void TargetSearch::updateDataList(int totalRecords, int totalPages, QVector<std::tuple<QImage, QString, QString, QString, QDateTime> > &data)
{
    dataListW_->clear();
    pageIndicator_->adjustRow();
    if(needUpdatePageInfo_){
        pageIndicator_->setPageInfo(totalPages,totalRecords);
        needUpdatePageInfo_ = false;
    }
    foreach (auto &info, data) {
        QListWidgetItem *item = new QListWidgetItem(std::get<3>(info) + '\n' + std::get<4>(info).toString("yyyy-MM-dd HH:mm:ss"));
        QImage img = std::get<0>(info);
        item->setIcon(QPixmap::fromImage(img));
        item->setData(Qt::UserRole+1,img);
        item->setData(Qt::UserRole+2,std::get<1>(info));
        item->setData(Qt::UserRole+3,std::get<2>(info));
        item->setTextAlignment(Qt::AlignHCenter);
        dataListW_->addItem(item);
    }
}

void TargetSearch::slotConditionListIndexChanged(int index)
{
//    switch (index) {
//    case 0:
//        attributTreeW_->hide();
//        dataVSplitL_->hide();
//        break;
//    case 1:
//        attributTreeW_->show();
//        dataVSplitL_->show();
//        break;
//    case 2:
//        attributTreeW_->hide();
//        dataVSplitL_->hide();
//        break;
//    default:
//        break;
//    }
    stackedW_->setCurrentIndex(index);
}

void TargetSearch::slotImageFaceImgBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("添加图片"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    QPixmap pix(filePath);
    if(pix.isNull()){
        imageBtn_->setIcon(imageBtn_->property("default-pix").value<QPixmap>());
        return;
    }
    imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
    imageBtn_->setProperty("pixmap",pix);
}

void TargetSearch::slotImageSearchBtnClicked()
{
    QImage faceImg = imageBtn_->property("pixmap").value<QPixmap>().toImage();
    if(faceImg.isNull() && imageCurOid_.isEmpty()){
        QMessageBox::information(this,imageConditionBackW_->objectName(),tr("need image or oid"));
        return;
    }
    if(!treeW_->currentItem()->type()){
        QMessageBox::information(this,imageConditionBackW_->objectName(),tr("please select database"));
        return;
    }

    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
        label->close();
        imageCurOid_.clear();
        QMessageBox::information(this,imageConditionBackW_->objectName(),str);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigFaceSearch,this,[this,label](const QVector<SearchFace> value){
        label->close();
        imageCurOid_.clear();
        curSearchType_ = 0;
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime> > dataListVec;
        std::transform(value.begin(),value.end(),std::back_inserter(dataListVec),[](const SearchFace &v){
            QImage img;
            img.loadFromData(QByteArray::fromStdString(v.face_img));
            return std::make_tuple(img,QString::fromStdString(v.oid),QString::fromStdString(v.camera_id),
                            QString::fromStdString(v.camera_pos),
                            QDateTime::fromMSecsSinceEpoch(v.time));
        });
        needUpdatePageInfo_ = true;
        updateDataList(value.count(),1,dataListVec);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    if(imageCurOid_.isEmpty()){
        serviceI->searchSnap(treeW_->currentItem()->text(0),faceImg,"",imagePosCombox_->currentData().toString(),
                             imageCountCombox_->currentData().toInt(),
                             imageSmilarSpin_->value() / (qreal)100,imageStartTimeEdit_->dateTime(),
                             imageEndTimeEdit_->dateTime());
    }else{
        serviceI->searchSnap(treeW_->currentItem()->text(0),QImage(),imageCurOid_,imagePosCombox_->currentData().toString(),
                             imageCountCombox_->currentData().toInt(),
                             imageSmilarSpin_->value() / (qreal)100,imageStartTimeEdit_->dateTime(),
                             imageEndTimeEdit_->dateTime());
    }
    startWorker(worker);
    label->show(500);
    stackedW_->setEnabled(false);
    pageIndicator_->setEnabled(false);
}

void TargetSearch::slotSemanticSearchBtnClicked()
{
//    TrackingWebView *view = new TrackingWebView(this);
//    view->setAttribute(Qt::WA_DeleteOnClose);
//    view->setWindowFlags(Qt::Window | Qt::Dialog);
//    view->setWindowModality(Qt::ApplicationModal);
//    view->resize(1495,705);
//    view->show();

    if(!treeW_->currentItem()->type()){
        QMessageBox::information(this,imageConditionBackW_->objectName(),tr("please select database"));
        return;
    }
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    WaitingLabel *label = new WaitingLabel(this);
    label->setAttribute(Qt::WA_DeleteOnClose);
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        label->close();
        QMessageBox::information(this,semanticConditionBacwW_->objectName(),str);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    connect(serviceI,&RestServiceI::sigSnapHistory,this,[this,label](const PagedSnapFaceHis value){
        label->close();
        curSearchType_ = 1;
        std::vector<SnapFaceHis> resData = value.snap_face_his;
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime> > dataListVec;
        std::transform(resData.begin(),resData.end(),std::back_inserter(dataListVec),[](const SnapFaceHis &v){
            QImage img;
            img.loadFromData(QByteArray::fromStdString(v.faceimg));
            return std::make_tuple(img,QString::fromStdString(v.oid),QString::fromStdString(v.cameraid),
                            QString::fromStdString(v.camera_pos),
                            QDateTime::fromMSecsSinceEpoch(v.time));
        });
        semanticCurCameraId_ = semanticPosCombox_->currentData().toString();
        semanticCurStartTime_ = semanticStartTimeEdit_->dateTime();
        semanticCurEndTime_ = semanticEndTimeEdit_->dateTime();
        needUpdatePageInfo_ = true;
        updateDataList(value.total_count,value.total_page,dataListVec);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    serviceI->captureSearch(treeW_->currentItem()->text(0),1,semanticPageCount_,semanticPosCombox_->currentData().toString(),
                            semanticStartTimeEdit_->dateTime(),
                            semanticEndTimeEdit_->dateTime());
    startWorker(worker);
    label->show(500);
    stackedW_->setEnabled(false);
    pageIndicator_->setEnabled(false);
}

void TargetSearch::slotOnCameraInfo(QVector<CameraInfo> data)
{
    imagePosCombox_->clear();
    imagePosCombox_->addItem(tr("Unlimited"),"");
    semanticPosCombox_->addItem(tr("Unlimited"),"");
    foreach (const CameraInfo &info, data) {
        imagePosCombox_->addItem(QString::fromStdString(info.position),QString::fromStdString(info.id));
        semanticPosCombox_->addItem(QString::fromStdString(info.position),QString::fromStdString(info.id));
    }
}

void TargetSearch::slotNextPage(int page)
{
    if(curSearchType_ == 1){
        BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
        RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
        WaitingLabel *label = new WaitingLabel(this);
        label->setAttribute(Qt::WA_DeleteOnClose);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            QMessageBox::information(this,semanticConditionBacwW_->objectName(),str);
            stackedW_->setEnabled(true);
            pageIndicator_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSnapHistory,this,[this,label](const PagedSnapFaceHis value){
            label->close();
            std::vector<SnapFaceHis> resData = value.snap_face_his;
            QVector<std::tuple<QImage, QString, QString, QString, QDateTime> > dataListVec;
            std::transform(resData.begin(),resData.end(),std::back_inserter(dataListVec),[](const SnapFaceHis &v){
                QImage img;
                img.loadFromData(QByteArray::fromStdString(v.faceimg));
                return std::make_tuple(img,QString::fromStdString(v.oid),QString::fromStdString(v.cameraid),
                                QString::fromStdString(v.camera_pos),
                                QDateTime::fromMSecsSinceEpoch(v.time));
            });
            updateDataList(value.total_count,value.total_page,dataListVec);
            stackedW_->setEnabled(true);
            pageIndicator_->setEnabled(true);
        });
        serviceI->captureSearch(QString("Query in the capture database"),page,semanticPageCount_,semanticCurCameraId_,
                                semanticCurStartTime_,
                                semanticCurEndTime_);
        startWorker(worker);
        label->show(500);
        stackedW_->setEnabled(false);
        pageIndicator_->setEnabled(false);
    }
}
