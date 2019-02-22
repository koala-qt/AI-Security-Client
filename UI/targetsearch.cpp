#include <QTreeWidget>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QPainter>
#include <QScrollBar>
#include <QHeaderView>
#include <QEvent>
#include "targetsearch.h"
#include "semanticsearchpage.h"
#include "facesearch.h"
#include "combinationpage.h"
#include "multiplesearch.h"
#include "facelinkpage.h"
#include "trackingpage.h"
#include "videoanalysis.h"
#include "portraitsearch.h"

TargetSearch::TargetSearch( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Target search"));
    backImg_.load("images/Mask.png");
    treeW_ = new QTreeWidget;
    stackedW_ = new QStackedWidget;
    semanticSearchPage_ = new SemanticSearchPage;
    faceSearchPage_ = new FaceSearch();
    combinationPage_ = new CombinationPage();
    multiPleSearchPage_ = new MultipleSearch();
    facelinkPage_ = new FaceLinkPage();
    trackingPage_ = new TrackingPage();
    videoAnalysisPage_ = new VideoAnalysis();
    portraitSearchPage_ = new PortraitSearch;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(treeW_,33);
    mainLay->addWidget(stackedW_,155);
    mainLay->setContentsMargins(20,30,20,25);
    mainLay->setSpacing(20);
    setLayout(mainLay);

    stackedW_->addWidget(semanticSearchPage_);
    stackedW_->addWidget(faceSearchPage_);
    stackedW_->addWidget(combinationPage_);
    stackedW_->addWidget(multiPleSearchPage_);
    stackedW_->addWidget(facelinkPage_);
    stackedW_->addWidget(trackingPage_);
    stackedW_->addWidget(portraitSearchPage_);
    stackedW_->addWidget(videoAnalysisPage_);
    semanticSearchPage_->installEventFilter(this);
    faceSearchPage_->installEventFilter(this);
    multiPleSearchPage_->installEventFilter(this);
    facelinkPage_->installEventFilter(this);
    trackingPage_->installEventFilter(this);
    portraitSearchPage_->installEventFilter(this);
    videoAnalysisPage_->installEventFilter(this);
    QVector<itemData> devicesVec;
    itemData items;
    items.name = tr("Capture search");
    items.value = 0;
    items.childrens << itemData{semanticSearchPage_->objectName(),0,QVector<itemData>()} << itemData{"Upload",1,QVector<itemData>()
                                                                                  << itemData{faceSearchPage_->objectName(),1,QVector<itemData>()}
                                                                                  << itemData{combinationPage_->objectName(),2,QVector<itemData>()}
                                                                                  << itemData{multiPleSearchPage_->objectName(),3,QVector<itemData>()}
                                                                                  << itemData{facelinkPage_->objectName(),4,QVector<itemData>()}
                                                                                  << itemData{trackingPage_->objectName(),5,QVector<itemData>()}};
    devicesVec << items;
    items.childrens.clear();
    items.name = portraitSearchPage_->objectName();
    items.value = 6;
    devicesVec << items;
    items.childrens.clear();
    items.name = videoAnalysisPage_->objectName();
    items.value = 7;
    devicesVec << items;
    for(auto value : devicesVec){
        createTreeItem(treeW_,nullptr,value);
    }
    treeW_->expandItem(treeW_->topLevelItem(0));
    treeW_->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    treeW_->headerItem()->setSizeHint(0,QSize(-1,45));
    treeW_->headerItem()->setText(0,tr(""));
    connect(treeW_,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeWidgetItemClicked(QTreeWidgetItem*,int)));
    treeW_->setCurrentItem(treeW_->topLevelItem(0));

    setUserStyle(userStyle());
}

void TargetSearch::setUserStyle(int s)
{
    QPalette pal;
    if(0 == s){
        treeW_->setStyleSheet("QTreeView{"
                               "border:none;"
                               "font-size: 16px;"
                               "color: #CECECE;"
                               "border-radius: 10px;"
                               "background-color: rgba(0, 0, 0, 90);}");
        treeW_->verticalScrollBar()->setStyleSheet(
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
        treeW_->header()->setStyleSheet("QHeaderView{"
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

void TargetSearch::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void TargetSearch::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, TargetSearch::itemData &items)
{
    QTreeWidgetItem *item{nullptr};
    if(parentItem){
        item = new QTreeWidgetItem(parentItem, QStringList() << items.name, items.childrens.isEmpty());
    }else{
        item = new QTreeWidgetItem(treeW, QStringList() << items.name, items.childrens.isEmpty());
    }
    item->setData(0,Qt::UserRole,items.value);
    if(item->type()){
        item->setSizeHint(0,QSize(-1,50));
    }
    for(auto value : items.childrens){
        createTreeItem(treeW,item,value);
    }
}

void TargetSearch::slotTreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    if(column != 0){
        return;
    }

    stackedW_->setCurrentIndex(item->data(0,Qt::UserRole).toInt());
}

bool TargetSearch::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchedWid = qobject_cast<QWidget*>(watched);
    bool isIn = false;
    for(int i = 0; i < stackedW_->count(); i++){
        if(stackedW_->widget(i) == watchedWid){
            isIn = true;
            break;
        }
    }
    if(isIn && event->type() == QEvent::Paint){
        QPainter p(watchedWid);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,50));
        p.drawRoundedRect(stackedW_->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}
