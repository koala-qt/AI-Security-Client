#include <QLabel>
#include <QDateTimeEdit>
#include <QPushButton>
#include <QPainter>
#include <QComboBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QApplication>
#include <QLineEdit>
#include <QFileDialog>
#include <QListWidget>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
#include "facelinkpage.h"
#include "treecharts.h"
#include "waitinglabel.h"
#include "informationdialog.h"
#include "nodatatip.h"

#pragma execution_character_set("utf-8")
FaceLinkPage::FaceLinkPage( WidgetI *parent) :
  WidgetI(parent)
{
    setObjectName(tr("Facelink"));
    levelCombox_ = new QComboBox;
    imgBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    levelLabel_ = new QLabel(tr("Level"));
    maxnumL_ = new QLabel(tr("Max Number"));
    maxnumEdit_ = new QLineEdit;
    startTimeL_ = new QLabel(tr("Starting Time"));
    endTimeL_ = new QLabel(tr("Ending Time"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    dataView_ = new TreeCharts;
    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *hlay = new QHBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    gridLay->addWidget(levelLabel_,0,0,1,1);
    gridLay->addWidget(levelCombox_,0,1,1,1);
    gridLay->addWidget(maxnumL_,0,2,1,1);
    gridLay->addWidget(maxnumEdit_,0,3,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(searchBtn_,1,4,1,1);
    gridLay->setAlignment(Qt::AlignLeft);
    gridLay->setSpacing(20);
    hlay->addWidget(imgBtn_,10);
    hlay->addLayout(gridLay,159);
    hlay->setAlignment(Qt::AlignLeft);
    hlay->setSpacing(20);
    mainLay->addLayout(hlay,3);
    mainLay->addWidget(dataView_,7);
    mainLay->setContentsMargins(40,40,43,40);
    mainLay->setSpacing(26);
    setLayout(mainLay);

    QPixmap defaultPersonBackPix("images/person-face-back.png");
    imgBtn_->setFixedSize(defaultPersonBackPix.size());
    imgBtn_->setIconSize(imgBtn_->size());
    imgBtn_->setIcon(defaultPersonBackPix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("default-pix",defaultPersonBackPix);
    QCursor imgBtnCoursor = imgBtn_->cursor();
    imgBtnCoursor.setShape(Qt::PointingHandCursor);
    imgBtn_->setCursor(imgBtnCoursor);
    imgBtn_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    levelCombox_->setFixedSize(200,34);
    maxnumEdit_->setFixedSize(200,34);
    maxnumEdit_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    maxnumEdit_->setText("5");
    startTimeEdit_->setFixedSize(200,34);
    endTimeEdit_->setFixedSize(200,34);
    searchBtn_->setFixedSize(99,33);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    imgBtn_->setFocusPolicy(Qt::NoFocus);
    levelCombox_->addItems(QStringList() << tr("1") << tr("2") << tr("3"));

    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(imgBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
    setUserStyle(userStyle());
}

void FaceLinkPage::setUserStyle(int s)
{
    QPalette pal;
    if(0 == s){
        levelCombox_->setStyleSheet("QComboBoxListView{"
                                    "color: #CECECE;"
                                    "background-color: transparent;"
                                    "border-radius: 0px;"
                                    "border: none;"
                                    "}"
                                    "QComboBox{"
                                    "color: rgba(255,255,255,0.75);"
                                    "font-size: 14px;"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "border: none;"
                                    "border-radius: 4px;"
                                    "padding-left: 10px;"
                                    "}"
                                    "QComboBox QAbstractItemView{"
                                    "background-color: rgb(43,49,61);"
                                    "border-radius: 0px;"
                                    "selection-color: white;"
                                    "outline: 0px;"
                                    "selection-background-color: rgba(255,255,255,0.1);"
                                    "}"
                                    "QComboBox::drop-down{"
                                    "subcontrol-position: center right;"
                                    "border-image: url(images/dropdown2.png);"
                                    "width:10px;"
                                    "height:6px;"
                                    "subcontrol-origin: padding;"
                                    "margin-right:5px;"
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
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                    "color: rgba(255,255,255,191);"
                                    "background-color: rgba(255,255,255,0.1);"
                                    "border-radius: 4px;"
                                    "padding-left: 10px;"
                                    "font-size: 14px;"
                                    "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
                                      "color: rgba(255,255,255,191);"
                                      "background-color: rgba(255,255,255,0.1);"
                                      "border-radius: 4px;"
                                      "padding-left: 10px;"
                                      "font-size: 14px;"
                                      "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: rgb(83,77,251);"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "width: 99px;"
                                  "height: 34px;"
                                  "font-size: 14px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: #312DA6;"
                                  "}");
        maxnumEdit_->setStyleSheet("QLineEdit{"
                                   "color: rgba(255,255,255,191);"
                                   "background-color: rgba(255,255,255,0.1);"
                                   "border-radius: 4px;"
                                   "padding-left: 10px;"
                                   "font-size: 14px;"
                                   "}");
        imgBtn_->setStyleSheet("QPushButton{"
                               "background-color: transparent;"
                               "}");

        levelLabel_->setStyleSheet("QLabel{"
                                   "color: rgba(255,255,255,191);"
                                   "font-size: 14px;"
                                   "}");
        startTimeL_->setStyleSheet("QLabel{"
                                   "color: rgba(255,255,255,191);"
                                   "font-size: 14px;"
                                   "}");
        endTimeL_->setStyleSheet("QLabel{"
                                 "color: rgba(255,255,255,191);"
                                 "font-size: 14px;"
                                 "}");
        maxnumL_->setStyleSheet("QLabel{"
                                "color: rgba(255,255,255,191);"
                                "font-size: 14px;"
                                "}");
    }
}

void FaceLinkPage::setFaceLinkOidAndImg(QString oid,QPixmap pix)
{
    imgOid_ = oid;
    imgBtn_->setIcon(pix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("pixmap",pix);
}

void FaceLinkPage::slotSearchBtnClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    RestServiceI::FaceLinkArgs args;
    args.depth = levelCombox_->currentText().toInt();
    args.endT = endTimeEdit_->dateTime();
    args.faceImg = imgBtn_->property("pixmap").value<QPixmap>().toImage();
    args.num = maxnumEdit_->text().toInt();
    args.oid = imgOid_;
    args.startT = startTimeEdit_->dateTime();
    args.thresh = 0.6;
    waitingL_ = new WaitingLabel(dataView_);
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        dataView_->stopWaiting();
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        searchBtn_->setEnabled(true);
    });
    connect(serviceI,SIGNAL(sigFaceLinkFinished(QString)),this,SLOT(slotFaceLinkFinished(QString)));
    serviceI->generateFaceLink(args);
    waitingL_->show(500);
    searchBtn_->setEnabled(false);
}

void FaceLinkPage::slotFaceLinkFinished(QString oid)
{
    qDebug() << "finished face link oid" << oid;
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        dataView_->stopWaiting();
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage(str);
        infoDialog.exec();
        searchBtn_->setEnabled(true);
    });
#if 0
    connect(serviceI,SIGNAL(sigFaceLinkData(RestServiceI::FaceLinkPointData)),this,SLOT(slotFaceLinkData(RestServiceI::FaceLinkPointData)));
    serviceI->getFaceLinkPoint(oid);
#else
    connect(serviceI,SIGNAL(sigFaceLinkTree(QJsonObject)),this,SLOT(slotFaceLinkTree(QJsonObject)));
    serviceI->getFaceLinkTree(oid);
#endif
}

void FaceLinkPage::slotFaceLinkTree(QJsonObject jsObj)
{
    waitingL_->close();
    delete waitingL_;
    waitingL_ = nullptr;
    searchBtn_->setEnabled(true);
    if(jsObj.isEmpty()){
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.setMessage("No matched result !");
        infoDialog.exec();
        return;
    }
#if 0
    TreeCharts *treeView = new TreeCharts(this);
    treeView->setAttribute(Qt::WA_DeleteOnClose);
    treeView->setWindowFlags(Qt::Window | Qt::Dialog);
    treeView->resize(1200,1000);
    treeView->updateData(jsObj);
    treeView->show();
#else
    dataView_->updateData(jsObj);
#endif
}

void FaceLinkPage::slotImgBtnClicked()
{
    QString filePath = QFileDialog::getOpenFileName(this,tr("add image"),QStandardPaths::writableLocation(QStandardPaths::DesktopLocation),"*.png *.jpg *.tmp");
    QPixmap pix(filePath);
    if(pix.isNull()){
        imgBtn_->setIcon(imgBtn_->property("default-pix").value<QPixmap>());
        imgBtn_->setProperty("pixmap",QPixmap());
        return;
    }
    imgBtn_->setIcon(pix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("pixmap",pix);
    imgOid_.clear();
}
