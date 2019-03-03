#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QStandardPaths>
#include <QPainter>
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
    upLoadBtn_->setMinimumWidth(198);
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
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size: 18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        videoTextL_->setStyleSheet("QLabel{"
                                   "color: rgb(85,96,120);"
                                   "}");
        tipTextL_->setStyleSheet("QLabel{"
                                   "color: rgb(126,140,177);"
                                   "}");
    }
}

void SelectUploadVide::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setBrush(QColor(48,54,68));
    p.drawRoundedRect(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
}

void SelectUploadVide::slotUploadBtnClicked()
{
    QString videoFile = QFileDialog::getOpenFileName(this,tr("Select video file"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),tr("Images (*.mp4)"));
    if(videoFile.isEmpty()){
        return;
    }

    emit sigVideoSelected(videoFile);
}
