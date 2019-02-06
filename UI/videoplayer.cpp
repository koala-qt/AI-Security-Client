#include <QMouseEvent>
#include <QHBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QThread>
#include <QTimer>
#include <QMenu>
#include <QGroupBox>
#include <QRadioButton>
#include <QMessageBox>
#include <QDebug>
#include "service/restservice.h"
#include "components/PaintAera/paintarea.h"
#include "canvaswidget.h"
#include "videoplayer.h"

#pragma execution_character_set("utf-8")
VideoPlayer::VideoPlayer(QWidget *parent):
    Klvideowidget(parent)
{
    m_canvas = new CanvasWidget;
    m_canvas->setObjectName("canvasW");
    m_canvas->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(m_canvas);
    hlay->setMargin(0);
    hlay->setSpacing(0);
    setLayout(hlay);
    menu_ = new QMenu(this);
    menu_->addAction(tr("设置警戒区"),this,[&]{
        if(playState() != Playing){
            QMessageBox::information(this,tr("设置警戒区"),tr("需要在视频播放情况下设置"));
            return;
        }
        QDialog setWaringAreaDialog(this);
        QPalette pal = setWaringAreaDialog.palette();
        pal.setColor(QPalette::Background,QColor(54,62,64));
        setWaringAreaDialog.setPalette(pal);
        setWaringAreaDialog.setAutoFillBackground(true);
        QDialogButtonBox *buttonBox = new QDialogButtonBox;
        QPushButton *okBtn = buttonBox->addButton(tr("确定"),QDialogButtonBox::AcceptRole);
        okBtn->setStyleSheet("QPushButton{"
                             "color: white;"
                             "background-color: rgb(59,69,78);"
                             "}"
                             "QPushButton:pressed{"
                             "background-color: rgb(49,54,57);"
                             "}");
        QPushButton *cancelBtn = buttonBox->addButton(tr("取消"),QDialogButtonBox::RejectRole);
        cancelBtn->setStyleSheet("QPushButton{"
                             "color: white;"
                             "background-color: rgb(59,69,78);"
                             "}"
                             "QPushButton:pressed{"
                             "background-color: rgb(49,54,57);"
                             "}");

        PaintArea *paintArea = new PaintArea;
        QHBoxLayout *hlay = new QHBoxLayout;
        QGroupBox *groupBox = new QGroupBox;
        QRadioButton *radioBtn = new QRadioButton(tr("普通门报警"));
        pal = radioBtn->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        radioBtn->setPalette(pal);
        radioBtn->setChecked(true);
        QPalette normalPal,ABPalette;
        normalPal.setColor(QPalette::Foreground,Qt::red);
        ABPalette.setColor(QPalette::Foreground,Qt::yellow);
        paintArea->setPalette(normalPal);
        connect(radioBtn,&QRadioButton::clicked,paintArea,[paintArea,&normalPal]{
            paintArea->setPalette(normalPal);
            paintArea->update();
        });
        hlay->addWidget(radioBtn);
        radioBtn = new QRadioButton(tr("AB门报警"));
        pal = radioBtn->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        radioBtn->setPalette(pal);
        connect(radioBtn,&QRadioButton::clicked,paintArea,[paintArea,&ABPalette]{
            paintArea->setPalette(ABPalette);
            paintArea->update();
        });
        hlay->addWidget(radioBtn);
        hlay->setAlignment(Qt::AlignLeft);
        groupBox->setLayout(hlay);

        QVBoxLayout *mainLay = new QVBoxLayout;
        mainLay->addWidget(paintArea);
        hlay = new QHBoxLayout;
        hlay->addWidget(groupBox);
        hlay->addStretch();
        mainLay->addLayout(hlay);
        hlay = new QHBoxLayout;
        hlay->addWidget(buttonBox);
        hlay->setAlignment(Qt::AlignRight);
        mainLay->addLayout(hlay);
        setWaringAreaDialog.setLayout(mainLay);
        paintArea->setCanvasBackground(QPixmap::fromImage(grabFramebuffer()).scaled(paintArea->sizeHint(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

        connect(buttonBox,&QDialogButtonBox::rejected, &setWaringAreaDialog, &QDialog::reject);
        connect(buttonBox,&QDialogButtonBox::accepted, &setWaringAreaDialog, &QDialog::accept);
        QDialogButtonBox::ButtonRole returnRole = QDialogButtonBox::ButtonRole(setWaringAreaDialog.exec());
        if(returnRole == QDialog::Accepted){
            QVector<QPolygonF> polygons = paintArea->polygons();
            qreal xPer = videoWidth() / (qreal)paintArea->width();
            qreal yPer = videoHeidht() / (qreal)paintArea->height();
            bool isABDoor = radioBtn->isChecked() ? true : false;
            AreaType::type ty = isABDoor ? AreaType::ABDOOR : AreaType::FORBIDDENZONE;
            QVector<QPair<int,QPolygonF>> areas;
            for(QPolygonF &polygon : polygons) {
                for(QPointF &pf : polygon) {
                    pf.setX(pf.x() * xPer);
                    pf.setY(pf.y() * yPer);
                }
                areas << qMakePair(ty,polygon);
            }

            BLL::Worker *w = new BLL::RestService(workerM_);
            RestServiceI *serI = dynamic_cast<RestServiceI*>(w);
            connect(serI,&RestServiceI::sigResultState,this,[&,areas](bool s){
                if(s){
                    slotSetPolygons(areas);
                }else{
                    QMessageBox::information(this,tr("设置警戒区"),tr("设置失败"));
                }
            });
            serI->setWaringArea(m_deviceId,areas);
            workerM_->startWorker(w);
        }else if(returnRole == QDialog::Rejected){
            qDebug() << "rejected";
        }
    });
    setContextMenuPolicy(Qt::CustomContextMenu);

    connect(this,SIGNAL(sigVideoStart(int,int)),this,SLOT(slotOnStarted(int,int)));
    connect(this,&VideoPlayer::sigError,this,[this](QString errorStr){
        if(oldRtsp_.isEmpty() || oldRtsp_ == url()){
            m_canvas->slotSetErrorStr(QString(tr("正在重连%1").arg(++reconnectCount_)));
            QTimer::singleShot(5000,this,[this]{
                qDebug() << "restart" << url();
                startPlay(url(),decoderName());
            });
        }
        oldRtsp_ = url();
    });
    connect(this,&VideoPlayer::sigVideoStoped,this,[this]{
        slotSetPolygons(QVector<QPair<int,QPolygonF>>());
        slotSetRects(QVector<QRect>());
    });
    connect(this,&VideoPlayer::customContextMenuRequested,this,[&](QPoint p){
        menu_->move(mapToGlobal(p));
        menu_->show();
    });
    connect(qApp,&QApplication::lastWindowClosed,this,[this]{
        workerM_ = nullptr;
    });
}

void VideoPlayer::setWorkerManager(BLL::WorkerManager *wm)
{
    workerM_ = wm;
}

void VideoPlayer::play(QString url, QString decoderFactoryName, QString id, QString name)
{
    m_deviceId = id;
    stop();
    m_canvas->slotSetPosition(name);
    startPlay(url,decoderFactoryName);
    reconnectCount_ = 0;
}

CanvasWidget *VideoPlayer::canvasWidget() const
{
    return m_canvas;
}

QString VideoPlayer::cameraId() const
{
    return m_deviceId;
}

void VideoPlayer::slotSetPolygons(QVector<QPair<int, QPolygonF> > points)
{
    if(playState() != Playing){
        m_canvas->slotSetPolygons(QVector<QPair<QColor,QPolygonF>>());
    }else{
        QVector<QPair<QColor,QPolygonF>> polygons;
        std::transform(points.begin(),points.end(),std::back_inserter(polygons),[](QPair<int,QPolygonF> &value){
            QColor   c;
            if(value.first == AreaType::FORBIDDENZONE){
                c = Qt::red;
            }else if(value.first == AreaType::ABDOOR){
                c = Qt::yellow;
            }
            return qMakePair(c,value.second);
        });
        m_canvas->slotSetPolygons(polygons);
    }
}

void VideoPlayer::slotSetRects(QVector<QRect> rs)
{
    if(playState() != Playing){
        m_canvas->slotSetRects(QVector<QRect>());
    }else{
        m_canvas->slotSetRects(rs);
    }
}

void VideoPlayer::slotOnStarted(int w, int h)
{
    if(!workerM_){
        return;
    }
    oldRtsp_.clear();
    reconnectCount_ = 0;
    m_canvas->slotSetPainterCoordinate(w,h);

    BLL::Worker *worker = new BLL::RestService(workerM_);
    RestServiceI *serI = dynamic_cast<RestServiceI*>(worker);
    connect(serI,SIGNAL(sigWaringAreas(QVector<QPair<int,QPolygonF> >)),this,SLOT(slotSetPolygons(QVector<QPair<int, QPolygonF> >)));
    serI->getWaringArea(m_deviceId);
    workerM_->startWorker(worker);
}
