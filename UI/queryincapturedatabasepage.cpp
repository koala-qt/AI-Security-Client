#include <QTabWidget>
#include <QPalette>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include "queryincapturedatabasepage.h"
#include "semanticsearchpage.h"
#include "facesearch.h"

#pragma execution_character_set("utf-8")
QueryInCaptureDataBasePage::QueryInCaptureDataBasePage(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    setObjectName(tr("Person analysis"));
    tabWidget_ = new QTabWidget;
    semanticPage_ = new SemanticSearchPage(wm);
    searchByImage_ = new FaceSearch(wm);
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(tabWidget_);
    setLayout(mainLay);

    tabWidget_->addTab(semanticPage_,semanticPage_->objectName());
    tabWidget_->addTab(searchByImage_,searchByImage_->objectName());

    setUserStyle(userStyle());
}

void QueryInCaptureDataBasePage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == s){
        tabWidget_->setStyleSheet("QTabBar{"
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

void QueryInCaptureDataBasePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(0,0,0,50));
    p.drawRoundedRect(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
}
