#include "rightstatisticswebview.h"

#include <QWebChannel>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>
#include <QWebEnginePage>

RightStatisticsWebView::RightStatisticsWebView(QWidget *parent)
{
    QWebChannel *channel = new QWebChannel(this);
    m_pWebBridge = new RightStatisticsBridge(channel);
    channel->registerObject("Bridge", qobject_cast<QObject*>(m_pWebBridge));
    page()->setWebChannel(channel);
    //load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/tree.html"));
    page()->setBackgroundColor(QColor(66,66,66,20));
    setContextMenuPolicy(Qt::NoContextMenu);
}
