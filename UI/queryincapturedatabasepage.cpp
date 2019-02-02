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
#include <QDir>
#include <QMessageBox>
#include <QMenu>
#include <QGraphicsView>
#include <QStandardPaths>
#include <QScrollBar>
#include <QApplication>
#include "queryincapturedatabasepage.h"
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
QueryInCaptureDataBasePage::QueryInCaptureDataBasePage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Query in face database"));
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
    attributTreeW_ = new QTreeWidget;
    conditionListW_ = new QListWidget;
    dataListW_ = new QListWidget;
    dataVSplitL_ = new QLabel;
    centeVSplieL_ = new QLabel;
    stackedW_ = new QStackedWidget;
    stackedW_->installEventFilter(this);
    pageIndicator_ = new PageIndicator;
    dataMenu_ = new QMenu;

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay = new QVBoxLayout;
    mainLay->addWidget(conditionListW_,1);
    mainLay->addWidget(centeVSplieL_,0);
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
    mainLay->addLayout(dataHlay_,10);
    mainLay->setContentsMargins(27,0,27,0);
    mainLay->setSpacing(0);
    setLayout(mainLay);

    QVector<itemData> devicesVec;
    itemData items;
    items.name = tr("faceAttribute");
    items.value = 0;
    items.childrens.clear();
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
#if 0
    items.childrens.clear();
    items.name = tr("personAttribute");
    items.value = 0;
    items.childrens << itemData{tr("personalLess30 "),0,QVector<itemData>()} << itemData{tr("personalLess45"),0,QVector<itemData>()}
                    << itemData{tr("personalLess60"),0,QVector<itemData>()} << itemData{tr("personalLarger60"),0,QVector<itemData>()}
                    << itemData{tr("carryingBackpack"),0,QVector<itemData>()} << itemData{tr("carryingOther"),0,QVector<itemData>()}
                    << itemData{tr("lowerBodyCasual"),0,QVector<itemData>()} << itemData{tr("upperBodyCasual"),0,QVector<itemData>()}
                    << itemData{tr("lowerBodyFormal"),0,QVector<itemData>()} << itemData{tr("upperBodyFormal"),0,QVector<itemData>()}
                    << itemData{tr("accessoryHat"),0,QVector<itemData>()} << itemData{tr("upperBodyJacket"),0,QVector<itemData>()}
                    << itemData{tr("lowerBodyJeans"),0,QVector<itemData>()} << itemData{tr("footwearLeatherShoes"),0,QVector<itemData>()}
                    << itemData{tr("upperBodyLogo"),0,QVector<itemData>()} << itemData{tr("hairLong"),0,QVector<itemData>()}
                    << itemData{tr("personalMale"),0,QVector<itemData>()} << itemData{tr("carryingMessengerBag"),0,QVector<itemData>()}
                    << itemData{tr("accessoryMuffler"),0,QVector<itemData>()} << itemData{tr("accessoryNothing"),0,QVector<itemData>()}
                    << itemData{tr("carryingNothing"),0,QVector<itemData>()} << itemData{tr("upperBodyPlaid"),0,QVector<itemData>()}
                    << itemData{tr("carryingPlasticBags"),0,QVector<itemData>()} << itemData{tr("footwearSandals"),0,QVector<itemData>()}
                    << itemData{tr("footwearShoes"),0,QVector<itemData>()} << itemData{tr("lowerBodyShorts"),0,QVector<itemData>()}
                    << itemData{tr("upperBodyShortSleeve"),0,QVector<itemData>()} << itemData{tr("lowerBodyShortSkirt"),0,QVector<itemData>()}
                    << itemData{tr("footwearSneakers"),0,QVector<itemData>()} << itemData{tr("upperBodyThinStripes"),0,QVector<itemData>()}
                    << itemData{tr("accessorySunglasses"),0,QVector<itemData>()} << itemData{tr("lowerBodyTrousers"),0,QVector<itemData>()}
                    << itemData{tr("upperBodyTshirt"),0,QVector<itemData>()} << itemData{tr("upperBodyOther"),0,QVector<itemData>()}
                    << itemData{tr("upperBodyVNeck"),0,QVector<itemData>()};
    devicesVec << items;
#endif
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
        connect(serviceI,&RestServiceI::sigPeronsDetails,this,[this,label](QImage face,QImage body,QStringList attrface,QStringList attrbody){
            label->close();
            delete label;
            Portrait *detailsW = new Portrait(widgetManger(),this);
            detailsW->setAttribute(Qt::WA_DeleteOnClose);
            detailsW->setWindowFlags(Qt::Window | Qt::Dialog);
            detailsW->setWindowModality(Qt::ApplicationModal);
            detailsW->setUserStyle(widgetManger()->currentStyle());
            detailsW->slotSetData(face,body,attrface,attrbody);
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
                QPixmap pix = QPixmap::fromImage(images.first());
                imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
                imageBtn_->setProperty("pixmap",pix);
                slotImageSearchBtnClicked();
            });
            dialog.exec();
            dataMenu_->setEnabled(true);
        });
//        serviceI->getScenePic(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        serviceI->getScenePic(dataListW_->currentItem()->data(Qt::UserRole + 5).toString());
        startWorker(worker);
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Search using the image"),[this]{
#if 0
        imageCurOid_ = dataListW_->currentItem()->data(Qt::UserRole + 2).toString();
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        imageBtn_->setIcon(pix.scaled(imageBtn_->iconSize()));
        imageBtn_->setProperty("pixmap",pix);
#else
        imageCurOid_ = dataListW_->currentItem()->data(Qt::UserRole + 2).toString();
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
                           dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
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
        faceLinkP->setFaceLinkOidAndImg(dataListW_->currentItem()->data(Qt::UserRole + 4).toString(),pix);
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

void QueryInCaptureDataBasePage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(s == WidgetManagerI::Danyahei){
        //main
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

bool QueryInCaptureDataBasePage::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        imageEndTimeEdit_->setDateTime(QDateTime::currentDateTime());
        semanticEndTimeEdit_->setDateTime(QDateTime::currentDateTime());
        return true;
    }
    return WidgetI::event(event);
}

void QueryInCaptureDataBasePage::getCameraInfo()
{
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
    startWorker(worker);
}

void QueryInCaptureDataBasePage::updateDataList(int totalRecords, int totalPages, QVector<std::tuple<QImage, QString, QString, QString, QDateTime, QString,QString> > &data)
{
    dataListW_->clear();
    pageIndicator_->adjustRow();
    if(needUpdatePageInfo_){
        pageIndicator_->setPageInfo(totalPages,totalRecords);
        needUpdatePageInfo_ = false;
    }
    foreach (auto &info, data) {
        QString posText = std::get<3>(info);
        QListWidgetItem *item = new QListWidgetItem(posText.left(18) + '\n' + std::get<4>(info).toString("yyyy-MM-dd HH:mm:ss"));
        QImage img = std::get<0>(info);
        item->setIcon(QPixmap::fromImage(img));
        item->setData(Qt::UserRole+1,img);
        item->setData(Qt::UserRole+2,std::get<1>(info));
        item->setData(Qt::UserRole+3,std::get<2>(info));
        item->setData(Qt::UserRole+4,std::get<5>(info));
        item->setData(Qt::UserRole+5,std::get<6>(info));
        item->setTextAlignment(Qt::AlignHCenter);
        dataListW_->addItem(item);
    }
}

void QueryInCaptureDataBasePage::semanticSearch(int index)
{
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
    connect(serviceI,&RestServiceI::sigSemanticSearch,this,[this,label](RestServiceI::SemanticReturnData &returnData){
        label->close();
        imageCurOid_.clear();
        curSearchType_ = 1;
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime,QString,QString> > dataListVec;
        std::transform(returnData.records.begin(),returnData.records.end(),std::back_inserter(dataListVec),[this](RestServiceI::DataRectureItem &nodeV){
            return std::make_tuple(nodeV.img,nodeV.id,nodeV.cameraId,cameraMapInfo_.value(nodeV.cameraId),nodeV.time,nodeV.personId,nodeV.sceneId);
        });
        updateDataList(returnData.toatal,returnData.totalPage,dataListVec);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    RestServiceI::SemanticSearchArgs args;
    args.cameraId = semanticCurCameraId_;
    args.mode = 0;
    args.startT = semanticCurStartTime_;
    args.endT = semanticCurEndTime_;
    args.pageNo = index;
    args.pageSize = 40;
    args.faceAttributList = curfaceAttrList_;
    args.bodyAttributeList = curbodyAttrList_;
    serviceI->semanticSearch(args);
    startWorker(worker);
    label->show(500);
    stackedW_->setEnabled(false);
    pageIndicator_->setEnabled(false);
}

QStringList QueryInCaptureDataBasePage::checkedAttrbute(QTreeWidgetItem *item)
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

void QueryInCaptureDataBasePage::slotConditionListIndexChanged(int index)
{
    switch (index) {
    case 0:
        attributTreeW_->show();
        dataVSplitL_->show();
        break;
    case 1:
        attributTreeW_->hide();
        dataVSplitL_->hide();
        break;
    default:
        break;
    }
    stackedW_->setCurrentIndex(index);
}

void QueryInCaptureDataBasePage::slotImageFaceImgBtnClicked()
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

void QueryInCaptureDataBasePage::slotImageSearchBtnClicked()
{
    QImage faceImg = imageBtn_->property("pixmap").value<QPixmap>().toImage();
    if(faceImg.isNull() && imageCurOid_.isEmpty()){
        QMessageBox::information(this,imageConditionBackW_->objectName(),tr("need image or oid"));
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
#if USEOLDMOLD
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
#else
    RestServiceI::SearchUseImageArgs args;
    args.cameraId = imagePosCombox_->currentData().toString();
    args.image = faceImg;
    args.endT = imageEndTimeEdit_->dateTime();
    args.mode = 1;
    args.recordsCount = imageCountCombox_->currentData().toInt();
    args.smilarty = imageSmilarSpin_->value() / (qreal)100;
    args.startT = imageStartTimeEdit_->dateTime();
    args.faceId = imageCurOid_;
    connect(serviceI,&RestServiceI::sigSearchByImage,this,[this,label](QVector<RestServiceI::DataRectureItem> &returnData){
        label->close();
        imageCurOid_.clear();
        curSearchType_ = 0;
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime,QString,QString> > dataListVec;
        std::transform(returnData.begin(),returnData.end(),std::back_inserter(dataListVec),[this](RestServiceI::DataRectureItem &nodeV){
            return std::make_tuple(nodeV.img,nodeV.id,nodeV.cameraId,cameraMapInfo_.value(nodeV.cameraId),nodeV.time,nodeV.personId,nodeV.sceneId);
        });
        needUpdatePageInfo_ = true;
        updateDataList(returnData.count(),1,dataListVec);
        stackedW_->setEnabled(true);
        pageIndicator_->setEnabled(true);
    });
    serviceI->searchByImage(args);
#endif
    startWorker(worker);
    label->show(500);
    stackedW_->setEnabled(false);
    pageIndicator_->setEnabled(false);
}

void QueryInCaptureDataBasePage::slotSemanticSearchBtnClicked()
{
#if USEOLDMOLD
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
#else
    curfaceAttrList_.clear();
    curbodyAttrList_.clear();
    curfaceAttrList_ = checkedAttrbute(attributTreeW_->topLevelItem(0));
//    curbodyAttrList_ = checkedAttrbute(attributTreeW_->topLevelItem(1));
    semanticCurCameraId_ = semanticPosCombox_->currentData().toString();
    semanticCurStartTime_ = semanticStartTimeEdit_->dateTime();
    semanticCurEndTime_ = semanticEndTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    semanticSearch(1);
#endif
}

void QueryInCaptureDataBasePage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    imagePosCombox_->clear();
    imagePosCombox_->addItem(tr("Unlimited"),"");
    semanticPosCombox_->addItem(tr("Unlimited"),"");
    for (auto &info : data) {
        imagePosCombox_->addItem(info.cameraPos,info.cameraId);
        semanticPosCombox_->addItem(info.cameraPos,info.cameraId);
        cameraMapInfo_[info.cameraId] = info.cameraPos;
    }
}

void QueryInCaptureDataBasePage::slotNextPage(int page)
{
    if(curSearchType_ == 1){
#if USEOLDMOLD
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
#else
        semanticSearch(page);
#endif
    }
}

void QueryInCaptureDataBasePage::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, QueryInCaptureDataBasePage::itemData &items)
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
