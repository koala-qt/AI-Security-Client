#include <QLabel>
#include <QListWidget>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QApplication>
#include "videoanalysisdata.h"
#include "service/servicei.h"

VideoAnalysisData::VideoAnalysisData(WidgetI *parent):
    WidgetI(parent)
{
    titleTextL_ = new QLabel(tr("Analysis result"));
    countTextL_ = new QLabel;
    dataListW_ = new QListWidget;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QVBoxLayout *topVLay = new QVBoxLayout;
    topVLay->addWidget(titleTextL_);
    topVLay->addWidget(countTextL_);
    topVLay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(topVLay);
    mainLay->addWidget(dataListW_);
    setLayout(mainLay);

    NotifyServiceI* notifyServiceI_ = reinterpret_cast<NotifyServiceI*>(qApp->property("NotifyServiceI").toULongLong());
    connect(notifyServiceI_,SIGNAL(sigVideoFacePicture(QString,QImage)),this,SLOT(slotVideoAnalysisData(QString,QImage)));
}

void VideoAnalysisData::setUserStyle(int s)
{

}

void VideoAnalysisData::slotVideoAnalysisData(QString personId, QImage img)
{
    if(!personInfoMap_.contains(personId)){
        QListWidgetItem *item = new QListWidgetItem;
        dataListW_->addItem(item);
        PersonInfo *personInfoW = new PersonInfo;
        dataListW_->setItemWidget(item,personInfoW);
        personInfoMap_.insert(personId,personInfoW);
        item->setSizeHint(QSize(dataListW_->width(),500));
    }
    personInfoMap_.value(personId)->addItem(personId,img);
}

PersonInfo::PersonInfo(WidgetI *parent):
    WidgetI(parent)
{
    headImgL_ = new QLabel;
    personOrderL_ = new QLabel;
    personImgCountL_ = new QLabel;
    imgListW_ = new QListWidget;
    QVBoxLayout *mainLay = new QVBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(headImgL_,0,0,2,1);
    gridLay->addWidget(personOrderL_,0,1,1,1);
    gridLay->addWidget(personImgCountL_,1,1,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(gridLay);
    mainLay->addWidget(imgListW_);
    setLayout(mainLay);

    imgListW_->setFlow(QListWidget::LeftToRight);
    imgListW_->setMovement(QListWidget::Static);
    imgListW_->setViewMode(QListWidget::IconMode);
    imgListW_->setIconSize(QSize(112,112));
}

void PersonInfo::setUserStyle(int s)
{

}

void PersonInfo::addItem(QString s, QImage img)
{
    QListWidgetItem *item = new QListWidgetItem;
    item->setText(s);
    item->setTextAlignment(Qt::AlignCenter);
    item->setIcon(QPixmap::fromImage(img));
    imgListW_->addItem(item);

    if(!headImgL_->pixmap()){
        headImgL_->setPixmap(QPixmap::fromImage(img));
        personOrderL_->setText(s);
    }
    personImgCountL_->setText(tr("×%1").arg(imgListW_->count()));
}

void PersonInfo::clearItems()
{
    headImgL_->clear();
    personImgCountL_->clear();
    personOrderL_->clear();
    imgListW_->clear();
}
