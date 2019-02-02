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
#include <QMenu>
#include <QMessageBox>
#include "semanticsearchpage.h"
#include "pageindicator.h"
#include "waitinglabel.h"
#include "service/restservice.h"

SemanticSearchPage::SemanticSearchPage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
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

    QVBoxLayout *mainLay = new QVBoxLayout;
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
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addLayout(vlay);
    hlay->addWidget(centeVSplieL_);
    hlay->addWidget(attributTreeW_);
    setLayout(mainLay);

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

    connect(pageIndicator_,SIGNAL(sigPageClicked(int)),this,SLOT(slotSemanticSearch(int)));
}

void SemanticSearchPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
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
}

void SemanticSearchPage::getCameraInfo()
{

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
        QVector<std::tuple<QImage, QString, QString, QString, QDateTime,QString,QString> > dataListVec;
        std::transform(returnData.records.begin(),returnData.records.end(),std::back_inserter(dataListVec),[this](RestServiceI::DataRectureItem &nodeV){
            return std::make_tuple(nodeV.img,nodeV.id,nodeV.cameraId,cameraMapInfo_.value(nodeV.cameraId),nodeV.time,nodeV.personId,nodeV.sceneId);
        });
//        updateDataList(returnData.toatal,returnData.totalPage,dataListVec);
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

void SemanticSearchPage::slotSemanticSearchBtnClicked()
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
