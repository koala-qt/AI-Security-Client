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
    m_backgroundImg.load("images/app-background.png");

#if 0
    menbanL_ = new QLabel(this);
    menbanL_->setScaledContents(true);
    menbanL_->setPixmap(QPixmap("images/glview/glviewbg.png"));
#endif

    m_mapWgt = new GlViewMapWidget(this);
    m_mapWgt->move(432, 0);

    // web
#if 0
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
#else
    m_webView = new GlobalWebView(this);
    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(m_webView);
    setLayout(mainLay);
#endif

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
#if 0
    menbanL_->setFixedSize(size());
#endif
}

void GolbalViewWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
#if 1
    QPainter p(this);
    p.drawImage(rect(), m_backgroundImg);
#endif
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

GlobalWebView::GlobalWebView(QWidget *parent):
    QWebEngineView(parent)
{
    QSettings configSetting("config.ini",QSettings::IniFormat);
    QString webHost_ = configSetting.value("Http/Javahost").toString();
    QWebChannel *webChannel = new QWebChannel(this);
    m_webBridget = new GolbalWebBridge(webChannel);
    webChannel->registerObject("Bridge",m_webBridget);
    page()->setWebChannel(webChannel);
    page()->setBackgroundColor(Qt::transparent);
    m_webBridget->setHostName(webHost_);
    this->setContextMenuPolicy(Qt::NoContextMenu);

    this->load(QUrl::fromLocalFile(qApp->applicationDirPath() + "/jsHtml/golbalview/index.html"));
}

void GlobalWebView::mousePressEvent(QMouseEvent *event)
{

}

void GlobalWebView::mouseMoveEvent(QMouseEvent *event)
{

}

bool GlobalWebView::event(QEvent *event)
{
    if ( QEvent::ChildAdded == event->type())
    {
        QChildEvent *child_ev = static_cast<QChildEvent*>(event);
        QObject *obj = child_ev->child();
        QWidget *w = qobject_cast<QWidget*>(obj);
        if (nullptr != w)
        {
            this->m_child = w;
        }
    }
    else if ( QEvent::ChildRemoved == event->type() )
    {
        QChildEvent *child_ev = static_cast<QChildEvent*>(event);
        QObject *obj = child_ev->child();
        QWidget *w = qobject_cast<QWidget*>(obj);
        if ( this->m_child == w )
        {
            this->m_child = nullptr;
        }
    }
    else
    {

    }

    return QWebEngineView::event(event);
}
