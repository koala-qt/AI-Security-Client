#include <QWebEngineView>
#include <QHBoxLayout>
#include <QSettings>
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
    webView_->load(QUrl(tr("%1#/login").arg(webHost_)));
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ResourceManagePage::setUserStyle(int s)
{

}
