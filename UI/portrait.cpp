#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include "portrait.h"
#include "cornerwidget.h"
#include "flowlayout.h"

Portrait::Portrait(WidgetManagerI *wm, WidgetI *parent):
    WidgetI(wm,parent)
{
    QHBoxLayout *mainLay = new QHBoxLayout;
    personBack_ = new QLabel;
    faceCornerW_ = new CornerWidget;
    faceL_ = new QLabel;
    faceTextL = new QLabel(tr("Face recognition"));
    bodyTextL = new QLabel(tr("Body recognition"));
    personL_ = new QLabel;
    splitVL_ = new QLabel;
    attributeBack = new QWidget;
    flowLay_ = new FlowLayout;
    mainLay->addWidget(personBack_,1);
    QVBoxLayout *vlay = new QVBoxLayout;
    vlay->addWidget(faceTextL);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(faceL_);
    faceCornerW_->setLayout(hlay);
    vlay->addWidget(faceCornerW_);
    vlay->addWidget(bodyTextL);
    vlay->addWidget(personL_);
    mainLay->addLayout(vlay,1);
    mainLay->addWidget(splitVL_);
    attributeBack->setLayout(flowLay_);
    mainLay->addWidget(attributeBack,2);
    setLayout(mainLay);

    personBack_->setPixmap(QPixmap("images/portrait_back.png"));
    splitVL_->setFixedWidth(1);
    faceL_->setFixedSize(78,78);
    personL_->setFixedSize(73,273);
    attributeBack->setMinimumWidth(200);

    flowLay_->addWidget(new QLabel(tr("femal")));
    flowLay_->addWidget(new QLabel(tr("large")));
    flowLay_->addWidget(new QLabel(tr("black")));
    flowLay_->addWidget(new QLabel(tr("glasses")));
    for(int i = 0; i < flowLay_->count(); i++){
        qobject_cast<QLabel*>(flowLay_->itemAt(i)->widget())->setStyleSheet("QLabel{"
                                                     "background:rgba(255,255,255,0.1);"
                                                     "color: white;"
                                                     "border: 1px solid rgba(255,255,255,1);"
                                                     "border-radius: 10px;"
                                                     "}");
    }
}

void Portrait::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == s){
        pal = palette();
        pal.setColor(QPalette::Background,QColor("#4B4B4B"));
        setPalette(pal);
        setAutoFillBackground(true);

        pal = splitVL_->palette();
        pal.setColor(QPalette::Background,Qt::gray);
        splitVL_->setPalette(pal);
        splitVL_->setAutoFillBackground(true);

        pal = faceCornerW_->palette();
        pal.setColor(QPalette::Foreground,Qt::gray);
        faceCornerW_->setPalette(pal);

        pal = faceTextL->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        faceTextL->setPalette(pal);
        bodyTextL->setPalette(pal);
    }
}
