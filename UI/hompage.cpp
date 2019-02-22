#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <QListWidget>
#include <QWebEngineView>
#include <QApplication>
#include <QPainter>
#include "hompage.h"

HomPage::HomPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Home"));
    backImg_.load("images/Mask.png");
    eventCombox_ = new QComboBox;
    eventListW_ = new QListWidget;
    webView_ = new QWebEngineView;
    QHBoxLayout *mainLay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(eventCombox_);
    vlay->addWidget(eventListW_);
    mainLay->addWidget(webView_,13);
    mainLay->addLayout(vlay,3);
    setLayout(mainLay);

    eventListW_->setViewMode(QListWidget::IconMode);
    eventListW_->setSpacing(10);
    eventListW_->setMovement(QListView::Static);
    webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/map.html"));
    webView_->page()->setBackgroundColor(Qt::transparent);

    setUserStyle(userStyle());
}

void HomPage::setUserStyle(int s)
{
    if(s == 0){
        eventListW_->setStyleSheet("QListWidget{"
                                   "background-color:rgba(0,0,0,30);"
                                   "border-radius:6px;"
                                   "border:none;"
                                   "}");

        eventCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: transparent;"
                    "border-radius: 6px;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 18px;"
                    "background-color: #525964;"
                    "border: 1px solid #CECECE;"
                    "border-radius: 6px;"
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

void HomPage::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}
