#include "realplaymanager.h"
#include "videoplayer.h"
#include <QGridLayout>
#include <QMouseEvent>
#include <stack>
#include <QFocusFrame>
#include <QApplication>
#include <QMessageBox>
#include <QSettings>
#include <QDebug>
#include "informationdialog.h"

RealPlayManager::RealPlayManager(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    m_mainLay = new QGridLayout;
    m_mainLay->setMargin(3);
    setLayout(m_mainLay);
    m_focusFrame = new QFocusFrame(this);

    QSettings config("config.ini",QSettings::IniFormat);
    decoderName_ = config.value("VideoScreen/decoder").toString();
}

RealPlayManager::~RealPlayManager()
{
}

void RealPlayManager::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    switch (s) {
    case WidgetManagerI::Danyahei:
        pal = m_focusFrame->palette();
        pal.setColor(QPalette::Foreground,QColor(189,56,57));
        m_focusFrame->setPalette(pal);

        for(int i = 0; i < m_mainLay->count(); i++){
            QFont f = m_mainLay->itemAt(i)->widget()->font();
            f.setFamily(font().family());
            m_mainLay->itemAt(i)->widget()->setFont(f);
        }
        break;
    default:
        break;
    }
}

int RealPlayManager::screenCount() const
{
    return curRow * curCol - curBigRowSpan * curBigColSpan + 1 ;
}

void RealPlayManager::splitScreen(const int rows, const int cols, const int bigFrameRow, const int bigFrameCol, const int bigFrameRowSpan, const int bigFrameColSpan)
{
    if(rows == curRow && cols == curCol && bigFrameRow == curBigRow && bigFrameCol == curBigCol && bigFrameRowSpan == curBigRowSpan && bigFrameColSpan == curBigColSpan){
        return;
    }
    curRow = rows;
    curCol = cols;
    curBigRow = bigFrameRow;
    curBigCol = bigFrameCol;
    curBigRowSpan = bigFrameRowSpan;
    curBigColSpan = bigFrameColSpan;
    QStack<QWidget*> realFrameLayStack;
    int oldCount = m_mainLay->count();
    for(int i = oldCount - 1; i >= 0; --i){
        realFrameLayStack.push(m_mainLay->takeAt(i)->widget());
    }

    int loopCount = bigFrameRow * cols;
    int _row = -1, _col = -1, _rowSpan = -1 , _colSpan = -1;
    for(int i = 0 ; i < loopCount ; i ++){
        _row = i / bigFrameCol; _col = i % bigFrameCol; _rowSpan = 1; _colSpan = 1;
        if(!realFrameLayStack.empty()){
            m_mainLay->addWidget(realFrameLayStack.pop(), _row, _col,_rowSpan,_colSpan);
        }else{
            VideoPlayer *vp = new VideoPlayer;
            vp->setWorkerManager(widgetManger()->workerManager());
            m_mainLay->addWidget(vp,_row, _col,_rowSpan,_colSpan);
        }
    }
    loopCount = bigFrameCol * bigFrameRowSpan;
    for(int i = 0; i < loopCount; i ++){
        _row = bigFrameRow + i / bigFrameCol; _col = i % bigFrameCol; _rowSpan = 1; _colSpan = 1;
        if(!realFrameLayStack.empty()){
            m_mainLay->addWidget(realFrameLayStack.pop(), _row, _col,_rowSpan,_colSpan);
        }else{
            VideoPlayer *vp = new VideoPlayer;
            vp->setWorkerManager(widgetManger()->workerManager());
            m_mainLay->addWidget(vp, _row, _col,_rowSpan,_colSpan);
        }
    }

    _row = bigFrameRow; _col = bigFrameCol; _rowSpan = bigFrameRowSpan; _colSpan = bigFrameColSpan;
    if(!realFrameLayStack.empty()){
        m_mainLay->addWidget(realFrameLayStack.pop(), _row, _col,_rowSpan,_colSpan);
    }else{
        VideoPlayer *vp = new VideoPlayer;
        vp->setWorkerManager(widgetManger()->workerManager());
        m_mainLay->addWidget(vp,_row, _col,_rowSpan,_colSpan);
    }

    int colStart = bigFrameCol + bigFrameColSpan;
    int colCount = cols - colStart;
    loopCount = colCount * bigFrameRowSpan;
    for(int i = 0; i < loopCount; i++){
        _row = bigFrameRow + i / colCount; _col = colStart + i % colCount; _rowSpan = 1; _colSpan = 1;
        if(!realFrameLayStack.empty()){
            m_mainLay->addWidget(realFrameLayStack.pop(), _row, _col,_rowSpan,_colSpan);
        }else{
            VideoPlayer *vp = new VideoPlayer;
            vp->setWorkerManager(widgetManger()->workerManager());
            m_mainLay->addWidget(vp,_row, _col,_rowSpan,_colSpan);
        }
    }
    int rowStart = bigFrameRow + bigFrameRowSpan;
    int rowCount = rows - rowStart;
    loopCount = rowCount * cols;
    for(int i = 0; i < loopCount; i++){
        _row = rowStart + i / cols; _col = i % cols; _rowSpan = 1; _colSpan = 1;
        if(!realFrameLayStack.empty()){
            m_mainLay->addWidget(realFrameLayStack.pop(), _row, _col,_rowSpan,_colSpan);
        }else{
            VideoPlayer *vp = new VideoPlayer;
            vp->setWorkerManager(widgetManger()->workerManager());
            m_mainLay->addWidget(vp,_row, _col,_rowSpan,_colSpan);
        }
    }

    if(!realFrameLayStack.isEmpty()){
        m_focusFrame->setWidget(m_mainLay->itemAt(0)->widget());
        while (!realFrameLayStack.empty()) {
              QWidget *item = realFrameLayStack.pop();
              m_videoMap.take(dynamic_cast<VideoPlayer*>(item)->cameraId())->deleteLater();
              delete item;
        }
    }

    if(m_isFullScreen){
        closeVideoFullScreen();
    }
}

void RealPlayManager::playByOrder(QString url, QString id, QString name)
{
    for(int i = 0; i < m_mainLay->count(); i++){
        VideoPlayer *w = dynamic_cast<VideoPlayer*>(m_mainLay->itemAt(i)->widget());
        if(w->playState() == Klvideowidget::Stop){
            m_videoMap.remove(w->cameraId());
            emit sigVideoRemoved(w->cameraId());
            connect(w,SIGNAL(sigVideoStart(int,int)),this,SLOT(slotVideoStarted(int,int)),Qt::UniqueConnection);
            connect(w,SIGNAL(sigError(QString)),this,SLOT(slotVideoError(QString)),Qt::UniqueConnection);
            w->play(url, decoderName_,id,name);//h264_cuvid
            m_videoMap[id] = w;
            break;
        }
    }
}

void RealPlayManager::playByFocus(QString url, QString id, QString name)
{
    VideoPlayer *w {nullptr};
    if(! (w = dynamic_cast<VideoPlayer*>(m_focusFrame->widget()))){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(widgetManger()->currentStyle());
        infoDialog.showMessage(tr("Please select a window first"));
        infoDialog.exec();
        return;
    }

    if(m_videoMap.contains(id)){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(widgetManger()->currentStyle());
        infoDialog.showMessage(tr("Already in the play window"));
        infoDialog.exec();
        return;
    }

    m_videoMap.remove(w->cameraId());
    emit sigVideoRemoved(w->cameraId());
    connect(w,SIGNAL(sigVideoStart(int,int)),this,SLOT(slotVideoStarted(int,int)),Qt::UniqueConnection);
    connect(w,SIGNAL(sigError(QString)),this,SLOT(slotVideoError(QString)),Qt::UniqueConnection);
    w->stop();
    w->play(url, decoderName_,id,name);//h264_cuvid
    m_videoMap.insert(id,w);
}

void RealPlayManager::playByPosition(QString url, QString id, int pos)
{
    VideoPlayer *w = dynamic_cast<VideoPlayer*>(m_mainLay->itemAt(pos)->widget());
    if(!w){
        return;
    }

    m_videoMap.remove(w->cameraId());
    emit sigVideoRemoved(w->cameraId());
    connect(w,SIGNAL(sigVideoStart(int,int)),this,SLOT(slotVideoStarted(int,int)),Qt::UniqueConnection);
    connect(w,SIGNAL(sigError(QString)),this,SLOT(slotVideoError(QString)),Qt::UniqueConnection);
    w->play(url,decoderName_,id);//h264_cuvid
    m_videoMap[id] = w;
}

VideoPlayer *RealPlayManager::focusPlayer() const
{
    return qobject_cast<VideoPlayer*>(m_focusFrame->widget());
}

int RealPlayManager::frameCount() const
{
    return m_mainLay->count();
}

void RealPlayManager::mouseDoubleClickEvent(QMouseEvent *ev)
{
    QWidget *videoW = childAt(ev->pos());
    if(!videoW)return;

    if(!m_isFullScreen){
        videoShowFullScreen(videoW);
    }else{
        closeVideoFullScreen();
    }
}

void RealPlayManager::mousePressEvent(QMouseEvent *ev)
{
    QWidget *videoW = childAt(ev->pos());
    if(!videoW)return;
    m_focusFrame->setWidget(videoW->parentWidget());
}

void RealPlayManager::videoShowFullScreen(QWidget *vw)
{
    if(m_isFullScreen)return;
    for(int i = 0; i < m_mainLay->count(); i++){
        QWidget *w = m_mainLay->itemAt(i)->widget();
        w->setMaximumSize(0,0);
    }
    m_mainLay->setSpacing(0);
    vw->parentWidget()->setMaximumSize(width() - layout()->margin() * 2, height() - layout()->margin() * 2);
    vw->parentWidget()->setMinimumSize(width() - layout()->margin() * 2, height() - layout()->margin() * 2);
    updateGeometry();
    m_isFullScreen = true;
}

void RealPlayManager::closeVideoFullScreen()
{
    if(!m_isFullScreen)return;
    for(int i = 0; i < m_mainLay->count(); i++){
        QWidget *w = m_mainLay->itemAt(i)->widget();
        w->setMinimumSize(0,0);
        w->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    }
    m_mainLay->setSpacing(5);
    updateGeometry();
    m_isFullScreen = false;
}

void RealPlayManager::slotDectecBoxes(QPair<QString, QVector<QRect> > rects)
{
    VideoPlayer *player = nullptr;
    if(player = m_videoMap.value(rects.first)){
        player->slotSetRects(rects.second);
    }
}

void RealPlayManager::slotVideoStarted(int, int)
{
    VideoPlayer *w = qobject_cast<VideoPlayer*>(sender());
    if(!w)return;
    emit sigVideoStarted(w->cameraId());
}

void RealPlayManager::slotVideoError(QString)
{
    VideoPlayer *w = qobject_cast<VideoPlayer*>(sender());
    if(!w)return;
    emit sigVideoError(w->cameraId());
}
