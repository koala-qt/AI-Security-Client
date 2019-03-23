#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include "selectuploadvide.h"
#include "service/servicei.h"
#include "informationdialog.h"

SelectUploadVide::SelectUploadVide(WidgetI *parent):
    WidgetI(parent)
{
    videoLabel_ = new QLabel;
    videoTextL_ = new QLabel(tr("Upload a video to search"));
    upLoadBtn_ = new QPushButton(tr("Upload and analysis"));
    tipIconL_ = new QLabel;
    tipTextL_ = new QLabel(tr("Please select a video in MP4 format Video size is less than 1GB"));
    QVBoxLayout *mainLay = new QVBoxLayout;
    videoLabel_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(videoLabel_);
    videoTextL_->setAlignment(Qt::AlignCenter);
    mainLay->addWidget(videoTextL_);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(upLoadBtn_);
    hlay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(hlay);
    hlay = new QHBoxLayout;
    hlay->addWidget(tipIconL_);
    hlay->addWidget(tipTextL_);
    hlay->setAlignment(Qt::AlignCenter);
    mainLay->addLayout(hlay);
    mainLay->setAlignment(Qt::AlignCenter);
    setLayout(mainLay);

    videoLabel_->setPixmap(QPixmap("images/videoIcon.png"));
    upLoadBtn_->setIcon(QPixmap("images/cameraIcon.png"));
    tipIconL_->setPixmap(QPixmap("images/tipIcon.png"));
    upLoadBtn_->setMinimumHeight(44);
    upLoadBtn_->setMinimumWidth(210);
    QCursor curSor = upLoadBtn_->cursor();
    curSor.setShape(Qt::PointingHandCursor);
    upLoadBtn_->setCursor(curSor);
    connect(upLoadBtn_,SIGNAL(clicked(bool)),this,SLOT(slotUploadBtnClicked()));

    setUserStyle(userStyle());
}

void SelectUploadVide::setUserStyle(int s)
{
    if(s == 0){
        upLoadBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: rgba(255,255,255,0.75);"
                                  "border-radius: 6px;"
                                  "font-size: 18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "color: rgba(255,255,255,1);"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        videoTextL_->setStyleSheet("QLabel{"
                                   "color: rgba(255,255,255,0.5);"
                                   "}");
        tipTextL_->setStyleSheet("QLabel{"
                                 "color: rgba(255,255,255,0.75);"
                                   "}");
    }
}

void SelectUploadVide::slotUploadBtnClicked()
{
    QString videoFile = QFileDialog::getOpenFileName(this,tr("Select video file"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("Images (*.mp4)"));
    if(videoFile.isEmpty()){
        return;
    }

    emit sigVideoSelected(videoFile);
}
