#include <QHBoxLayout>
#include <QWebEngineView>
#include <QWebChannel>
#include <QSettings>
#include <QApplication>
#include "reportpage.h"

ReportPage::ReportPage(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Report"));
    webView_ = new QWebEngineView;
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(webView_);
    setLayout(mainLay);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    webHost_ = configSetting.value("Http/Javahost").toString();
    webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/report/report.html"));
    webBridge_ = new ReportWebBridge(webView_);
    QWebChannel *channel = new QWebChannel(webBridge_);
    webView_->page()->setWebChannel(channel);
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ReportPage::setUserStyle(int s)
{

}

ReportWebBridge::ReportWebBridge(QObject *parent):
    QObject(parent)
{

}

void ReportWebBridge::setHostName(QString s)
{
    hostName_ = s;
}

void ReportWebBridge::onInitsized()
{
    emit sigHostNameChanged(hostName_);
}
