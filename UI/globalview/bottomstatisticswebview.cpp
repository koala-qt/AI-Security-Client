#include "bottomstatisticswebview.h"

#include <QWebChannel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <QWebEnginePage>

BottomStatisticsWebView::BottomStatisticsWebView(QWidget *parent)
{
    QWebChannel *channel = new QWebChannel(this);
    m_pWebBridge = new BottomStatisticsBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(m_pWebBridge));
    page()->setWebChannel(channel);
    //load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/tree.html"));
    page()->setBackgroundColor(Qt::transparent);
    setContextMenuPolicy(Qt::NoContextMenu);
}
