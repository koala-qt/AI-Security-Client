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
    faceL_->setScaledContents(true);
    personL_->setScaledContents(true);
    attributeBack->setMinimumWidth(200);
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

void Portrait::slotSetData(QImage face, QImage body, QStringList attributeList)
{
    faceL_->setPixmap(QPixmap::fromImage(face));
    personL_->setPixmap(QPixmap::fromImage(body));
    while (QLayoutItem *item = flowLay_->takeAt(0)) {
        delete item;
    }
    for(auto attrStr : attributeList){
        QLabel *attL = new QLabel(attrStr);
        attL->setStyleSheet("QLabel{"
                            "background:rgba(255,255,255,0.1);"
                            "color: white;"
                            "border: 1px solid rgba(255,255,255,1);"
                            "border-radius: 10px;"
                            "}");
        flowLay_->addWidget(attL);
    }
}
