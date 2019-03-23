#include <QHBoxLayout>
#include <QWebEngineView>
#include <QDebug>
#include <QSettings>
#include <QWebChannel>
#include <QUrl>
#include <QApplication>
#include "waitinglabel.h"
#include "informationdialog.h"
#include "golbalviewwidget.h"
#include "glviewmapwidget.h"
#include "UI/movielabel.h"

GolbalViewWidget::GolbalViewWidget(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Home"));
    m_backgroundImg.load("images/glview/bg.png");

    menbanL_ = new QLabel(this);
    menbanL_->setScaledContents(true);
    menbanL_->setPixmap(QPixmap("images/glview/glviewbg.png"));

    m_mapWgt = new GlViewMapWidget(this);
    m_mapWgt->move(432, 0);

    // web
    webView_ = new QWebEngineView(this);
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(webView_);
    setLayout(mainLay);

    QSettings configSetting("config.ini",QSettings::IniFormat);
    webHost_ = configSetting.value("Http/Javahost").toString();
    QWebChannel *webChannel = new QWebChannel(webView_);
    webBridge_ = new GolbalWebBridge(webChannel);
    webChannel->registerObject("Bridge",webBridge_);
    webView_->page()->setWebChannel(webChannel);
    webView_->page()->setBackgroundColor(Qt::transparent);
    webBridge_->setHostName(webHost_);
    webView_->setContextMenuPolicy(Qt::NoContextMenu);

    webView_->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/golbalview/index.html"));
    setUserStyle(userStyle());
}

void GolbalViewWidget::setUserStyle(int style)
{
    if (0 == style)
    {

    }
}

void GolbalViewWidget::resizeEvent(QResizeEvent *event)
{
    menbanL_->setFixedSize(size());
}

void GolbalViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(), m_backgroundImg);
}

void GolbalViewWidget::mousePressEvent(QMouseEvent *event)
{
    WidgetI::mousePressEvent(event);
}

GolbalWebBridge::GolbalWebBridge(QObject *parent):
    QObject(parent)
{

}

void GolbalWebBridge::setHostName(QString s)
{
    if(!s.isEmpty() && s.right(1) == '/'){
        s.remove(s.count() - 1, 1);
    }
    host_ = s;
}

void GolbalWebBridge::onInitsized()
{
    emit sigHostNameChanged(host_);
}
