#include <QLabel>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QBoxLayout>
#include <QStandardPaths>
#include "portrait.h"
#include "cornerwidget.h"
#include "flowlayout.h"
#include "informationdialog.h"

Portrait::Portrait( WidgetI *parent):
    WidgetI(parent)
{
    idL_ = new QLabel;
    nameL_ = new QLabel;
    personType_ = new QLabel;
    faceImgL_ = new QLabel;
    bodyImgL_ = new QLabel;
    saveBtn_ = new QPushButton(tr("Save images"));
    flowLayFace_ = new FlowLayout;
    flowLayBody_ = new FlowLayout;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QBoxLayout *topBox = new QBoxLayout(QBoxLayout::TopToBottom);
    topBox->addWidget(idL_);
    topBox->addWidget(nameL_);
    topBox->addWidget(personType_);
    topBox->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(topBox);
    QHBoxLayout *hlay = new QHBoxLayout;
    faceImgL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    hlay->addWidget(faceImgL_);
    hlay->addLayout(flowLayFace_);
    mainLay->addLayout(hlay);
    hlay = new QHBoxLayout;
    bodyImgL_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Preferred);
    hlay->addWidget(bodyImgL_);
    hlay->addLayout(flowLayBody_);
    mainLay->addLayout(hlay);
    mainLay->addStretch();
    hlay = new QHBoxLayout;
    hlay->addWidget(saveBtn_);
    hlay->setAlignment(Qt::AlignRight);
    mainLay->addLayout(hlay);
    setLayout(mainLay);

    saveBtn_->setMinimumSize(139,33);
    connect(saveBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSaveBtnClicked()));
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

        pal = idL_->palette();
        pal.setColor(QPalette::Foreground,QColor(126,140,177));
        idL_->setPalette(pal);
        nameL_->setPalette(pal);
        personType_->setPalette(pal);

        saveBtn_->setStyleSheet("QPushButton{"
                                "background-color: rgb(83,77,251);"
                                "color: white;"
                                "border-radius: 6px;"
                                "font-size: 18px;"
                                "}"
                                "QPushButton:pressed{"
                                "padding: 2px;"
                                "background-color: #312DA6;"
                                "}");
    }
}

void Portrait::slotSetData(RestServiceI::PortraitReturnData &data)
{
    if(data.id.isEmpty()){
        data.id = tr("unknown");
    }
    if(data.name.isEmpty()){
        data.name = tr("unknown");
    }
    if(data.personType.isEmpty()){
        data.personType = tr("unknown");
    }
    curObjId_ = data.objId;
    curFaceImg_ = data.faceImg;
    curBodyImg_ = data.bodyImg;
    idL_->setText(tr("ID:%1").arg(data.id));
    nameL_->setText(tr("Name:%1").arg(data.name));
    personType_->setText(tr("Person Type:%1").arg(data.personType));
    faceImgL_->setPixmap(QPixmap::fromImage(data.faceImg));
    bodyImgL_->setPixmap(QPixmap::fromImage(data.bodyImg.scaled(data.faceImg.width(), data.faceImg.width() * ((qreal)data.bodyImg.height() / data.bodyImg.width()),Qt::KeepAspectRatio)));
    while (QLayoutItem *item = flowLayFace_->takeAt(0)) {
        delete item;
    }
    for(auto attrStr : data.faceAttrs){
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
    for(auto attrStr : data.bodyAttrs){
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

void Portrait::slotSaveBtnClicked()
{
    QString filePath =  QFileDialog::getExistingDirectory(this,tr("Save face image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));
    if(filePath.isEmpty())return;
    if(!curFaceImg_.save(filePath + "/" + curObjId_ + ".jpg")){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(0);
        infoDialog.setMessage("Operation failed!");
    }

    if(!curBodyImg_.save(filePath + "/" + curObjId_ + "body.jpg")){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(0);
        infoDialog.setMessage("Operation failed!");
    }
}
