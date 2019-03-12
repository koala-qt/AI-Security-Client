#include <QWebEngineView>
#include <QHBoxLayout>
#include <QWebChannel>
#include <QSettings>
#include <QApplication>
#include <QWebEngineProfile>
#include <QMessageBox>
#include <QStandardPaths>
#include "resourcemanagepage.h"

ResourceManagePage::ResourceManagePage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Resource"));
    backImg_.load(tr("images/Mask.png"));
    QHBoxLayout *mainLay = new QHBoxLayout;
    webView_ = new QWebEngineView;
    mainLay->addWidget(webView_);
    mainLay->setMargin(0);
    setLayout(mainLay);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    webHost_ = configSetting.value("Http/Javahost").toString();
    QWebChannel *webChannel = new QWebChannel(webView_);
    webBridge_ = new ResourceWebBridge(webChannel);
    webChannel->registerObject("Bridge",webBridge_);
    webView_->page()->setWebChannel(webChannel);
    webView_->page()->setBackgroundColor(Qt::transparent);
    webBridge_->setHostName(webHost_);
    webView_->setContextMenuPolicy(Qt::NoContextMenu);

    connect(webView_->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),this,SLOT(slotLoadRequest(QWebEngineDownloadItem*)));
}

void ResourceManagePage::setUserStyle(int s)
{

}

void ResourceManagePage::loadWebPage(int index)
{
    if(index == 0){
        webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + tr("/jsHtml/resource/device/index.html")));
    }else if(index == 1){
        webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + tr("/jsHtml/resource/person/index.html")));
    }else if(index == 2){
        webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + tr("/jsHtml/resource/markperson/index.html")));
    }
}

void ResourceManagePage::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.drawImage(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),backImg_);
}

void ResourceManagePage::slotLoadRequest(QWebEngineDownloadItem *download)
{
    QMessageBox::StandardButton btn = QMessageBox::information(this,tr("download"),download->path(),QMessageBox::Ok,QMessageBox::Cancel);
    if(btn == QMessageBox::Ok){
        download->accept();
    }else{
        download->cancel();
    }
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
