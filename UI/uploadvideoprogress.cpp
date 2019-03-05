#include <QLabel>
#include <QProgressBar>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QPushButton>
#include "uploadvideoprogress.h"

UploadVideoProgress::UploadVideoProgress(WidgetI *parent):
    WidgetI(parent)
{
    progressBackW_ = new QWidget;
    progressBarBackW_ = new QWidget;
    progressBar_ = new QProgressBar;
    videoNameL_ = new QLabel(tr("Uploading-video.mp4"));
    progressL_ = new QLabel;
    cancelBtn_ = new QPushButton(tr("cancel"));

    QVBoxLayout *mainBackLay = new QVBoxLayout;
    mainBackLay->addWidget(videoNameL_);

    QVBoxLayout *progessLay = new QVBoxLayout;
    progessLay->addWidget(progressBar_);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(progressL_);
    hlay->addStretch();
    hlay->addWidget(cancelBtn_);
    progessLay->addLayout(hlay);
    progressBarBackW_->setLayout(progessLay);
    mainBackLay->addWidget(progressBarBackW_);
    progressBackW_->setLayout(mainBackLay);

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->addWidget(progressBackW_);
    mainLay->setAlignment(Qt::AlignTop);
    setLayout(mainLay);

    progressBackW_->installEventFilter(this);
    progressBarBackW_->installEventFilter(this);
    connect(cancelBtn_,SIGNAL(clicked(bool)),this,SIGNAL(sigCancelBtnClicked()));
    setUserStyle(userStyle());
}

void UploadVideoProgress::setUserStyle(int s)
{
    if(s == 0){
        progressBar_->setStyleSheet("QProgressBar {"
                                    "border: 2px solid grey;"
                                    "border-radius: 5px;"
                                    "}"
                                    "QProgressBar::chunk {"
                                    "background-color: #05B8CC;"
                                    "width: 20px;}");
    }
}

bool UploadVideoProgress::eventFilter(QObject *watched, QEvent *event)
{
    QWidget *watchedWid = qobject_cast<QWidget*>(watched);
    if(watchedWid == progressBackW_ && event->type() == QEvent::Paint){
        QPainter p(watchedWid);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(48,54,68));
        p.drawRoundedRect(watchedWid->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }else if(watchedWid == progressBarBackW_ && event->type() == QEvent::Paint){
        QPainter p(watchedWid);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(0,0,0,25));
        p.drawRoundedRect(watchedWid->rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
    }

    return WidgetI::eventFilter(watched,event);
}

void UploadVideoProgress::slotSetValue(double total, double uploaded)
{
    progressBar_->setRange(0,static_cast<int>(total));
    progressBar_->setValue(static_cast<int>(uploaded));
    progressL_->setText(tr("Uploaded %1M/%2M").arg(total/(1024*1024)).arg(uploaded/(1024*1024)));
}
