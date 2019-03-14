#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QBitmap>
#include <QDebug>
#include "personitemwidget.h"
#include "UI/components/CircleAnimation/circleanimation.h"

PersonItemWidget::PersonItemWidget(WidgetI *parent):
    WidgetI(parent)
{
    personTypeL_ = new QLabel;
    captureTextL_ = new QLabel(tr("Captured photo"));
    registeredTextL_ = new QLabel(tr("Registered photo"));
    captredImgL_ = new QLabel;
    registedImgL_ = new QLabel;
    similrityL_ = new CircleAnimation;
    timeIconL_ = new QLabel;
    timeTextL_ = new QLabel;
    posIconL_ = new QLabel;
    posTextL_ = new QLabel;

    QVBoxLayout *mainLay = new QVBoxLayout;
    personTypeL_->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    personTypeL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    mainLay->addWidget(personTypeL_);
    QHBoxLayout *centerHlay = new QHBoxLayout;
    QVBoxLayout *vlay = new QVBoxLayout;
    captureTextL_->setAlignment(Qt::AlignCenter);
    captureTextL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    captredImgL_->setAlignment(Qt::AlignCenter);
    vlay->addWidget(captureTextL_);
    vlay->addWidget(captredImgL_);
    vlay->setAlignment(Qt::AlignCenter);
    centerHlay->addLayout(vlay);
    similrityL_->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    centerHlay->addWidget(similrityL_);
    vlay = new QVBoxLayout;
    registeredTextL_->setAlignment(Qt::AlignCenter);
    registeredTextL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    registedImgL_->setAlignment(Qt::AlignCenter);
    vlay->addWidget(registeredTextL_);
    vlay->addWidget(registedImgL_);
    vlay->setAlignment(Qt::AlignCenter);
    centerHlay->addLayout(vlay);
    mainLay->addLayout(centerHlay);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(timeIconL_);
    hlay->addWidget(timeTextL_);
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);
    hlay = new QHBoxLayout;
    hlay->addWidget(posIconL_);
    hlay->addWidget(posTextL_);
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay);
    mainLay->setContentsMargins(0,5,0,5);
    setLayout(mainLay);

    timeIconL_->setPixmap(QPixmap("images/tipIcon.png"));
    posIconL_->setPixmap(QPixmap("images/cameraIcon.png"));

    posIconL_->hide();
    timeIconL_->hide();
    timeTextL_->hide();
    posTextL_->hide();
    setUserStyle(userStyle());
}

PersonItemWidget::~PersonItemWidget()
{

}

void PersonItemWidget::setInfos(const NotifyEventI::PersonEventData &data)
{
#if 0
    QImage maskImg = data.image;
    maskImg.fill(Qt::transparent);
    QPainter p(&maskImg);
    p.setBrush(Qt::yellow);
    p.drawEllipse(p.window().center(),p.window().width() >> 1,p.window().width() >> 1);
    p.end();
    QPixmap faceCapturePix = QPixmap::fromImage(data.image);
    faceCapturePix.setMask(QPixmap::fromImage(maskImg).createMaskFromColor(Qt::yellow,Qt::MaskOutColor));
    captredImgL_->setPixmap(faceCapturePix.scaled(captredImgL_->size(),Qt::KeepAspectRatio));
    personTypeL_->setText(data.personTypenName);
    if(!data.faceImages.isEmpty()){
        QImage registedImg = data.faceImages.first().scaled(data.image.size());
        QImage maskImg = registedImg;
        maskImg.fill(Qt::transparent);
        QPainter p(&maskImg);
        p.setBrush(Qt::yellow);
        p.drawEllipse(p.window().center(),p.window().width() >> 1,p.window().width() >> 1);
        p.end();
        QPixmap registerPix = QPixmap::fromImage(registedImg);
        registerPix.setMask(QPixmap::fromImage(maskImg).createMaskFromColor(Qt::yellow,Qt::MaskOutColor));
        registedImgL_->setPixmap(registerPix.scaled(registedImgL_->size(),Qt::KeepAspectRatio));
    }
#else
    captredImgL_->setPixmap(QPixmap::fromImage(data.image));
    personTypeL_->setText(data.personTypenName);
    if(!data.faceImages.isEmpty()){
        registedImgL_->setPixmap(QPixmap::fromImage(data.faceImages.first().scaled(data.image.size(),Qt::KeepAspectRatio)));
    }
#endif
    similrityL_->setStartValue(0);
    similrityL_->setEndValue(data.faceSimilarity);
    timeTextL_->setText(data.timeStamp.toString("yyyy-MM-dd HH:mm:ss"));
    posTextL_->setText(data.deviceName);
}

void PersonItemWidget::setUserStyle(int s)
{
    QFont f;
    if(s == 0){
        personTypeL_->setStyleSheet("QLabel{"
                                    "color: rgba(255,255,255,0.75);"
                                    "border: 0px;"
                                    "font-size: 12px;"
                                    "font-weight: bold;"
                                    "}");
        captureTextL_->setStyleSheet("QLabel{"
                                     "color: rgba(255,255,255,0.75);"
                                     "font-size: 12px;"
                                     "border: 0px;"
                                     "}");
        registeredTextL_->setStyleSheet("QLabel{"
                                        "color: rgba(255,255,255,0.75);"
                                        "font-size: 12px;"
                                        "border: 0px;"
                                        "}");
        timeTextL_->setStyleSheet("QLabel{"
                                  "color: rgba(255,255,255,0.75);"
                                  "font-size: 12px;"
                                  "border: 0px;"
                                  "}");
        posTextL_->setStyleSheet("QLabel{"
                                 "color: rgba(255,255,255,0.75);"
                                 "font-size: 12px;"
                                 "border: 0px;"
                                 "}");
        f = similrityL_->font();
        f.setPixelSize(10);
        similrityL_->setFont(f);
    }
}
