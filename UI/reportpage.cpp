#include <QHBoxLayout>
#include <QWebEngineView>
#include "reportpage.h"

ReportPage::ReportPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Report"));
    webView_ = new QWebEngineView;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(webView_);
    setLayout(mainLay);

    webView_->load(QUrl("http://192.168.100.103:18081/#/login"));
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ReportPage::setUserStyle(int s)
{

}
