#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QListWidget>
#include <QWebEngineView>
#include <QPainter>
#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QWebEnginePage>
#include <QWebChannel>
#include "hompage.h"
#include "personitemwidget.h"
#include "videoplayer.h"

HomPage::HomPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Home"));
    eventCombox_ = new QComboBox;
    eventListW_ = new QListWidget;
    eventBackW_ = new QWidget;
    webView_ = new QWebEngineView;
    QHBoxLayout *mainLay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(eventCombox_);
    vlay->addWidget(eventListW_);
    eventBackW_->setLayout(vlay);
    mainLay->addWidget(webView_,13);
    mainLay->addWidget(eventBackW_,3);
    setLayout(mainLay);

    eventCombox_->addItem(tr("All events"));
    eventCombox_->addItem(tr("Intruder events"));
    eventCombox_->addItem(tr("AB-Door evetns"));
    eventCombox_->addItem(tr("Climb evetns"));
    eventCombox_->addItem(tr("Gather evetns"));
    eventCombox_->addItem(tr("Blacklist events"));
    eventBackW_->installEventFilter(this);
    webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/index.html"));
    webView_->page()->setBackgroundColor(Qt::transparent);
    QWebChannel *channel = new QWebChannel(webView_);
    webBridge_ = new HomePageWebBridge(channel);
    channel->registerObject("Bridge", webBridge_);
    webView_->page()->setWebChannel(channel);

    connect(eventCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotEventComboxIndexChanged(int)));
    connect(webBridge_,SIGNAL(sigCameraClicked(QString)),this,SLOT(slotOnCameraClicked(QString)));
    notifyServiceI_ = reinterpret_cast<NotifyServiceI*>(qApp->property("NotifyServiceI").toULongLong());
    setUserStyle(userStyle());

    slotEventComboxIndexChanged(0);
    eventBackW_->hide();
}

void HomPage::setUserStyle(int s)
{
    if(s == 0){
        eventListW_->setStyleSheet("QListWidget{"
                                   "background-color:transparent;"
                                   "border-radius:0px;"
                                   "border:none;"
                                   "}");
        eventCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: transparent;"
                    "border-radius: 0px;"
                    "border: none;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: transparent;"
                    "border: none;"
                    "border-radius: 0px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "background-color: #525964;"
                    "border-radius: 6px;"
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
    }
}

//bool HomPage::event(QEvent *event)
//{
//    if(event->type() == QEvent::Show){
//        int imgItemH = (eventListW_->height() - (eventItemCount + 1) * eventListW_->spacing() - eventListW_->frameWidth() * 2) / eventItemCount;
//        int imgItemW = eventListW_->width() - 2 * eventListW_->spacing() - 2 * eventListW_->frameWidth();
//        eventItemSize_.setHeight(imgItemH);
//        eventItemSize_.setWidth(imgItemW);
//        for(int i = 0; i < eventListW_->count(); i++){
//            QListWidgetItem *item = eventListW_->item(i);
//            item->setSizeHint(eventItemSize_);
//        }
//    }

//    return WidgetI::event(event);
//}

void HomPage::resizeEvent(QResizeEvent *event)
{
    int imgItemH = (eventListW_->height() - (eventItemCount + 1) * eventListW_->spacing() - eventListW_->frameWidth() * 2) / eventItemCount;
    int imgItemW = eventListW_->width() - 2 * eventListW_->spacing() - 2 * eventListW_->frameWidth();
    eventItemSize_.setHeight(imgItemH);
    eventItemSize_.setWidth(imgItemW);

    eventItemSize_.setHeight(251);
    eventItemSize_.setWidth(456);
    for(int i = 0; i < eventListW_->count(); i++){
        QListWidgetItem *item = eventListW_->item(i);
        item->setSizeHint(eventItemSize_);
    }

    return WidgetI::resizeEvent(event);
}

bool HomPage::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchWid = qobject_cast<QWidget*>(watched);
    if(watchWid == eventBackW_ && event->type() == QEvent::Paint){
        QPainter p(watchWid);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(48,54,68));
        p.setRenderHint(QPainter::Antialiasing);
        p.drawRoundedRect(p.window().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }
    return WidgetI::eventFilter(watched,event);
}

void HomPage::slotEventComboxIndexChanged(int index)
{
    disconnect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)),this,SLOT(slotOnClimbEvent(NotifyEventI::ClimbEventData)));
    disconnect(notifyServiceI_,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)),this,SLOT(slotOngGatherEvent(NotifyEventI::GatherEventData)));
    if(index == 0){
        connect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)),Qt::UniqueConnection);
        connect(notifyServiceI_,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)),this,SLOT(slotOnClimbEvent(NotifyEventI::ClimbEventData)));
        connect(notifyServiceI_,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)),this,SLOT(slotOngGatherEvent(NotifyEventI::GatherEventData)));
    }else if(index == 1){
        connect(notifyServiceI_,SIGNAL(sigIntruderEvent(NotifyEventI::IntruderEventData)),this,SLOT(slotOnIntruderEvent(NotifyEventI::IntruderEventData)),Qt::UniqueConnection);
    }else if(index == 2){
        connect(notifyServiceI_,SIGNAL(sigABDoorEventData(NotifyEventI::ABDoorEventData)),this,SLOT(slotOnAbDoorEvent(NotifyEventI::ABDoorEventData)),Qt::UniqueConnection);
    }else if(index == 3){
        connect(notifyServiceI_,SIGNAL(sigClimbEventData(NotifyEventI::ClimbEventData)),this,SLOT(slotOnClimbEvent(NotifyEventI::ClimbEventData)));
    }else if(index == 4){
        connect(notifyServiceI_,SIGNAL(sigGatherEventData(NotifyEventI::GatherEventData)),this,SLOT(slotOngGatherEvent(NotifyEventI::GatherEventData)));
    }else if(index == 5){
        connect(notifyServiceI_,SIGNAL(sigPersonEventData(NotifyEventI::PersonEventData)),this,SLOT(slotOnPersonEvent(NotifyEventI::PersonEventData)),Qt::UniqueConnection);
    }
}

void HomPage::slotOnIntruderEvent(NotifyEventI::IntruderEventData evData)
{
    if(eventListW_->count() >= eventItemCount){
        QListWidgetItem *delItem = eventListW_->takeItem(eventItemCount - 1);
        eventListW_->removeItemWidget(delItem);
        delete delItem;
    }
    QListWidgetItem *item = new QListWidgetItem(nullptr,0);
    QPainter p(&evData.sceneImg);
    p.setBrush(QColor(200,0,0,100));
    p.drawPolygon(evData.warnZone);
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventListW_->insertItem(0,item);
    QWidget *itemW = new QWidget;
    QLabel *imgL = new QLabel;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(imgL);
    itemW->setLayout(mainLay);
    imgL->setPixmap(QPixmap::fromImage(evData.sceneImg.scaled(item->sizeHint())));
    eventListW_->setItemWidget(item,itemW);
}

void HomPage::slotOnPersonEvent(NotifyEventI::PersonEventData evData)
{
    if(eventListW_->count() >= eventItemCount){
        QListWidgetItem *delItem = eventListW_->takeItem(eventItemCount - 1);
        eventListW_->removeItemWidget(delItem);
        delete delItem;
    }
    QListWidgetItem *item = new QListWidgetItem(nullptr,2);
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventListW_->insertItem(0,item);
    PersonItemWidget *itemW = new PersonItemWidget;
    itemW->setStyleSheet("QWidget{"
                         "background-color: transparent;"
                         "color: rgb(126,140,177);"
                         "border-radius: 0px;"
                         "border: none;"
                         "}");
    itemW->setInfos(evData);
    eventListW_->setItemWidget(item,itemW);
}

void HomPage::slotOnAbDoorEvent(NotifyEventI::ABDoorEventData evData)
{
    if(eventListW_->count() >= eventItemCount){
        QListWidgetItem *delItem = eventListW_->takeItem(eventItemCount - 1);
        eventListW_->removeItemWidget(delItem);
        delete delItem;
    }
    QListWidgetItem *item = new QListWidgetItem(nullptr,0);
    QPainter p(&evData.sceneImg);
    p.setBrush(QColor(200,100,0,100));
    p.drawPolygon(evData.warnZone);
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventListW_->insertItem(0,item);
    QWidget *itemW = new QWidget;
    QLabel *imgL = new QLabel;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(imgL);
    itemW->setLayout(mainLay);
    imgL->setPixmap(QPixmap::fromImage(evData.sceneImg.scaled(item->sizeHint())));
    eventListW_->setItemWidget(item,itemW);
}

void HomPage::slotOnClimbEvent(NotifyEventI::ClimbEventData evData)
{
    if(eventListW_->count() >= eventItemCount){
        QListWidgetItem *delItem = eventListW_->takeItem(eventItemCount - 1);
        eventListW_->removeItemWidget(delItem);
        delete delItem;
    }
    QListWidgetItem *item = new QListWidgetItem(nullptr,0);
    QPainter p(&evData.sceneImg);
    p.setBrush(QColor(200,100,0,100));
    p.drawPolygon(evData.warnZone);
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventListW_->insertItem(0,item);
    QWidget *itemW = new QWidget;
    QLabel *imgL = new QLabel;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(imgL);
    itemW->setLayout(mainLay);
    imgL->setPixmap(QPixmap::fromImage(evData.sceneImg.scaled(item->sizeHint())));
    eventListW_->setItemWidget(item,itemW);
}

void HomPage::slotOngGatherEvent(NotifyEventI::GatherEventData evData)
{
    if(eventListW_->count() >= eventItemCount){
        QListWidgetItem *delItem = eventListW_->takeItem(eventItemCount - 1);
        eventListW_->removeItemWidget(delItem);
        delete delItem;
    }
    QListWidgetItem *item = new QListWidgetItem(nullptr,0);
    item->setData(Qt::UserRole + 1,evData.sceneId);
    item->setSizeHint(eventItemSize_);
    eventListW_->insertItem(0,item);
    QWidget *itemW = new QWidget;
    QLabel *imgL = new QLabel;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(imgL);
    itemW->setLayout(mainLay);
    imgL->setPixmap(QPixmap::fromImage(evData.sceneImg.scaled(item->sizeHint())));
    eventListW_->setItemWidget(item,itemW);
}

void HomPage::slotOnCameraClicked(QString rtsp)
{
    qDebug() << rtsp;
    VideoPlayer *player = new VideoPlayer(this);
    player->setWindowTitle(rtsp);
    player->setAttribute(Qt::WA_DeleteOnClose);
    player->setWindowFlags(Qt::Dialog | Qt::WindowCloseButtonHint);
    player->setWindowModality(Qt::ApplicationModal);
    player->setMinimumSize(960,540);
    player->startPlay(rtsp,"fmg_decoder");
    player->show();
}

HomePageWebBridge::HomePageWebBridge(QObject *parent):
    QObject(parent)
{

}

void HomePageWebBridge::setHostName(QString s)
{
    hostName_ = s;
}

void HomePageWebBridge::onInitsized()
{
    emit sigHostNameChanged(hostName_);
}

void HomePageWebBridge::onCameraClicked(QString rtsp)
{
    emit sigCameraClicked(rtsp);
}
