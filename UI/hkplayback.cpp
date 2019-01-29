#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include "hkplayback.h"
#include "HCNetSDK.h"
#include "timeaxis.h"

HKPlayBack::HKPlayBack(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *mainLay = new QVBoxLayout;
    videoW_ = new HKWidget;

    QHBoxLayout *hlay = new QHBoxLayout;
    play_ = new QPushButton;
    fastForward_ = new QPushButton;
    stop_ = new QPushButton;
    schedule_ = new TimeAxis;
    schedule_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    rewind_ = new QPushButton;
    hlay->addWidget(play_);
    hlay->addWidget(stop_);
    hlay->addWidget(rewind_);
    hlay->addWidget(schedule_);
    hlay->addWidget(fastForward_);

    mainLay->addWidget(videoW_);
    mainLay->addLayout(hlay);
    setLayout(mainLay);

    QPalette pal = schedule_->palette();
    pal.setColor(QPalette::Background,Qt::black);
    pal.setColor(QPalette::Foreground,Qt::white);
    schedule_->setPalette(pal);
    schedule_->setAutoFillBackground(true);
    schedule_->setMaxValue(24 * 60 * 60);
    schedule_->setMinValue(0);
    schedule_->setGraduation( 3 * 60 * 60 / 10);
    QFont f = schedule_->font();
    f.setPixelSize(12);
    schedule_->setBigTextFont(f);
    f.setPixelSize(10);
    schedule_->setMidTextFont(f);
    f.setPixelSize(15);
    schedule_->setCurTextFont(f);
    schedule_->setPen(QPen(Qt::white,3),QPen(Qt::lightGray,2),QPen(Qt::blue,1),QPen(Qt::white,1),QPen(Qt::yellow,1),QPen(Qt::green,1),QPen(Qt::red,1));
    schedule_->setGerateString([](double &value)->QString{
        if(value == 0){
            return "0";
        }else{
            QTime time(0,0,0);
            time = time.addSecs(value);
            return time.toString("HH:mm:ss");
        }
//        if(!static_cast<int>(::ceil(value)) % 5){
//            return QString::number(::ceil(value));
//        }else{
//            return QString::number(value);
//        }
    });
    schedule_->setValue(0);
    play_->setFixedSize(25,25);
    fastForward_->setFixedSize(25,25);
    rewind_->setFixedSize(25,25);
    connect(play_,SIGNAL(clicked(bool)),this,SLOT(slotPlay()));
    connect(fastForward_,SIGNAL(clicked(bool)),this,SLOT(slotFastForward()));
    connect(rewind_,SIGNAL(clicked(bool)),this,SLOT(slotRewind()));
    connect(stop_,SIGNAL(clicked(bool)),this,SLOT(slotStop()));
    connect(&time_,SIGNAL(timeout()),this,SLOT(slotUpdateTime()));
    connect(schedule_,&TimeAxis::valueChanged,this,[&]{
        slotValueChanged(schedule_->value());
    });
}

void HKPlayBack::startPlay(PlayBackParam &param)
{
    playBackArgs_ = param;
    videoW_->stop();
    if(videoW_->startPlayBackByTime(param)){
        schedule_->setValue(param.startTime.time().hour() * 60 * 60 + param.startTime.time().minute() * 60 + param.startTime.time().second());
        time_.start(1000);
        play_->setStyleSheet(playStyleStr_);
        isPlay_ = true;
        timeStuep_ = 1;
    }
}

void HKPlayBack::setUserStyle(WidgetManagerI::SkinStyle s)
{
    if(WidgetManagerI::Danyahei == s){
        pauseStyleStr_ = "QPushButton{"
                        "border: 0px;"
                        "image: url(images/symbols-play.png);"
                        "}"
                        "QPushButton:pressed{"
                        "padding: 1px;"
                        "}";
        playStyleStr_ = "QPushButton{"
                         "border: 0px;"
                         "image: url(images/symbols-play-copy.png);"
                         "}"
                         "QPushButton:pressed{"
                         "padding: 1px;"
                         "}";
        play_->setStyleSheet(pauseStyleStr_);
        fastForward_->setStyleSheet("QPushButton{"
                                   "border: 0px;"
                                   "image: url(images/symbols-forward.png);"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 1px;"
                                   "}");
        rewind_->setStyleSheet("QPushButton{"
                                   "border: 0px;"
                                   "image: url(images/symbols-backward.png);"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 1px;"
                                   "}");
    }
}

void HKPlayBack::slotPlay()
{
    if(isPlay_){
        if(videoW_->pause()){
            isPlay_ = false;
            play_->setStyleSheet(pauseStyleStr_);
            time_.stop();
        }
    }else{
        if(videoW_->play()){
            isPlay_ = true;
            play_->setStyleSheet(playStyleStr_);
            time_.start();
            timeStuep_ = 1;
        }
    }
}

void HKPlayBack::slotFastForward()
{
    if(videoW_->fastForward()){
        isPlay_ = false;
        timeStuep_ *= 2;
    }
}

void HKPlayBack::slotRewind()
{
    if(videoW_->reWind()){
        isPlay_ = false;
        timeStuep_ /=2 ;
        if(timeStuep_ == 0){
            timeStuep_ = 1;
        }
    }
}

void HKPlayBack::slotStop()
{
    videoW_->stop();
    time_.stop();
    schedule_->setValue(0);
    isPlay_ = false;
    play_->setStyleSheet(pauseStyleStr_);
}

void HKPlayBack::slotUpdateTime()
{
    unsigned int time = videoW_->getPlayTime();
    schedule_->setValue(schedule_->value() + timeStuep_);
}

void HKPlayBack::slotValueChanged(int value)
{
    time_.stop();
    QDateTime dtime(playBackArgs_.startTime);
    QTime time(0,0,0);
    time = time.addSecs(value);
    dtime.setTime(time);
    if(videoW_->setPlayTime(dtime)){
        time_.start();
    }
}
