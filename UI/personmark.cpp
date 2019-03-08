#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include "personmark.h"
#include "informationdialog.h"
#include "waitinglabel.h"

PersonMark::PersonMark(QWidget *parent, Qt::WindowFlags f):
    QDialog(parent,f)
{
    setObjectName(tr("Person Labeling"));
    titleL_ = new QLabel(objectName());
    nameL_ = new QLabel(tr("name"));
    nameEdit_ = new QLineEdit;
    idL_ = new QLabel(tr("ID"));
    idEdit_ = new QLineEdit;
    phontoL_ = new QLabel(tr("photo"));
    imgL_ = new QLabel;
    typeL_ = new QLabel(tr("type"));
    typeCombox_ = new QComboBox;
    statusL_ = new QLabel(tr("Status"));
    statusCombox_ = new QComboBox;
    customL_ = new QLabel(tr("describ"));
    customEdit_ = new QLineEdit;
    btnBox_ = new QDialogButtonBox;
    sumitBtn_ = btnBox_->addButton(tr("Submit"),QDialogButtonBox::AcceptRole);
    cancelBtn_ = btnBox_->addButton(tr("Cancel"),QDialogButtonBox::RejectRole);

    QVBoxLayout *mainLay = new QVBoxLayout;
    mainLay->addWidget(titleL_);
    QFormLayout *formLay = new QFormLayout;
    formLay->addRow(nameL_,nameEdit_);
    formLay->addRow(idL_,idEdit_);
    formLay->addRow(phontoL_,imgL_);
    formLay->addRow(typeL_,typeCombox_);
    formLay->addRow(statusL_,statusCombox_);
    formLay->addRow(customL_,customEdit_);
    formLay->setSpacing(20);
    mainLay->addLayout(formLay);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(btnBox_);
    hlay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mainLay->addLayout(hlay);
    mainLay->setSpacing(40);
    mainLay->setContentsMargins(40,40,40,40);
    setLayout(mainLay);

    QPalette pal = palette();
    pal.setColor(QPalette::Background,QColor(48,54,68));
    setPalette(pal);
    imgL_->setFixedSize(180,180);
    nameEdit_->setMinimumHeight(34);
    idEdit_->setMinimumHeight(34);
    typeCombox_->setMinimumHeight(34);
    statusCombox_->setMinimumHeight(34);
    customEdit_->setMinimumHeight(34);
    setMinimumWidth(510);
    setUserStyle(0);
    connect(cancelBtn_,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(sumitBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSubmitBtnClicked()));
    connect(typeCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotOnTypeComboxIndexChanged(int)));
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigPersonTypesResult(QVector<RestServiceI::PersonType>)),this,SLOT(slotOnPersonType(QVector<RestServiceI::PersonType>)));
    serviceI->queryPersonTypes();

    setWindowFlag(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
}

void PersonMark::setPhoto(QImage &img)
{
    imgL_->setPixmap(QPixmap::fromImage(img));
}

void PersonMark::setUserStyle(int s)
{
    QPalette pal;
    QFont f;
    if(s == 0){
        pal = nameL_->palette();
        pal.setColor(QPalette::Foreground,QColor(126,140,177));
        nameL_->setPalette(pal);
        idL_->setPalette(pal);
        phontoL_->setPalette(pal);
        typeL_->setPalette(pal);
        statusL_->setPalette(pal);
        customL_->setPalette(pal);

        pal.setColor(QPalette::Foreground,QColor(194,207,249));
        titleL_->setPalette(pal);

        f = titleL_->font();
        f.setPixelSize(16);
        titleL_->setFont(f);
        sumitBtn_->setFont(f);
        cancelBtn_->setFont(f);

        f.setPixelSize(12);
        idL_->setFont(f);
        phontoL_->setFont(f);
        typeL_->setFont(f);
        statusL_->setFont(f);
        customL_->setFont(f);

        nameEdit_->setStyleSheet("QLineEdit{"
                                 "color: rgb(194,207,249);"
                                 "background: rgb(40,45,56);"
                                 "border: none;"
                                 "}");
        idEdit_->setStyleSheet("QLineEdit{"
                               "color: rgb(194,207,249);"
                               "background: rgb(40,45,56);"
                               "border: none;"
                               "}");
        customEdit_->setStyleSheet("QLineEdit{"
                                   "color: rgb(194,207,249);"
                                   "background: rgb(40,45,56);"
                                   "border: none;"
                                   "}");
        statusCombox_->setStyleSheet("QComboBoxListView{"
                                     "color: #CECECE;"
                                     "background-color: rgb(43,49,61);"
                                     "}"
                                     "QComboBox{"
                                     "color: rgb(126,140,177);"
                                     "font-size: 18px;"
                                     "background-color: rgb(40,45,56);"
                                     "border: none;"
                                     //                    "box-shadow: 1px 1px 0px rgba(77,86,107,1);"
                                     "}"
                                     "QComboBox QAbstractItemView{"
                                     "selection-color: white;"
                                     "outline: 0px;"
                                     "selection-background-color: rgb(71,65,242);"
                                     "}"
                                     "QComboBox::drop-down{"
                                     "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                                     "}"
                                     "QScrollBar:vertical{"
                                     "background: transparent;"
                                     "border: 0px solid gray;"
                                     "width: 13px;"
                                     "}"
                                     "QScrollBar::handle:vertical{"
                                     "background: rgba(255,255,255,0.5);"
                                     "border-radius: 5px;"
                                     "}"
                                     "QScrollBar::add-line:vertical{"
                                     "background: transparent;"
                                     "border:0px solid #274168;"
                                     "border-radius: 5px;"
                                     "min-height: 10px;"
                                     "width: 13px;"
                                     "}"
                                     "QScrollBar::sub-line:vertical{"
                                     "background: transparent;"
                                     "border:0px solid #274168;"
                                     "min-height: 10px;"
                                     "width: 13px;"
                                     "}"
                                     "QScrollBar::up-arrow:vertical{"
                                     "subcontrol-origin: margin;"
                                     "height: 0px;"
                                     "border:0 0 0 0;"
                                     "visible:false;"
                                     "}"
                                     "QScrollBar::down-arrow:vertical{"
                                     "subcontrol-origin: margin;"
                                     "height: 0px;"
                                     "visible:false;"
                                     "}"
                                     "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                     "background: transparent;"
                                     "border: none;"
                                     "border-radius: 0px;"
                                     "}");
        typeCombox_->setStyleSheet("QComboBoxListView{"
                                   "color: #CECECE;"
                                   "background-color: rgb(43,49,61);"
                                   "}"
                                   "QComboBox{"
                                   "color: rgb(126,140,177);"
                                   "font-size: 18px;"
                                   "background-color: rgb(40,45,56);"
                                   "border: none;"
                                   //                    "box-shadow: 1px 1px 0px rgba(77,86,107,1);"
                                   "}"
                                   "QComboBox QAbstractItemView{"
                                   "selection-color: white;"
                                   "outline: 0px;"
                                   "selection-background-color: rgb(71,65,242);"
                                   "}"
                                   "QComboBox::drop-down{"
                                   "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                                   "}"
                                   "QScrollBar:vertical{"
                                   "background: transparent;"
                                   "border: 0px solid gray;"
                                   "width: 13px;"
                                   "}"
                                   "QScrollBar::handle:vertical{"
                                   "background: rgba(255,255,255,0.5);"
                                   "border-radius: 5px;"
                                   "}"
                                   "QScrollBar::add-line:vertical{"
                                   "background: transparent;"
                                   "border:0px solid #274168;"
                                   "border-radius: 5px;"
                                   "min-height: 10px;"
                                   "width: 13px;"
                                   "}"
                                   "QScrollBar::sub-line:vertical{"
                                   "background: transparent;"
                                   "border:0px solid #274168;"
                                   "min-height: 10px;"
                                   "width: 13px;"
                                   "}"
                                   "QScrollBar::up-arrow:vertical{"
                                   "subcontrol-origin: margin;"
                                   "height: 0px;"
                                   "border:0 0 0 0;"
                                   "visible:false;"
                                   "}"
                                   "QScrollBar::down-arrow:vertical{"
                                   "subcontrol-origin: margin;"
                                   "height: 0px;"
                                   "visible:false;"
                                   "}"
                                   "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical{"
                                   "background: transparent;"
                                   "border: none;"
                                   "border-radius: 0px;"
                                   "}");
        cancelBtn_->setStyleSheet("QPushButton{"
                                  "color: white;"
                                  "width: 120px;"
                                  "height: 44px;"
                                  "background-color: #4741F2;"
                                  "border: none;"
                                  "border-radius: 4px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "background-color: rgba(71,65,242,0.3);"
                                  "border:1px solid rgba(71,65,242,1);"
                                  "padding: 2px;"
                                  "}");
        sumitBtn_->setStyleSheet("QPushButton{"
                                 "color: white;"
                                 "width: 120px;"
                                 "height: 44px;"
                                 "background-color: #4741F2;"
                                 "border: none;"
                                 "border-radius: 4px;"
                                 "}"
                                 "QPushButton:pressed{"
                                 "background-color: rgba(71,65,242,0.3);"
                                 "border:1px solid rgba(71,65,242,1);"
                                 "padding: 2px;"
                                 "}");
    }
}

void PersonMark::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.setPen(Qt::NoPen);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(palette().color(QPalette::Background));
    p.drawRoundedRect(rect().adjusted(0,0,-p.pen().width(),-p.pen().width()),4,4);
}

void PersonMark::mousePressEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    if(event->button() == Qt::RightButton)return;
    startP_ = event->globalPos();
}

void PersonMark::mouseMoveEvent(QMouseEvent *event)
{
    if(event->button() == Qt::RightButton || startP_.isNull())return;
    move(pos() + (event->globalPos() - startP_));
    startP_ = event->globalPos();
}

void PersonMark::mouseReleaseEvent(QMouseEvent *event)
{
    qSwap(startP_,QPoint());
}

void PersonMark::parseGroupToVec(RestServiceI::PersonGroupInfo &datas, QVector<PersonGroup> &Vec)
{
    PersonGroup pdata;
    pdata.no = datas.no;
    pdata.name = datas.name;
    pdata.id = datas.id;
    pdata.description = datas.description;
    Vec << pdata;
    for(RestServiceI::PersonGroupInfo &value : datas.children){
        parseGroupToVec(value,Vec);
    }
}

void PersonMark::slotOnPersonType(QVector<RestServiceI::PersonType> data)
{
    for(RestServiceI::PersonType &value :data){
        typeCombox_->addItem(value.strTypeName,value.strTypeNo);
        typeCombox_->setItemData(typeCombox_->count()-1,value.groupNo,Qt::UserRole + 1);
        for(int j = 0; j < value.lstChildren.count(); j++){
            if(value.lstChildren.at(j).strDescription == "tagging"){
                typeCombox_->setItemData(typeCombox_->count()-1,value.lstChildren.at(j).strName,Qt::UserRole + 2);
                break;
            }
        }
        QString groupNo = value.groupNo;

        ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
        RestServiceI *serviceI = factoryI->makeRestServiceI();
        connect(serviceI,&RestServiceI::sigPersonGroupInfos,this,[this,groupNo](QVector<RestServiceI::PersonGroupInfo> infos){
            QVector<PersonGroup> dataVec;
            for(RestServiceI::PersonGroupInfo &value : infos){
                parseGroupToVec(value,dataVec);
            }
            personTypeGroupMap_.insert(groupNo,dataVec);
            if(statusCombox_->count() == 0){
                slotOnTypeComboxIndexChanged(0);
            }
        });
        connect(serviceI,&RestServiceI::sigError,this,[this,groupNo](QString str){
            qDebug() << str;
            personTypeGroupMap_.insert(groupNo,QVector<PersonGroup>());
            if(statusCombox_->count() == 0){
                slotOnTypeComboxIndexChanged(0);
            }
        });
        serviceI->getPersonGoupInfos(groupNo);
    }
}

void PersonMark::slotOnTypeComboxIndexChanged(int index)
{
    statusCombox_->clear();
    QVector<PersonGroup> personGroupVec = personTypeGroupMap_.value(typeCombox_->itemData(index,Qt::UserRole + 1).toString());
    for(const PersonGroup & value : personGroupVec){
        statusCombox_->addItem(value.name,value.no);
    }
}

void PersonMark::slotSubmitBtnClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    WaitingLabel *label = new WaitingLabel(this);
    connect(serviceI,&RestServiceI::sigResultState,this,[this,label](bool res){
        label->close();
        delete label;
        accept();
    });
    connect(serviceI,&RestServiceI::sigError,this,[this,label](QString str){
        label->close();
        delete label;
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(0);
        infoDialog.setMessage(str);
        infoDialog.exec();
    });
    RestServiceI::PersonRegisterArgs args;
    args.describ = customEdit_->text();
    args.groupName = statusCombox_->currentText();
    args.groupNo = statusCombox_->currentData().toString();
    args.id = idEdit_->text();
    args.image = imgL_->pixmap()->toImage();
    args.name = nameEdit_->text();
    args.personTypeName = typeCombox_->currentText();
    args.personTypeNo = typeCombox_->currentData().toString();
    args.taggingKey = typeCombox_->currentData(Qt::UserRole + 2).toString();
    serviceI->personRegist(args);
    label->show(500);
}
