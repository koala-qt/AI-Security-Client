#include <QHBoxLayout>
#include <QWebEngineView>
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
    webView_->page()->setBackgroundColor(Qt::transparent);
}

void ReportPage::setUserStyle(int s)
{

}
