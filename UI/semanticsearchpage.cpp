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
#include <QEvent>
#include <QDir>
#include <QMenu>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QPainter>
#include <QStandardPaths>
#include <QSettings>
#include <QApplication>
#include "semanticsearchpage.h"
#include "pageindicator.h"
#include "waitinglabel.h"
#include "sceneimagedialog.h"
#include "facelinkpage.h"
#include "trackingpage.h"
#include "portrait.h"
#include "facesearch.h"
#include "informationdialog.h"
#include "nodatatip.h"
#include "portraitsearch.h"
#include "personmark.h"

const int UserPropertyData = Qt::UserRole + 10;

SemanticSearchPage::SemanticSearchPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Surveillance Database"));
    personTypeL_ = new QLabel(tr("Source"));
    personTypeCombox_ = new QComboBox;
    posL_ = new QLabel(tr("Regional location"));
    posCombox_ = new QComboBox;
    startTimeL_ = new QLabel(tr("Start Time"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeL_ = new QLabel(tr("End Time"));
    endTimeEdit_ = new QDateTimeEdit;
    searchBtn_ = new QPushButton(tr("Search"));
    attributTreeW_ = new QTreeWidget;
    dataListW_ = new QListWidget;
    centeVSplieL_ = new QLabel;
    flushAttrBtn_ = new QPushButton(tr("Attributes Reset"));
    pageIndicator_ = new PageIndicator;
    dataMenu_ = new QMenu(dataListW_);

    QVBoxLayout *vlay = new QVBoxLayout;
    QGridLayout *topGridLay = new QGridLayout;
    topGridLay->addWidget(personTypeL_,0,0,1,1);
    topGridLay->addWidget(personTypeCombox_,0,1,1,1);
    topGridLay->addWidget(posL_,0,2,1,1);
    topGridLay->addWidget(posCombox_,0,3,1,1);
    topGridLay->addWidget(startTimeL_,1,0,1,1);
    topGridLay->addWidget(startTimeEdit_,1,1,1,1);
    topGridLay->addWidget(endTimeL_,1,2,1,1);
    topGridLay->addWidget(endTimeEdit_,1,3,1,1);
    topGridLay->addWidget(searchBtn_,1,4,1,1);
    topGridLay->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    topGridLay->setVerticalSpacing(20);
    topGridLay->setHorizontalSpacing(10);
    vlay->addLayout(topGridLay);
    vlay->addSpacing(40);
    vlay->addWidget(dataListW_);
    vlay->addWidget(pageIndicator_);
    vlay->setSpacing(0);
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addLayout(vlay);
    mainLay->addWidget(centeVSplieL_);
    vlay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(flushAttrBtn_);
    vlay->addLayout(hlay);
    vlay->addWidget(attributTreeW_);
    vlay->setSpacing(0);
    mainLay->addLayout(vlay);
    mainLay->setSpacing(20);
    mainLay->setContentsMargins(40,40,40,40);
    setLayout(mainLay);

    attributTreeW_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    attributTreeW_->setIconSize(QSize(14, 17));
    dataMenu_->addAction(tr("Profile"),[this]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(dataListW_);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigPeronsDetails,this,[this,label](RestServiceI::PortraitReturnData resData){
            label->close();
            delete label;
            Portrait *detailsW = new Portrait(this);
            detailsW->setAttribute(Qt::WA_DeleteOnClose);
            detailsW->setWindowFlags(Qt::Dialog);
            detailsW->setWindowModality(Qt::ApplicationModal);
            detailsW->setUserStyle(userStyle());
            detailsW->slotSetData(resData);
            detailsW->setMinimumSize(582,514);
            detailsW->show();
            dataMenu_->setEnabled(true);
        });
        serviceI->getPersonDetails(dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Scene Analysis"),[this]{
        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        WaitingLabel *label = new WaitingLabel(this);
        connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(dataListW_);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            dataMenu_->setEnabled(true);
        });
        connect(serviceI,&RestServiceI::sigSceneInfo,this,[this,label](const RestServiceI::SceneInfo sinfo){
            label->close();
            delete label;
            SceneImageDialog dialog(dataListW_);
            dialog.setUserStyle(userStyle());
            dialog.setSceneInfo(sinfo);
            connect(&dialog,&SceneImageDialog::sigImages,&dialog,[this](QVector<QImage> images){
                if(!images.count()){
                    return;
                }
                FaceSearch *faceDialog = new FaceSearch(this);
                faceDialog->setAttribute(Qt::WA_DeleteOnClose);
                faceDialog->setWindowFlags(Qt::Dialog);
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
        serviceI->getSceneInfo(dataListW_->currentItem()->data(Qt::UserRole + 5).toString());
        label->show(500);
        dataMenu_->setEnabled(false);
    });
    dataMenu_->addAction(tr("Capture Search"),[this]{
        FaceSearch *faceDialog = new FaceSearch(this);
        faceDialog->setAttribute(Qt::WA_DeleteOnClose);
        faceDialog->setWindowFlags(Qt::Dialog);
        faceDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = faceDialog->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        faceDialog->setPalette(pal);
        faceDialog->setAutoFillBackground(true);
        faceDialog->setUserStyle(userStyle());
        faceDialog->layout()->setMargin(10);
        faceDialog->setFaceImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceDialog->setOid(dataListW_->currentItem()->data(Qt::UserRole + 2).toString());
        faceDialog->setMinimumHeight(700);
        faceDialog->show();
    });
    dataMenu_->addAction(tr("Registration Search"),[this]{
        PortraitSearch *portSearchDialog = new PortraitSearch(this);
        portSearchDialog->setAttribute(Qt::WA_DeleteOnClose);
        portSearchDialog->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
        portSearchDialog->setWindowModality(Qt::ApplicationModal);
        QPalette pal = portSearchDialog->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        portSearchDialog->setPalette(pal);
        portSearchDialog->setAutoFillBackground(true);
        portSearchDialog->setUserStyle(userStyle());
        portSearchDialog->setMinimumSize(1250,710);
        portSearchDialog->slotAddImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
//        portSearchDialog->layout()->setMargin(10);
//        portSearchDialog->setMinimumHeight(700);
        portSearchDialog->show();
    });
    dataMenu_->addAction(tr("Facelink"),[this]{
        FaceLinkPage *faceLinkP = new FaceLinkPage(this);
        QPalette pal = faceLinkP->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        faceLinkP->setPalette(pal);
        faceLinkP->setAutoFillBackground(true);
        faceLinkP->setAttribute(Qt::WA_DeleteOnClose);
        faceLinkP->setWindowFlags(Qt::Dialog);
        faceLinkP->setWindowModality(Qt::ApplicationModal);
        QPixmap pix = QPixmap::fromImage(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        faceLinkP->setFaceLinkOidAndImg(dataListW_->currentItem()->data(Qt::UserRole + 4).toString(),pix);
        faceLinkP->resize(1200,900);
        faceLinkP->show();
    });
    dataMenu_->addAction(tr("Trajectory"),[this]{
        TrackingPage *view = new TrackingPage(this);
        QPalette pal = view->palette();
        pal.setColor(QPalette::Background,QColor(37,41,52));
        view->setPalette(pal);
        view->setAutoFillBackground(true);
        view->setUserStyle(userStyle());
        view->setAttribute(Qt::WA_DeleteOnClose);
        view->setWindowFlags(Qt::Dialog);
        view->setWindowModality(Qt::ApplicationModal);
        view->setMinimumSize(1655,924);
        view->setImgageOid(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>(),
                           dataListW_->currentItem()->data(Qt::UserRole + 4).toString());
        view->show();
    });
    dataMenu_->addAction(tr("Register"),[this]{
        PersonMark markDialog;
        markDialog.setPhoto(dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>());
        QDialog::DialogCode returnCode = QDialog::DialogCode(markDialog.exec());
        if(returnCode == QDialog::Accepted){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(0);
            infoDialog.setMessage(tr("Successed"));
            infoDialog.exec();
        }
    });
    dataMenu_->addAction(tr("Save Image"),[this]{
        QString personId = dataListW_->currentItem()->data(Qt::UserRole + 4).toString();
        QString filePath =  QFileDialog::getSaveFileName(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/" + personId + ".jpg",tr("Images (*.png *.jpg)"));
        if(filePath.isEmpty()){
            return;
        }
        if(!dataListW_->currentItem()->data(Qt::UserRole + 1).value<QImage>().save(filePath)){
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage("Operation failed!");
            infoDialog.exec();
        }
    });
    dataListW_->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(dataListW_,&QListWidget::customContextMenuRequested,this,[&](QPoint p){
        if(!dataListW_->itemAt(p))return;
        dataMenu_->move(QCursor::pos());
        dataMenu_->show();
    });
    dataListW_->setFrameStyle(QFrame::NoFrame);
    dataListW_->setFocusPolicy(Qt::NoFocus);
    dataListW_->setMovement(QListWidget::Static);
    dataListW_->setResizeMode(QListWidget::Adjust);
    dataListW_->setViewMode(QListWidget::IconMode);
    centeVSplieL_->setFixedWidth(1);
    QVector<itemData> devicesVec;
    itemData items;
    items.name = tr("Face Attributes");
    items.strValue = "Face Attributes";
    items.value = 0;
    items.childrens << itemData{tr("Obstruction"),tr("5_o_Clock_Shadow"),0,QVector<itemData>()}
                    << itemData{tr("Arched Eyebrows"),tr("Arched_Eyebrows"),0,QVector<itemData>()}
                    << itemData{tr("Attractive"),tr("Attractive"),0,QVector<itemData>()}
                    << itemData{tr("Bags Under Eyes"),tr("Bags_Under_Eyes"),0,QVector<itemData>()}
                    << itemData{tr("Bald"),tr("Bald"),0,QVector<itemData>()}
                    << itemData{tr("Bangs"),tr("Bangs"),0,QVector<itemData>()}
                    << itemData{tr("Big Lips"),tr("Big_Lips"),0,QVector<itemData>()}
                    << itemData{tr("Big Nose"),tr("Big_Nose"),0,QVector<itemData>()}
                    << itemData{tr("Black Hair"),tr("Black_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Blond Hair"),tr("Blond_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Blurry"),tr("Blurry"),0,QVector<itemData>()}
                    << itemData{tr("Brown Hair"),tr("Brown_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Bushy Eyebrows"),tr("Bushy_Eyebrows"),0,QVector<itemData>()}
                    << itemData{tr("Chubby"),tr("Chubby"),0,QVector<itemData>()}
                    << itemData{tr("Double Chin"),tr("Double_Chin"),0,QVector<itemData>()}
                    << itemData{tr("Eyeglasses"),tr("Eyeglasses"),0,QVector<itemData>()}
                    << itemData{tr("Goatee"),tr("Goatee"),0,QVector<itemData>()}
                    << itemData{tr("Gray Hair"),tr("Gray_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Heavy Makeup"),tr("Heavy_Makeup"),0,QVector<itemData>()}
                    << itemData{tr("High Cheekbones"),tr("High_Cheekbones"),0,QVector<itemData>()}
                    << itemData{tr("Male"),tr("Male"),0,QVector<itemData>()}
                    << itemData{tr("Female"),tr("Female"),0,QVector<itemData>()}
                    << itemData{tr("Mouth Slightly Open"),tr("Mouth_Slightly_Open"),0,QVector<itemData>()}
                    << itemData{tr("Mustache"),tr("Mustache"),0,QVector<itemData>()}
                    << itemData{tr("Narrow Eyes"),tr("Narrow_Eyes"),0,QVector<itemData>()}
                    << itemData{tr("No Beard"),tr("No_Beard"),0,QVector<itemData>()}
                    << itemData{tr("Oval Face"),tr("Oval_Face"),0,QVector<itemData>()}
                    << itemData{tr("Pale Skin"),tr("Pale_Skin"),0,QVector<itemData>()}
                    << itemData{tr("Pointy Nose"),tr("Pointy_Nose"),0,QVector<itemData>()}
                    << itemData{tr("Receding Hairline"),tr("Receding_Hairline"),0,QVector<itemData>()}
                    << itemData{tr("Rosy Cheeks"),tr("Rosy_Cheeks"),0,QVector<itemData>()}
                    << itemData{tr("Sideburns"),tr("Sideburns"),0,QVector<itemData>()}
                    << itemData{tr("Smiling"),tr("Smiling"),0,QVector<itemData>()}
                    << itemData{tr("Straight Hair"),tr("Straight_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Wavy Hair"),tr("Wavy_Hair"),0,QVector<itemData>()}
                    << itemData{tr("Wearing Earrings"),tr("Wearing_Earrings"),0,QVector<itemData>()}
                    << itemData{tr("Wearing Hat"),tr("Wearing_Hat"),0,QVector<itemData>()}
                    << itemData{tr("Wearing Lipstick"),tr("Wearing_Lipstick"),0,QVector<itemData>()}
                    << itemData{tr("Young"),tr("Young"),0,QVector<itemData>()};
    devicesVec << items;
    for(auto value : devicesVec){
        createTreeItem(attributTreeW_,nullptr,value);
    }
    attributTreeW_->expandItem(attributTreeW_->topLevelItem(0));
    attributTreeW_->setHeaderLabel(tr("Attributes"));
    attributTreeW_->headerItem()->setTextAlignment(0,Qt::AlignCenter);
    attributTreeW_->header()->setStretchLastSection(true);
    attributTreeW_->header()->setIconSize(QSize(50,50));
    attributTreeW_->header()->hide();
    attributTreeW_->setFocusPolicy(Qt::NoFocus);
    flushAttrBtn_->setFixedSize(164,34);
    QSize s = attributTreeW_->headerItem()->sizeHint(0);
    attributTreeW_->headerItem()->setSizeHint(0,QSize(s.width(),50));
    personTypeCombox_->setFixedSize(200,34);
    posCombox_->setFixedSize(200,34);
    startTimeEdit_->setFixedSize(200,34);
    startTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    connect(startTimeEdit_, &QDateTimeEdit::editingFinished, this, [this](){
        if (1 != personTypeCombox_->currentIndex())
        {
            return;
        }
        if (startTimeEdit_->dateTime().addDays(3).msecsTo(endTimeEdit_->dateTime()) > 0)
        {
            endTimeEdit_->setDateTime(startTimeEdit_->dateTime().addDays(3));
        }
    });
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setFixedSize(200,34);
    endTimeEdit_->setDisplayFormat("yyyy-MM-dd HH:mm:ss");
    connect(endTimeEdit_, &QDateTimeEdit::editingFinished, this, [this](){
        if (1 != personTypeCombox_->currentIndex())
        {
            return;
        }
        if (startTimeEdit_->dateTime().addDays(3).msecsTo(endTimeEdit_->dateTime()) > 0)
        {
            endTimeEdit_->setDateTime(startTimeEdit_->dateTime().addDays(3));
        }
    });
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    searchBtn_->setMinimumSize(99,33);
    pageIndicator_->setPageInfo(0,0);
    personTypeVec_ << qMakePair(QString("All"),QVector<int>() << 1 << 2 << 3 << 6 << 7);
    personTypeVec_ << qMakePair(QString("Semantic"),QVector<int>() << 0 << 1 << 2 << 3 << 5 << 6 << 7);
    personTypeVec_ << qMakePair(QString("Facelink"),QVector<int>() << 0 << 1 << 2 << 3 << 4 << 5 << 6 << 7);
    for(auto &v : personTypeVec_){
        personTypeCombox_->addItem(v.first);
    }
    // 3.17 edit limit time range
    connect(personTypeCombox_, SIGNAL(currentIndexChanged(int)), this, SLOT(onSourceCurrentIndexChanged(int)));

    noDataW_ = new NoDataTip(dataListW_);

    connect(pageIndicator_,SIGNAL(sigPageClicked(int)),this,SLOT(slotPageIndexChanged(int)));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(attributTreeW_,SIGNAL(itemChanged(QTreeWidgetItem*,int)),this,SLOT(slotTreeItemChanged(QTreeWidgetItem*,int)));
    connect(flushAttrBtn_,SIGNAL(clicked(bool)),this,SLOT(slotAttributeBtnClicked()));

    QSettings config("config.ini",QSettings::IniFormat);
    dataRows_ = config.value("App/SemanticRows").toInt();
    dataCols_ = config.value("App/SemanticCols").toInt();
    setUserStyle(userStyle());
    getCameraInfo();
}

void SemanticSearchPage::setUserStyle(int s)
{
    QFont f;
    if(0 == s){
        personTypeCombox_->setStyleSheet("QComboBoxListView{"
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

        personTypeL_->setStyleSheet("QLabel{"
                                    "font-size: 14px;"
                                    "color: rgba(255,255,255,0.75);"
                                    "}");
        posL_->setStyleSheet("QLabel{"
                             "font-size: 14px;"
                             "color: rgba(255,255,255,0.75);"
                             "}");
        startTimeL_->setStyleSheet("QLabel{"
                                   "font-size: 14px;"
                                   "color: rgba(255,255,255,0.75);"
                                   "}");
        endTimeL_->setStyleSheet("QLabel{"
                                 "font-size: 14px;"
                                 "color: rgba(255,255,255,0.75);"
                                 "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "color: rgba(255,255,255,0.75);"
                                      "border-radius:4px;"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "padding-left: 10px;"
                                      "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "color: rgba(255,255,255,0.75);"
                                    "border-radius:4px;"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "padding-left: 10px;"
                                    "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 4px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        dataListW_->setStyleSheet("QListWidget{"
                                  "background: transparent;"
                                  "font: 11px;"
                                  "color: rgba(255,255,255,0.75);"
                                  "}");
        dataListW_->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{"
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
        attributTreeW_->setStyleSheet("QTreeView{"
                                      "border:none;"
                                      "font-size: 14px;"
                                      "color: rgba(255,255,255,0.75);"
                                      "border-radius: 10px;"
                                      "background-color: transparent;}"
                                      "QTreeView::item:disabled{"
                                      "color:gray;"
                                      "}"
                                      "QTreeView::item{"
                                      "color: rgba(255,255,255,0.75);"
                                      "border:none;background-color: transparent;}"
                                      "}"
                                      "QTreeView::item:selected{"
                                      "color: rgba(255,255,255,0.75);"
                                      "border:none;"
                                      "}"
                                      "QTreeWidget::indicator {width:30px;height:18px;background-color:transparent;}"
                                      "QTreeWidget::indicator:checked {image: url(images/open.png);}"
                                      "QTreeWidget::indicator:unchecked {image: url(images/close.png);}"
                                              //"QTreeWidget::indicator:indeterminate {image: url(:/Images/bxz.png);}"
                                      "QTreeWidget::indicator:disabled {"
                                      "background:transparent;"
                                      "}"
                                      "QTreeView::branch:has-children:!has-siblings:closed,"
                                      "QTreeView::branch:closed:has-children:has-siblings{"
                                      "border-image: none;"
                                      "image: url(images/tree_unexpand.png);"
                                      "}"
                                      "QTreeView::branch:open:has-children:!has-siblings,"
                                      "QTreeView::branch:open:has-children:has-siblings  {"
                                      "border-image: none;"
                                      "image: url(images/tree_expand.png);"
                                      "}");
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
                                                "color: rgba(255,255,255,0.75);"
                                                "border: none;"
                                                "font-size: 14px;"
                                                "font-weight: bold;"
                                                "background-color: transparent;"
                                                "}");
        flushAttrBtn_->setStyleSheet("QPushButton{"
                                     "background-color: rgb(83,77,251);"
                                     "color: white;"
                                     "border-radius: 4px;"
                                     "font-size: 14px;"
                                     "}"
                                     "QPushButton:pressed{"
                                     "padding: 2px;"
                                     "background-color: #312DA6;"
                                     "}");

        noDataW_->setUserStyle(s);
    }
    pageIndicator_->setUserStyle();
}

void SemanticSearchPage::resizeEvent(QResizeEvent *event)
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

bool SemanticSearchPage::event(QEvent *event)
{
    if(event->type() == QEvent::Show  && searchBtn_->isEnabled()){
        endTimeEdit_->setDateTime(QDateTime::currentDateTime());
//        slotSearchBtnClicked();
    }
    return WidgetI::event(event);
}

void SemanticSearchPage::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void SemanticSearchPage::setEnableAttrs(const QStringList & attrs)
{
    for(int i = 0; i < attributTreeW_->topLevelItem(0)->childCount(); i++){
        attributTreeW_->topLevelItem(0)->child(i)->setDisabled(true);
    }
    for(const QString attr : attrs){
        for(int i = 0; i < attributTreeW_->topLevelItem(0)->childCount(); i++){
            if(attributTreeW_->topLevelItem(0)->child(i)->text(0) == attr){
                attributTreeW_->topLevelItem(0)->child(i)->setDisabled(false);
                break;
            }
        }
    }
}

void SemanticSearchPage::setTableData(QVector<RestServiceI::DataRectureItem> &data)
{
    dataListW_->clear();
    for(int i = 0; i < personTypeVec_.count(); i++){
        if(personTypeCombox_->itemText(curMode_) == personTypeVec_.at(i).first){
            for(int j = 0; j < dataMenu_->actions().count(); j++){
                dataMenu_->actions().at(j)->setEnabled(personTypeVec_.at(i).second.contains(j));
            }
            break;
        }
    }
    QFontMetrics fs(dataListW_->font());
    int textwidth = itemSize_.width() / (fs.height() >> 1);
    for (RestServiceI::DataRectureItem &info : data) {
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

    if(data.isEmpty()){
        noDataW_->show();
    }
}

void SemanticSearchPage::slotSemanticSearch(int page,bool attrAsync)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(dataListW_);
    if(attrAsync){
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            dataGeted_ = true;
            curEnableAttrErrorStr_ = str;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                InformationDialog infoDialog(this);
                infoDialog.setUserStyle(userStyle());
                if(curEnableAttrErrorStr_.isEmpty()){
                    infoDialog.setMessage(tr("Data:%2").arg(str));
                }else{
                    infoDialog.setMessage(tr("Data:%1\nGetEnabledAttr:%2").arg(str,curEnableAttrErrorStr_));
                }
                infoDialog.exec();
                noDataW_->show();
                curEnableAttrErrorStr_.clear();
                curDataErrorStr_.clear();
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        connect(serviceI,&RestServiceI::sigSemanticSearch,this,[this,label](RestServiceI::SemanticReturnData returnData){
            dataGeted_ = true;
            curTotalPage_ = returnData.totalPage;
            curTotalRecords_ = returnData.toatal;
            curDataVec_ = returnData.records;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                if(!curEnableAttrErrorStr_.isEmpty()){
                    InformationDialog infoDialog(this);
                    infoDialog.setUserStyle(userStyle());
                    infoDialog.setMessage(tr("GetEnabledAttr:%1").arg(curDataErrorStr_));
                    infoDialog.exec();
                    curEnableAttrErrorStr_.clear();
                    noDataW_->show();
                    pageIndicator_->setEnabled(true);
                    searchBtn_->setEnabled(true);
                    attributTreeW_->setEnabled(true);
                    flushAttrBtn_->setEnabled(true);
                    preIsSearch_ = false;
                    isClearnAttrSelected_ = false;
                    return;
                }
                setEnableAttrs(curEnableAttrList_);
                pageIndicator_->adjustRow();
                pageIndicator_->setPageInfo(curTotalPage_,curTotalRecords_);
                setTableData(curDataVec_);
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        getAvailableAttrs(label);
    }else{
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            noDataW_->show();
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
        connect(serviceI,&RestServiceI::sigSemanticSearch,this,[this,label](RestServiceI::SemanticReturnData returnData){
            label->close();
            delete label;
            pageIndicator_->adjustRow();
            if(needUpdatePageInfo_){
                pageIndicator_->setPageInfo(returnData.totalPage,returnData.toatal);
                needUpdatePageInfo_ = false;
            }
            setTableData(returnData.records);
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
    }
    RestServiceI::SemanticSearchArgs args;
    args.cameraId = curCameraId_;
    args.mode = 0;
    args.startT = curStartTime_;
    args.endT = curEndTime_;
    args.pageNo = page;
    args.pageSize = dataRows_ * dataCols_;
    args.faceAttributList = curfaceAttrList_;
    serviceI->semanticSearch(args);
    label->show(500);
    pageIndicator_->setEnabled(false);
    searchBtn_->setEnabled(false);
    attributTreeW_->setEnabled(false);
    noDataW_->hide();
    dataListW_->clear();
    preIsSearch_ = true;
    dataGeted_ = false;
}

void SemanticSearchPage::slotSearchFaceLink(int page, bool attrAsync)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(dataListW_);
    if(attrAsync){
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            dataGeted_ = true;
            curEnableAttrErrorStr_ = str;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                InformationDialog infoDialog(this);
                infoDialog.setUserStyle(userStyle());
                if(curEnableAttrErrorStr_.isEmpty()){
                    infoDialog.setMessage(tr("Data:%2").arg(str));
                }else{
                    infoDialog.setMessage(tr("Data:%1\nGetEnabledAttr:%2").arg(str,curEnableAttrErrorStr_));
                }
                infoDialog.exec();
                noDataW_->show();
                curEnableAttrErrorStr_.clear();
                curDataErrorStr_.clear();
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        connect(serviceI,&RestServiceI::sigFaceLinkDataColl,this,[this,label](RestServiceI::FaceLinkDataCollReturn &returnData){
            dataGeted_ = true;
            curTotalPage_ = returnData.totalPage;
            curTotalRecords_ = returnData.toatal;
            curDataVec_ = returnData.records;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                if(!curEnableAttrErrorStr_.isEmpty()){
                    InformationDialog infoDialog(this);
                    infoDialog.setUserStyle(userStyle());
                    infoDialog.setMessage(tr("GetEnabledAttr:%1").arg(curDataErrorStr_));
                    infoDialog.exec();
                    curEnableAttrErrorStr_.clear();
                    noDataW_->show();
                    pageIndicator_->setEnabled(true);
                    searchBtn_->setEnabled(true);
                    attributTreeW_->setEnabled(true);
                    flushAttrBtn_->setEnabled(true);
                    preIsSearch_ = false;
                    isClearnAttrSelected_ = false;
                    return;
                }
                setEnableAttrs(curEnableAttrList_);
                pageIndicator_->adjustRow();
                pageIndicator_->setPageInfo(curTotalPage_,curTotalRecords_);
                setTableData(curDataVec_);
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        getAvailableAttrs(label);
    }else{
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            noDataW_->show();
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
        connect(serviceI,&RestServiceI::sigFaceLinkDataColl,this,[this,label](RestServiceI::FaceLinkDataCollReturn &returnData){
            label->close();
            delete label;
            pageIndicator_->adjustRow();
            if(needUpdatePageInfo_){
                pageIndicator_->setPageInfo(returnData.totalPage,returnData.toatal);
                needUpdatePageInfo_ = false;
            }
            setTableData(returnData.records);
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
    }
    RestServiceI::FaceLinkDataCollArgs args;
    args.cameraId = curCameraId_;
    args.faceAttrs = curfaceAttrList_;
    args.startT = curStartTime_;
    args.endT = curEndTime_;
    args.pageNo = page;
    args.pageSize = dataRows_ * dataCols_;
    serviceI->getFaceLinkDataColl(args);
    label->show(500);
    pageIndicator_->setEnabled(false);
    searchBtn_->setEnabled(false);
    attributTreeW_->setEnabled(false);
    noDataW_->hide();
    dataListW_->clear();
    preIsSearch_ = true;
    dataGeted_ = false;
}

QStringList SemanticSearchPage::checkedAttrbute(QTreeWidgetItem *item)
{
    QStringList attrbuteList;
    QTreeWidgetItemIterator it(item);
    while (*it) {
        if ((*it)->type() && (*it)->checkState(0) == Qt::Checked){
            //attrbuteList << (*it)->text(0);
            attrbuteList << (*it)->data(0, UserPropertyData).toString();
        }
        ++it;
    }
    return attrbuteList;
}

void SemanticSearchPage::getAvailableAttrs(WaitingLabel *label)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,&RestServiceI::sigAvailableAttrs,this,[this,label](QStringList attrs){
        availabelAttrGeted_ = true;
        curEnableAttrList_ = attrs;
        if(dataGeted_){
            label->close();
            delete label;
            if(!curDataErrorStr_.isEmpty()){
                InformationDialog infoDialog(this);
                infoDialog.setUserStyle(userStyle());
                infoDialog.setMessage(tr("Data:%1").arg(curDataErrorStr_));
                infoDialog.exec();
                curDataErrorStr_.clear();
                noDataW_->show();
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
                flushAttrBtn_->setEnabled(true);
                return;
            }
            setEnableAttrs(attrs);
            pageIndicator_->adjustRow();
            pageIndicator_->setPageInfo(curTotalPage_,curTotalRecords_);
            setTableData(curDataVec_);
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            flushAttrBtn_->setEnabled(true);
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        }
    });
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        availabelAttrGeted_ = true;
        if(dataGeted_){
            label->close();
            delete label;
            curEnableAttrErrorStr_ = str;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            if(curDataErrorStr_.isEmpty()){
                infoDialog.setMessage(tr("GetEnabledAttr:%2").arg(str));
            }else{
                infoDialog.setMessage(tr("Data:%1\nGetEnabledAttr:%2").arg(curDataErrorStr_,str));
            }
            infoDialog.exec();
            noDataW_->show();
            curEnableAttrErrorStr_.clear();
            curDataErrorStr_.clear();
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            flushAttrBtn_->setEnabled(true);
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        }
    });
    RestServiceI::SearchAttrsArgs args;
    args.cameraId = curCameraId_;
    args.model = QString::number(curMode_ + 1);
    args.faceAttrs = curfaceAttrList_;
    args.startT = curStartTime_;
    args.endT = curEndTime_;
    serviceI->getAvailabelAttrs(args);
    availabelAttrGeted_ = false;
}

void SemanticSearchPage::slotAttributeBtnClicked()
{
    isClearnAttrSelected_ = true;
    QTreeWidgetItemIterator it(attributTreeW_->topLevelItem(0));
    while (*it) {
        if((*it)->type()){
            (*it)->setCheckState(0,Qt::Unchecked);
        }
        ++it;
    }

    curfaceAttrList_.clear();
    curCameraId_ = posCombox_->currentData().toString();
    curMode_ = personTypeCombox_->currentIndex();
    curStartTime_ = startTimeEdit_->dateTime();
    curEndTime_ = endTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    if(curMode_ == 0){
        slotSearchAll(1,true);
    }else if(curMode_ == 1){
        slotSemanticSearch(1,true);
    }else if(curMode_ == 2){
        slotSearchFaceLink(1,true);
    }
    flushAttrBtn_->setEnabled(false);
}

void SemanticSearchPage::slotPageIndexChanged(int page)
{
    if(curMode_ == 0){
        slotSearchAll(page,false);
    }else if(curMode_ == 1){
        slotSemanticSearch(page,false);
    }else if(curMode_ == 2){
        slotSearchFaceLink(page,false);
    }
}

void SemanticSearchPage::slotSearchAll(int page, bool attrAsync)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(dataListW_);
    if(attrAsync){
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            dataGeted_ = true;
            curEnableAttrErrorStr_ = str;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                InformationDialog infoDialog(this);
                infoDialog.setUserStyle(userStyle());
                if(curEnableAttrErrorStr_.isEmpty()){
                    infoDialog.setMessage(tr("Data:%2").arg(str));
                }else{
                    infoDialog.setMessage(tr("Data:%1\nGetEnabledAttr:%2").arg(str,curEnableAttrErrorStr_));
                }
                infoDialog.exec();
                noDataW_->show();
                curEnableAttrErrorStr_.clear();
                curDataErrorStr_.clear();
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        connect(serviceI,&RestServiceI::sigCaptureSearch,this,[this,label](RestServiceI::CaptureSearchReturnData value){
            dataGeted_ = true;
            curTotalPage_ = value.totalPage;
            curTotalRecords_ = value.totalCount;
            curDataVec_ = value.data;
            if(availabelAttrGeted_){
                label->close();
                delete label;
                if(!curEnableAttrErrorStr_.isEmpty()){
                    InformationDialog infoDialog(this);
                    infoDialog.setUserStyle(userStyle());
                    infoDialog.setMessage(tr("GetEnabledAttr:%1").arg(curDataErrorStr_));
                    infoDialog.exec();
                    curEnableAttrErrorStr_.clear();
                    pageIndicator_->setEnabled(true);
                    searchBtn_->setEnabled(true);
                    attributTreeW_->setEnabled(true);
                    flushAttrBtn_->setEnabled(true);
                    preIsSearch_ = false;
                    isClearnAttrSelected_ = false;
                    noDataW_->show();
                    return;
                }

                setEnableAttrs(curEnableAttrList_);
                pageIndicator_->adjustRow();
                pageIndicator_->setPageInfo(curTotalPage_,curTotalRecords_);
                setTableData(curDataVec_);
                pageIndicator_->setEnabled(true);
                searchBtn_->setEnabled(true);
                attributTreeW_->setEnabled(true);
                flushAttrBtn_->setEnabled(true);
                preIsSearch_ = false;
                isClearnAttrSelected_ = false;
            }
        });
        getAvailableAttrs(label);
    }else{
        connect(serviceI,&RestServiceI::sigError,this,[this,label](const QString str){
            label->close();
            delete label;
            InformationDialog infoDialog(this);
            infoDialog.setUserStyle(userStyle());
            infoDialog.setMessage(str);
            infoDialog.exec();
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            noDataW_->show();
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
        connect(serviceI,&RestServiceI::sigCaptureSearch,this,[this,label](RestServiceI::CaptureSearchReturnData value){
            label->close();
            delete label;
            pageIndicator_->adjustRow();
            if(needUpdatePageInfo_){
                pageIndicator_->setPageInfo(value.totalPage,value.totalCount);
                needUpdatePageInfo_ = false;
            }
            setTableData(value.data);
            pageIndicator_->setEnabled(true);
            searchBtn_->setEnabled(true);
            attributTreeW_->setEnabled(true);
            preIsSearch_ = false;
            isClearnAttrSelected_ = false;
        });
    }
    RestServiceI::CaptureSearchArgs args;
    args.page = page;
    args.pageCount = dataRows_ * dataCols_;
    args.position = curCameraId_;
    args.faceAttributList = curfaceAttrList_;
    args.start = curStartTime_;
    args.end = curEndTime_;
    serviceI->captureSearch(args);
    label->show(500);
    pageIndicator_->setEnabled(false);
    searchBtn_->setEnabled(false);
    attributTreeW_->setEnabled(false);
    noDataW_->hide();
    dataListW_->clear();
    preIsSearch_ = true;
    dataGeted_ = false;
}

void SemanticSearchPage::slotSearchBtnClicked()
{
    curCameraId_ = posCombox_->currentData().toString();
    curMode_ = personTypeCombox_->currentIndex();
    curStartTime_ = startTimeEdit_->dateTime();
    curEndTime_ = endTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    slotPageIndexChanged(1);
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

void SemanticSearchPage::slotTreeItemChanged(QTreeWidgetItem *item, int column)
{
    if(preIsSearch_ || isClearnAttrSelected_)return;
    curfaceAttrList_ = checkedAttrbute(attributTreeW_->topLevelItem(0));
    curCameraId_ = posCombox_->currentData().toString();
    curMode_ = personTypeCombox_->currentIndex();
    curStartTime_ = startTimeEdit_->dateTime();
    curEndTime_ = endTimeEdit_->dateTime();
    needUpdatePageInfo_ = true;
    if(curMode_ == 0){
        slotSearchAll(1,true);
    }else if(curMode_ == 1){
        slotSemanticSearch(1,true);
    }else if(curMode_ == 2){
        slotSearchFaceLink(1,true);
    }
}

void SemanticSearchPage::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, SemanticSearchPage::itemData &items)
{
    static int imgIndex = 0;
    QString strIconPath = QString("images/attributes/%1%2").arg(imgIndex).arg(".png");
    //qDebug() << Q_FUNC_INFO << strIconPath;
    QTreeWidgetItem *item{nullptr};
    if(parentItem){
        item = new QTreeWidgetItem(parentItem, QStringList() << items.name, items.childrens.isEmpty());
    }else{
        item = new QTreeWidgetItem(treeW, QStringList() << items.name, items.childrens.isEmpty());
    }
    item->setData(0, UserPropertyData, items.strValue);
    if (0 != imgIndex)
    {
        item->setIcon(0, QIcon(strIconPath));
    }
    imgIndex++;
    if(item->type()){
        item->setCheckState(0,Qt::Unchecked);
        item->setDisabled(true);
    }
    for(auto value : items.childrens){
        createTreeItem(treeW,item,value);
    }
}

void SemanticSearchPage::onSourceCurrentIndexChanged(int index)
{
    if (1 == index)
    {
        if (startTimeEdit_->dateTime().addDays(3).msecsTo(endTimeEdit_->dateTime()) > 0)
        {
            endTimeEdit_->setDateTime(startTimeEdit_->dateTime().addDays(3));
        }
    }
}
