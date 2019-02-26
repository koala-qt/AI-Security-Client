#include <QPushButton>
#include <QLabel>
#include <QDateTimeEdit>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QMessageBox>
#include <QSettings>
#include <QFileDialog>
#include <QStandardPaths>
#include <QApplication>
#include <QTimer>
#include "trackingpage.h"
#include "trackingwebview.h"
#include "waitinglabel.h"
#include "informationdialog.h"

TrackingPage::TrackingPage( WidgetI *parent):
    WidgetI(parent)
{
    setObjectName(tr("Tracking"));
    imgBtn_ = new QPushButton;
    searchBtn_ = new QPushButton(tr("Search"));
    startTimeL_ = new QLabel(tr("Starting time"));
    endTimeL_ = new QLabel(tr("Ending time"));
    threshL_ = new QLabel(tr("Thresh"));
    startTimeEdit_ = new QDateTimeEdit;
    endTimeEdit_ = new QDateTimeEdit;
    threshSpin_ = new QSpinBox;
    dataView_ = new TrackingWebView;

    QVBoxLayout *mainLay = new QVBoxLayout;
    QHBoxLayout *topHLay = new QHBoxLayout;
    QGridLayout *gridLay = new QGridLayout;
    topHLay->addWidget(imgBtn_);
    gridLay->addWidget(threshL_,0,0,1,1);
    gridLay->addWidget(threshSpin_,0,1,1,1);
    gridLay->addWidget(startTimeL_,1,0,1,1);
    gridLay->addWidget(startTimeEdit_,1,1,1,1);
    gridLay->addWidget(endTimeL_,1,2,1,1);
    gridLay->addWidget(endTimeEdit_,1,3,1,1);
    gridLay->addWidget(searchBtn_,1,4,1,1);
    topHLay->addLayout(gridLay);
    topHLay->setAlignment(Qt::AlignLeft);
    mainLay->addLayout(topHLay);
    mainLay->addWidget(dataView_);
    setLayout(mainLay);

    threshSpin_->setRange(0,100);
    threshSpin_->setSuffix("%");
    threshSpin_->setMinimumHeight(40);
    startTimeEdit_->setMinimumSize(250,44);
    endTimeEdit_->setMinimumSize(250,44);
    startTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    endTimeEdit_->setDisplayFormat("yyyy/MM/dd HH:mm:ss");
    startTimeEdit_->setDateTime(QDateTime::currentDateTime().addDays(-1));
    endTimeEdit_->setDateTime(QDateTime::currentDateTime());
    threshSpin_->setValue(40);
    searchBtn_->setMinimumSize(120,44);
    imgBtn_->setFixedSize(95,95);
    imgBtn_->setIconSize(imgBtn_->size());
    QPixmap defaultPersonBackPix("images/person-face-back.png");
    imgBtn_->setIcon(defaultPersonBackPix.scaled(imgBtn_->iconSize()));
    imgBtn_->setProperty("default-pix",defaultPersonBackPix);
    QCursor imgBtnCoursor = imgBtn_->cursor();
    imgBtnCoursor.setShape(Qt::PointingHandCursor);
    imgBtn_->setCursor(imgBtnCoursor);
    imgBtn_->setFocusPolicy(Qt::NoFocus);

    connect(imgBtn_,SIGNAL(clicked(bool)),this,SLOT(slotImgBtnClicked()));
    connect(searchBtn_,SIGNAL(clicked(bool)),this,SLOT(slotSearchBtnClicked()));

    QSettings configSetting("config.ini",QSettings::IniFormat);
    hostname_ = configSetting.value("Http/Javahost").toString();

    setUserStyle(userStyle());
    getCameraInfo();
}

void TrackingPage::setUserStyle(int s)
{
    QPalette pal;
    if(0 == s){
        imgBtn_->setStyleSheet("QPushButton{"
                               "background-color: transparent;"
                               "}");
        pal = palette();
        pal.setColor(QPalette::Foreground,Qt::white);
        setPalette(pal);

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
                                  "color: white;"
                                  "background-color: rgba(112, 112, 112, 1);"
                                  "}"
                                  "QPushButton:pressed{"
                                  "background-color: rgba(255,0,0,100);"
                                  "}");
        threshSpin_->setStyleSheet("QSpinBox{"
                                 "padding-right: 15px;"
                                 "border-width: 3;"
                                 "background-color: transparent;"
                                 "border:1px solid #CECECE;"
                                    "border-radius:6px;"
                                 "color: white;"
                                 "font-size: 18px;"
                                 "}"
                                 "QSpinBox::up-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: top right;"
                                 "width: 16px;"
                                 "border-image: url(images/on.png) 1;"
                                 "}"
                                 "QSpinBox::down-button{"
                                 "subcontrol-origin: border;"
                                 "subcontrol-position: bottom right;"
                                 "width: 16px;"
                                 "border-image: url(images/under.png) 1;"
                                 "}");
        searchBtn_->setStyleSheet("QPushButton{"
                                  "background-color: #B4A06C;"
                                  "color: white;"
                                  "border-radius: 6px;"
                                  "font-size:18px;"
                                  "}"
                                  "QPushButton:pressed{"
                                  "padding: 2px;"
                                  "background-color: rgba(255,0,0,100);"
                                  "}");
    }
}

void TrackingPage::setImgageOid(QImage img, QString oid)
{
    curOid_ = oid;
    QPixmap pix = QPixmap::fromImage(img.scaled(imgBtn_->iconSize()));
    imgBtn_->setIcon(pix);
    imgBtn_->setProperty("pixmap",pix);
}

void TrackingPage::getCameraInfo()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    connect(serviceI,SIGNAL(sigCameraInfo(QVector<RestServiceI::CameraInfo>)),this,SLOT(slotOnCameraInfo(QVector<RestServiceI::CameraInfo>)));
    serviceI->getCameraInfo();
}

void TrackingPage::slotImgBtnClicked()
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
    curOid_.clear();
}

void TrackingPage::slotSearchBtnClicked()
{
    ServiceFactoryI *factoryI = reinterpret_cast<ServiceFactoryI*>(qApp->property("ServiceFactoryI").toULongLong());
    RestServiceI *serviceI = factoryI->makeRestServiceI();
    RestServiceI::FaceTrackingArgs args;
    args.oid = curOid_;
    args.faceImg = imgBtn_->property("pixmap").value<QPixmap>().toImage();
    args.startT = startTimeEdit_->dateTime();
    args.endT = endTimeEdit_->dateTime();
    args.thresh = threshSpin_->value() / qreal(100);
    connect(serviceI,&RestServiceI::sigError,this,[this](QString str){
        dataView_->updateTracking(QVector<TrackingWebView::TrackingPoint>());
        InformationDialog infoDialog(this);
        infoDialog.setUserStyle(userStyle());
        infoDialog.showMessage(str);
        infoDialog.exec();
        searchBtn_->setEnabled(true);
    });
#if 0
    connect(serviceI,&RestServiceI::sigTracking,this,[this,label](const QVector<SearchFace> data){
        label->close();
        delete label;
        slotTracking(data);
        searchBtn_->setEnabled(true);
    });
#else
    connect(serviceI,&RestServiceI::sigTrackingNew,this,[this](const QVector<RestServiceI::TrackingReturnData> data){
        slotTrackingNew(data);
        searchBtn_->setEnabled(true);
    });
#endif
    serviceI->faceTracking(args);
    searchBtn_->setEnabled(false);
    QTimer::singleShot(500,this,[this]{
        if(!searchBtn_->isEnabled()){
            dataView_->startWaiting();
        }
    });
}

void TrackingPage::slotOnCameraInfo(QVector<RestServiceI::CameraInfo> data)
{
    for(RestServiceI::CameraInfo &info : data){
        curCameraMap_[info.cameraId] = info.cameraPos;
    }
}

void TrackingPage::slotTrackingNew(QVector<RestServiceI::TrackingReturnData> data)
{
    QVector<TrackingWebView::TrackingPoint> trackingVec;
    std::transform(data.begin(),data.end(),std::back_inserter(trackingVec),[this](const RestServiceI::TrackingReturnData &value){
        TrackingWebView::TrackingPoint pointData;
        pointData.cameraId = value.cameraId.toInt();
        pointData.name = curCameraMap_.value(value.cameraId);
        pointData.grabTime = value.timeIn.toString("yyyy-MM-dd HH:mm:ss");
        int holdTime = (value.timeOut.toMSecsSinceEpoch() - value.timeIn.toMSecsSinceEpoch())/1000;
        pointData.holdTime.setNum(holdTime < 1 ? 1 : holdTime);
        pointData.personImgUr = hostname_ + "graph/node/picture/" + value.objId;
        pointData.lat = value.lat;
        pointData.lng = value.lng;
        pointData.sceneId = value.sceneId;
//        qDebug() << pointData.cameraId << pointData.name << pointData.grabTime << pointData.holdTime << pointData.personImgUr << pointData.lat << pointData.lng;
        return pointData;
    });
    dataView_->updateTracking(trackingVec);
}

void TrackingPage::slotTracking(QVector<SearchFace> data)
{
    QVector<TrackingWebView::TrackingPoint> trackingVec;
    std::transform(data.begin(),data.end(),std::back_inserter(trackingVec),[this](const SearchFace &value){
        TrackingWebView::TrackingPoint pointData;
        pointData.name = QString::fromStdString(value.camera_pos);
        pointData.grabTime = QDateTime::fromMSecsSinceEpoch(value.time).toString("yyyy-MM-dd HH:mm:ss");
        pointData.personImgUr = hostname_ + "graph/node/picture/" + QString::fromStdString(value.oid);
        return pointData;
    });
    dataView_->updateTracking(trackingVec);
}
