#include <QWebEngineView>
#include <QHBoxLayout>
#include <QWebChannel>
#include <QSettings>
#include <QApplication>
#include <QWebEngineProfile>
#include <QFileDialog>
#include <QStandardPaths>
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
    QWebChannel *webChannel = new QWebChannel(webView_);
    webBridge_ = new ResourceWebBridge(webChannel);
    webChannel->registerObject("Bridge",webBridge_);
    webView_->page()->setWebChannel(webChannel);
    webView_->page()->setBackgroundColor(Qt::transparent);
    webBridge_->setHostName(webHost_);

    connect(webView_->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),this,SLOT(slotLoadRequest(QWebEngineDownloadItem*)));
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

void ResourceManagePage::slotLoadRequest(QWebEngineDownloadItem *download)
{
    QString filenName = QFileDialog::getSaveFileName(this,tr("download"),download->path());
    if(filenName.isEmpty())return;
    download->accept();
}

ResourceWebBridge::ResourceWebBridge(QObject *parent):
    QObject(parent)
{

}

void ResourceWebBridge::setHostName(QString s)
{
    if(!s.isEmpty() && s.right(1) == '/'){
        s.remove(s.count() - 1, 1);
    }
    host_ = s;
}

void ResourceWebBridge::onInitsized()
{
    emit sigHostNameChanged(host_);
}
