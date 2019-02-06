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
#include "facelinkpage.h"
#include "treecharts.h"
#include "service/restservice.h"
#include "service/notifyservicei.h"
#include "waitinglabel.h"

#pragma execution_character_set("utf-8")
FaceLinkPage::FaceLinkPage(WidgetManagerI *wm, WidgetI *parent) :
  WidgetI(wm,parent)
{
    setObjectName(tr("Face Link"));
    backImg_.load("images/Mask.png");
    levelCombox_ = new QComboBox;
    imgBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("search"));
    levelLabel_ = new QLabel(tr("Level"));
    maxnumL_ = new QLabel(tr("Max number"));
    maxnumEdit_ = new QLineEdit;
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
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
    hlay->addWidget(imgBtn_,10);
    hlay->addLayout(gridLay,159);
    hlay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(hlay,3);
    mainLay->addWidget(dataView_,7);
    setLayout(mainLay);

    imgBtn_->setFixedSize(95,95);
    imgBtn_->setIconSize(imgBtn_->size());
    QPixmap defaultPersonBackPix("images/person-face-back.png");
    imgBtn_->setIcon(defaultPersonBackPix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("default-pix",defaultPersonBackPix);
    QCursor imgBtnCoursor = imgBtn_->cursor();
    imgBtnCoursor.setShape(Qt::PointingHandCursor);
    imgBtn_->setCursor(imgBtnCoursor);
    imgBtn_->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    levelCombox_->setMinimumHeight(44);
    maxnumEdit_->setMaximumWidth(250);
    maxnumEdit_->setMinimumHeight(44);
    maxnumEdit_->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    maxnumEdit_->setText("10");
    startTimeEdit_->setMinimumSize(250,44);
    endTimeEdit_->setMinimumSize(250,44);
    searchBtn_->setMinimumSize(120,44);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    imgBtn_->setFocusPolicy(Qt::NoFocus);
    levelCombox_->addItems(QStringList() << tr("2") << tr("3") << tr("4") << tr("5") << tr("6"));
    levelCombox_->setCurrentIndex(1);

    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));
    connect(imgBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
}

void FaceLinkPage::setUserStyle(WidgetManagerI::SkinStyle s)
{
    QPalette pal;
    if(WidgetManagerI::Danyahei == s){
        levelCombox_->setStyleSheet(
                    "QComboBoxListView{"
                    "color: #CECECE;"
                    "background-color: #525964;"
                    "}"
                    "QComboBox{"
                    "color: white;"
                    "font-size: 16px;"
                    "background-color: transparent;"
                    "border: 1px solid rgba(255, 255, 255, 1);"
                    "border-radius: 4px;"
                    "}"
                    "QComboBox QAbstractItemView{"
                    "selection-color: white;"
                    "outline: 0px;"
                    "selection-background-color: #CECECE;"
                    "}"
                    "QComboBox::drop-down{"
                    "subcontrol-position: center right;border-image: url(images/dropdown2.png);width:11px;height:8px;subcontrol-origin: padding;margin-right:5px;"
                    "}");
        endTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        startTimeEdit_->setStyleSheet("QDateEdit,QTimeEdit,QComboBox,QDateTimeEdit,QSpinBox,QDoubleSpinBox{"
            "color: rgba(206, 206, 206, 1);"
            "border:1px solid white;"
            "border-radius:4px;"
            "background-color: transparent;"
            "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                   "background-color: #B4A06C;"
                                   "color: white;"
                                   "border-radius: 6px;"
                                   "font-size:18px;"
                                   "}"
                                   "QPushButton:pressed{"
                                   "padding: 2px;"
                                   "}");
        maxnumEdit_->setStyleSheet("QLineEdit{"
                                   "color: white;"
                                   "border-radius: 4px;"
                                   "border: 1px solid white;"
                                   "background-color: transparent;"
                                   "}");
        imgBtn_->setStyleSheet("QPushButton{"
                               "background-color: transparent;"
                               "}");
        pal = levelLabel_->palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        levelLabel_->setPalette(pal);
        startTimeL_->setPalette(pal);
        endTimeL_->setPalette(pal);
        maxnumL_->setPalette(pal);
    }
}

void FaceLinkPage::setFaceLinkOidAndImg(QString oid,QPixmap pix)
{
    imgOid_ = oid;
    imgBtn_->setIcon(pix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("pixmap",pix);
}

void FaceLinkPage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter p(this);
    p.drawImage(rect(),backImg_);
}

void FaceLinkPage::slotSearchBtnClicked()
{
    if(waitingL_){
        return;
    }
#if 0
    NotifyServiceI *notifyServiceI_ = dynamic_cast<NotifyServiceI*>(getWoker("NotifyService"));
    notifyServiceI_->disconnect(SIGNAL(sigFaceLinkDataFinished(QString)));
    connect(notifyServiceI_,SIGNAL(sigFaceLinkDataFinished(QString)),this,SLOT(slotFaceLinkFinished(QString)));
#else
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    RestServiceI::FaceLinkArgs args;
    args.depth = levelCombox_->currentText().toInt();
    args.endT = endTimeEdit_->dateTime();
    args.faceImg = imgBtn_->property("pixmap").value<QPixmap>().toImage();
    args.num = maxnumEdit_->text().toInt();
    args.oid = imgOid_;
    args.startT = startTimeEdit_->dateTime();
    args.thresh = 0.6;
    waitingL_ = new WaitingLabel(this);
#if 1
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        QMessageBox::information(this,objectName(),str);
        searchBtn_->setEnabled(true);
    });
    connect(serviceI,SIGNAL(sigFaceLinkFinished(QString)),this,SLOT(slotFaceLinkFinished(QString)));
#endif
#endif
    serviceI->generateFaceLink(args);
    startWorker(worker);
    waitingL_->show(500);
    searchBtn_->setEnabled(false);
}

void FaceLinkPage::slotFaceLinkFinished(QString oid)
{
    qDebug() << "finished face link oid" << oid;
    BLL::Worker * worker = new BLL::RestService(widgetManger()->workerManager());
    RestServiceI *serviceI = dynamic_cast<RestServiceI*>(worker);
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        waitingL_->close();
        delete waitingL_;
        waitingL_ = nullptr;
        QMessageBox::information(this,objectName(),str);
        searchBtn_->setEnabled(true);
    });
#if 0
    connect(serviceI,SIGNAL(sigFaceLinkData(RestServiceI::FaceLinkPointData)),this,SLOT(slotFaceLinkData(RestServiceI::FaceLinkPointData)));
    serviceI->getFaceLinkPoint(oid);
#else
    connect(serviceI,SIGNAL(sigFaceLinkTree(QJsonObject)),this,SLOT(slotFaceLinkTree(QJsonObject)));
    serviceI->getFaceLinkTree(oid);
#endif
    startWorker(worker);
}

void FaceLinkPage::slotFaceLinkTree(QJsonObject jsObj)
{
    waitingL_->close();
    delete waitingL_;
    waitingL_ = nullptr;
    searchBtn_->setEnabled(true);
    if(jsObj.isEmpty()){
        QMessageBox::information(this,objectName(),tr("No matched result !"));
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
        return;
    }
    imgBtn_->setIcon(pix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("pixmap",pix);
    imgOid_.clear();
}
