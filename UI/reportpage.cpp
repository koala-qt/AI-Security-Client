#include <QHBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QSettings>
#include <QPainter>
#include <QMessageBox>
#include <QWebEngineProfile>
#include <QApplication>
#include <QWebEngineCookieStore>
#include "reportpage.h"

ReportPage::ReportPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Report"));
    backImg_.load(tr("images/Mask.png"));
    webView_ = new QWebEngineView;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(webView_);
    mainLay->setMargin(0);
    setLayout(mainLay);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    webHost_ = configSetting.value("Http/Javahost").toString();
    webBridge_ = new ReportWebBridge(webView_);
    webBridge_->setHostName(webHost_);
    QWebChannel *channel = new QWebChannel(webBridge_);
    channel->registerObject(QString("Bridge"),webBridge_);
    webView_->page()->setWebChannel(channel);
    webView_->page()->setBackgroundColor(Qt::transparent);
    webView_->setContextMenuPolicy(Qt::NoContextMenu);

    connect(webView_->page()->profile(),SIGNAL(downloadRequested(QWebEngineDownloadItem*)),this,SLOT(slotLoadRequest(QWebEngineDownloadItem*)));
}

void ReportPage::setUserStyle(int s)
{

}

bool ReportPage::event(QEvent *event)
{
    if(event->type() == QEvent::Show){
        if(webView_->url().isEmpty()){
//            webView_->page()->profile()->setPersistentCookiesPolicy(QWebEngineProfile::NoPersistentCookies);
//            webView_->page()->profile()->clearHttpCache(); // 清理缓存
//            webView_->page()->profile()->clearAllVisitedLinks(); // 清理浏览记录
//            QWebEngineCookieStore* pCookie = webView_->page()->profile()->cookieStore();
//            pCookie->deleteAllCookies(); // 清理cookie
//            pCookie->deleteSessionCookies(); // 清理会话cookie
            webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/report/index.html"));
        }
    }
    return WidgetI::event(event);
}

void ReportPage::paintEvent(QPaintEvent *event)
{
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.drawImage(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),backImg_);
}

void ReportPage::slotLoadRequest(QWebEngineDownloadItem *download)
{
    QMessageBox::StandardButton btn = QMessageBox::information(this,tr("download"),download->path(),QMessageBox::Ok,QMessageBox::Cancel);
    if(btn == QMessageBox::Ok){
        download->accept();
    }else{
        download->cancel();
    }
}

ReportWebBridge::ReportWebBridge(QObject *parent):
    QObject(parent)
{

}

void ReportWebBridge::setHostName(QString s)
{
    if(s.isEmpty())return;
    if(!s.isEmpty() && s.right(1) == '/'){
        s.remove(s.count() - 1, 1);
    }
    host_ = s;
}

void ReportWebBridge::onInitsized()
{
    emit sigHostNameChanged(host_);
}
