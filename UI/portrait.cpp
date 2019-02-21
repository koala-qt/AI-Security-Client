#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include "portrait.h"
#include "cornerwidget.h"
#include "flowlayout.h"

Portrait::Portrait( WidgetI *parent):
    WidgetI(parent)
{
    QHBoxLayout *mainLay = new QHBoxLayout;
    personBack_ = new QLabel;
    faceCornerW_ = new CornerWidget;
    faceL_ = new QLabel;
    faceTextL = new QLabel(tr("Face recognition"));
    bodyTextL = new QLabel(tr("Body recognition"));
    personL_ = new QLabel;
    splitVL_ = new QLabel;
    faceAttributeTitleL_ = new QLabel(tr("Face attribute"));
    bodyAttributeTitleL_ = new QLabel(tr("body attribute"));
    attributeSpliteL_ = new QLabel;
    faceAttributeBack = new QWidget;
    bodyAttributeBack_ = new QWidget;
    flowLayFace_ = new FlowLayout;
    flowLayBody_ = new FlowLayout;
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
    vlay = new QVBoxLayout;
    vlay->addWidget(faceAttributeTitleL_);
    faceAttributeBack->setLayout(flowLayFace_);
    vlay->addWidget(faceAttributeBack);
    vlay->addWidget(attributeSpliteL_);
    vlay->addWidget(bodyAttributeTitleL_);
    bodyAttributeBack_->setLayout(flowLayBody_);
    vlay->addWidget(bodyAttributeBack_);
    mainLay->addLayout(vlay,2);
    setLayout(mainLay);

    personBack_->setPixmap(QPixmap("images/portrait_back.png"));
    splitVL_->setFixedWidth(1);
    attributeSpliteL_->setFixedHeight(1);
    faceAttributeTitleL_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    bodyAttributeTitleL_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    faceL_->setFixedSize(78,78);
    personL_->setFixedSize(73,273);
    faceL_->setScaledContents(true);
    personL_->setScaledContents(true);
    faceAttributeBack->setMinimumWidth(325);

    setUserStyle(userStyle());
}

void Portrait::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(0 == s){
        if(parentWidget()){
            f = font();
            f.setFamily(parentWidget()->font().family());
            setFont(f);
        }
        pal = palette();
        pal.setColor(QPalette::Background,QColor("#4B4B4B"));
        setPalette(pal);
        setAutoFillBackground(true);

        pal = splitVL_->palette();
        pal.setColor(QPalette::Background,Qt::gray);
        splitVL_->setPalette(pal);
        splitVL_->setAutoFillBackground(true);
        attributeSpliteL_->setPalette(pal);
        attributeSpliteL_->setAutoFillBackground(true);

        pal = faceCornerW_->palette();
        pal.setColor(QPalette::Foreground,Qt::gray);
        faceCornerW_->setPalette(pal);

        pal = faceTextL->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        faceTextL->setPalette(pal);
        bodyTextL->setPalette(pal);
        faceAttributeTitleL_->setPalette(pal);
        bodyAttributeTitleL_->setPalette(pal);
    }
}

void Portrait::slotSetData(QImage face, QImage body, QStringList attributeList, QStringList bodyAttributeList)
{
    faceL_->setPixmap(QPixmap::fromImage(face));
    personL_->setPixmap(QPixmap::fromImage(body));
    while (QLayoutItem *item = flowLayFace_->takeAt(0)) {
        delete item;
    }
    for(auto attrStr : attributeList){
        QLabel *attL = new QLabel(attrStr);
        attL->setStyleSheet("QLabel{"
                            "background:rgba(255,255,255,0.1);"
                            "color: white;"
                            "border: 1px solid rgba(255,255,255,1);"
                            "border-radius: 2px;"
                            "}");
        QFont f = attL->font();
        f.setFamily(font().family());
        setFont(f);
        flowLayFace_->addWidget(attL);
    }
    while (QLayoutItem *item = flowLayBody_->takeAt(0)) {
        delete item;
    }
    for(auto attrStr : bodyAttributeList){
        QLabel *attL = new QLabel(attrStr);
        attL->setStyleSheet("QLabel{"
                            "background:rgba(255,255,255,0.1);"
                            "color: white;"
                            "border: 1px solid rgba(255,255,255,1);"
                            "border-radius: 2px;"
                            "}");
        QFont f = attL->font();
        f.setFamily(font().family());
        setFont(f);
        flowLayBody_->addWidget(attL);
    }
}
