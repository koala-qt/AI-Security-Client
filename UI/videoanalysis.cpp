#include <QDebug>
#include <QStackedWidget>
#include <QApplication>
#include <QVBoxLayout>
#include <QPainter>
#include "service/servicei.h"
#include "videoanalysis.h"
#include "videoanalysisdata.h"
#include "uploadvideoprogress.h"
#include "selectuploadvide.h"
#include "informationdialog.h"

VideoAnalysis::VideoAnalysis(WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Video Analysis"));
    stackedW_ = new QStackedWidget;
    videoDataW_ = new VideoAnalysisData;
    progressW_ = new UploadVideoProgress;
    selectVideoW_ = new SelectUploadVide;

    QHBoxLayout *mainLay = new QHBoxLayout;
    mainLay->addWidget(stackedW_);
    mainLay->setMargin(0);
    setLayout(mainLay);

    stackedW_->addWidget(selectVideoW_);
    stackedW_->addWidget(progressW_);
    stackedW_->addWidget(videoDataW_);
    selectVideoW_->installEventFilter(this);
    progressW_->installEventFilter(this);
    videoDataW_->installEventFilter(this);
    connect(selectVideoW_,SIGNAL(sigVideoSelected(QString)),this,SLOT(slotFileSelected(QString)));
    connect(progressW_,SIGNAL(sigCancelBtnClicked()),this,SLOT(slotCancelUPload()));
}

void VideoAnalysis::setUserStyle(int s)
{

}

bool VideoAnalysis::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchedWid = qobject_cast<QWidget*>(watched);
    bool isIn = false;
    for(int i = 0; i < stackedW_->count(); i++){
        if(stackedW_->widget(i) == watchedWid){
            isIn = true;
            break;
        }
    }
    if(isIn && event->type() == QEvent::Paint){
        QPainter p(watchedWid);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(48,54,68));
        p.drawRoundedRect(stackedW_->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }
    return WidgetI::eventFilter(watched,event);
}

void VideoAnalysis::slotFileSelected(QString videoFile)
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    upLoadVideoService_ = factoryI->makeRestServiceI();
    connect(upLoadVideoService_,&RestServiceI::sigResultState,this,[this](bool res){
        upLoadVideoService_ = nullptr;
        if(res){
            stackedW_->setCurrentIndex(2);
        }
    });
    connect(upLoadVideoService_,&RestServiceI::sigError,this,[this](QString str){
        upLoadVideoService_ = nullptr;
        stackedW_->setCurrentIndex(0);
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
    });
    connect(upLoadVideoService_,&RestServiceI::sigVideoUploadProgress,this,[this](double total,double uploaded){
        stackedW_->setCurrentIndex(1);
        progressW_->slotSetValue(total,uploaded);
    });
    upLoadVideoService_->uploadVideo(videoFile);
}

void VideoAnalysis::slotCancelUPload()
{
    if(upLoadVideoService_){
        upLoadVideoService_->cancelRequest();
    }
}
