#include "leftstatisticswebview.h"

#include <QWebChannel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <QWebEnginePage>

LeftStatisticsWebView::LeftStatisticsWebView(QWidget *parent)
{
    QWebChannel *channel = new QWebChannel(this);
    m_pWebBridge = new LeftStatisticsBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(m_pWebBridge));
    page()->setWebChannel(channel);
    load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/index.html"));
    page()->setBackgroundColor(Qt::transparent);
    setContextMenuPolicy(Qt::NoContextMenu);
}
