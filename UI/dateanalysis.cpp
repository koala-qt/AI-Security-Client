#include <QHBoxLayout>
#include <QEvent>
#include <QTreeWidget>
#include <QPainter>
#include <QScrollBar>
#include <QHeaderView>
#include <QStackedWidget>
#include <QTabWidget>
#include "dateanalysis.h"
#include "trackingpage.h"
#include "facelinktable.h"
#include "facelinkpage.h"

DateAnalysis::DateAnalysis(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Data analysis"));
    backImg_.load("images/Mask.png");
    treeW_ = new QTreeWidget;
    stackedW_ = new QStackedWidget;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(treeW_,327);
    mainLay->addWidget(stackedW_,1890);
    setLayout(mainLay);

    trackingW_ = new TrackingPage(wm);
    tabWidgetW_ = new QTabWidget;
    faceLinkTable_ = new FacelinkTable(wm);
    faceLinkPage_ = new FaceLinkPage(wm);
    tabWidgetW_->addTab(faceLinkTable_,faceLinkTable_->objectName());
    tabWidgetW_->addTab(faceLinkPage_,faceLinkPage_->objectName());
    stackedW_->addWidget(trackingW_);
    stackedW_->addWidget(tabWidgetW_);
    stackedW_->installEventFilter(this);
    QVector<itemData> devicesVec;
    itemData items;
    items.name = trackingW_->objectName();
    items.value = 0;
    devicesVec << items;
    items.name = faceLinkTable_->objectName();
    items.value = 1;
    devicesVec << items;
    for(auto value : devicesVec){
        createTreeItem(treeW_,nullptr,value);
    }
    treeW_->expandAll();
    treeW_->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    treeW_->headerItem()->setSizeHint(0,QSize(-1,45));
    treeW_->headerItem()->setText(0,tr(""));
    connect(treeW_,SIGNAL(itemClicked(QTreeWidgetItem*,int)),this,SLOT(slotTreeWidgetItemClicked(QTreeWidgetItem*,int)));
    treeW_->setCurrentItem(treeW_->topLevelItem(0));
}

void DateAnalysis::setUserStyle(WidgetManagerI::SkinStyle s)
{
    if(WidgetManagerI::Danyahei == s){
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
        tabWidgetW_->setStyleSheet("QTabBar{"
                                  "border: 0px;"
                                  "background: transparent;"
                                  "}"
                                  "QTabWidget::tab-bar"
                                  "{"
                                  "background-color: red;"
                                  "subcontrol-position: left;"
                                  "}"
                                  "QTabBar::tab{"
                                  "min-width: 120px;"
                                  "min-height: 40px;"
                                  "color: white;"
                                  "background: transparent;"
                                  "}"
                                  "QTabBar::tab:selected{"
                                  "background: rgba(255,255,255,0.3);"
                                  "border: none;"
                                  "}"
                                  "QTabWidget::pane{"
                                  "border-top: 1px solid rgba(255,255,255,0.4);"
                                  "background: transparent;"
                                  "}");
    }
}

bool DateAnalysis::eventFilter(QObject *watched, QEvent *event)
{
    if(qobject_cast<QWidget*>(watched) == stackedW_ && event->type() == QEvent::Paint){
        QPainter p(stackedW_);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,50));
        p.drawRoundedRect(stackedW_->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}

void DateAnalysis::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void DateAnalysis::createTreeItem(QTreeWidget *treeW, QTreeWidgetItem *parentItem, DateAnalysis::itemData &items)
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

void DateAnalysis::slotTreeWidgetItemClicked(QTreeWidgetItem *item, int column)
{
    if(column != 0 || !item->type()){
        return;
    }

    stackedW_->setCurrentIndex(item->data(0,Qt::UserRole).toInt());
}
