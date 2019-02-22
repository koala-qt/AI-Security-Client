#include <QWebEngineView>
#include <QHBoxLayout>
#include "resourcemanagepage.h"

ResourceManagePage::ResourceManagePage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Resource"));
    QHBoxLayout *mainLay = new QHBoxLayout;
    webView_ = new QWebEngineView;
    mainLay->addWidget(webView_);
    setLayout(mainLay);

    webView_->load(QUrl("http://192.168.100.103:18081/#/login"));
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ResourceManagePage::setUserStyle(int s)
{

}
