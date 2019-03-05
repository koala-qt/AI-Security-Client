#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QDebug>
#include "personmark.h"
#include "informationdialog.h"
#include "waitinglabel.h"

PersonMark::PersonMark(PersonMark *parent):
    QDialog(parent)
{
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
    QFormLayout *formLay = new QFormLayout;
    formLay->addRow(nameL_,nameEdit_);
    formLay->addRow(idL_,idEdit_);
    formLay->addRow(phontoL_,imgL_);
    formLay->addRow(typeL_,typeCombox_);
    formLay->addRow(statusL_,statusCombox_);
    formLay->addRow(customL_,customEdit_);
    mainLay->addLayout(formLay);
    QHBoxLayout *hlay = new QHBoxLayout;
    hlay->addWidget(btnBox_);
    hlay->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    mainLay->addLayout(hlay);
    setLayout(mainLay);

    connect(cancelBtn_,SIGNAL(clicked(bool)),this,SLOT(reject()));
    connect(sumitBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSubmitBtnClicked()));
    connect(typeCombox_,SIGNAL(currentIndexChanged(int)),this,SLOT(slotOnTypeComboxIndexChanged(int)));
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigPersonTypesResult(QVector<RestServiceI::PersonType>)),this,SLOT(slotOnPersonType(QVector<RestServiceI::PersonType>)));
    serviceI->queryPersonTypes();
}

void PersonMark::setPhoto(QImage &img)
{
    imgL_->setPixmap(QPixmap::fromImage(img));
}

void PersonMark::setUserStyle(int s)
{

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
    serviceI->personRegist(args);
    label->show(500);
}
