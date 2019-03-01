#include <QWebEngineView>
#include <QHBoxLayout>
#include <QSettings>
#include <QApplication>
#include "resourcemanagepage.h"

ResourceManagePage::ResourceManagePage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Resource"));
    QHBoxLayout *mainLay = new QHBoxLayout;
    webView_ = new QWebEngineView;
    mainLay->addWidget(webView_);
    setLayout(mainLay);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    webHost_ = configSetting.value("Http/Javahost").toString();
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ResourceManagePage::setUserStyle(int s)
{

}

void ResourceManagePage::loadWebPage(int index)
{
    if(index == 0){
        webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + tr("/jsHtml/resource/device_page/index.html")));
    }else if(index == 1){
        webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + tr("/jsHtml/resource/person_page/index.html")));
    }
}
